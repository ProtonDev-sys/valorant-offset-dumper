#include "pattern_scanner.hpp"

#define NOMINMAX
#include <Windows.h>

#include <algorithm>
#include <charconv>
#include <cstring>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

namespace
{
    bool is_wildcard_token(const std::string& token)
    {
        return token == "?" || token == "??";
    }
}

PatternScanner::PatternScanner(const std::filesystem::path& image_path)
{
    std::ifstream input(image_path, std::ios::binary);
    if (!input)
    {
        throw std::runtime_error("failed to open binary: " + image_path.string());
    }

    image_ = std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    if (image_.size() < sizeof(IMAGE_DOS_HEADER))
    {
        throw std::runtime_error("binary is too small to be a valid PE image");
    }

    const auto* dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(image_.data());
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        throw std::runtime_error("invalid DOS signature");
    }

    const auto nt_header_offset = static_cast<std::size_t>(dos_header->e_lfanew);
    if (nt_header_offset + sizeof(IMAGE_NT_HEADERS64) > image_.size())
    {
        throw std::runtime_error("invalid NT header offset");
    }

    const auto* nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS64*>(image_.data() + nt_header_offset);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
    {
        throw std::runtime_error("invalid NT signature");
    }

    const auto* section = IMAGE_FIRST_SECTION(nt_headers);
    for (unsigned index = 0; index < nt_headers->FileHeader.NumberOfSections; ++index, ++section)
    {
        if ((section->Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
        {
            continue;
        }

        const auto raw_offset = static_cast<std::size_t>(section->PointerToRawData);
        const auto raw_size = static_cast<std::size_t>(section->SizeOfRawData);
        if (raw_offset >= image_.size())
        {
            continue;
        }

        const auto bytes_available = std::min(raw_size, image_.size() - raw_offset);

        SectionView view{};
        view.name = std::string(reinterpret_cast<const char*>(section->Name), strnlen_s(reinterpret_cast<const char*>(section->Name), IMAGE_SIZEOF_SHORT_NAME));
        view.rva = section->VirtualAddress;
        view.bytes.insert(view.bytes.end(), image_.begin() + raw_offset, image_.begin() + raw_offset + bytes_available);
        executable_sections_.push_back(std::move(view));
    }

    if (executable_sections_.empty())
    {
        throw std::runtime_error("no executable sections found");
    }
}

std::optional<ResolvedOffset> PatternScanner::resolve(const SignatureDefinition& signature) const
{
    last_error_.clear();

    const auto compiled = compile_pattern(signature.pattern);
    const auto matches = find_matches(compiled);
    if (matches.empty())
    {
        last_error_ = "pattern not found";
        return std::nullopt;
    }

    std::vector<std::uint32_t> resolved_rvas;
    resolved_rvas.reserve(matches.size());

    for (const auto& match : matches)
    {
        switch (signature.kind)
        {
        case SignatureKind::DirectRva:
            resolved_rvas.push_back(match.rva);
            break;
        case SignatureKind::RipRelative32:
            resolved_rvas.push_back(read_rip_relative_target(match, signature));
            break;
        }
    }

    const auto first = resolved_rvas.front();
    const auto all_same = std::all_of(resolved_rvas.begin(), resolved_rvas.end(), [first](const std::uint32_t value) {
        return value == first;
    });

    if (matches.size() > 1 && !(signature.allow_multiple_matches_with_same_target && all_same))
    {
        std::ostringstream stream;
        stream << "pattern matched " << matches.size() << " locations";
        last_error_ = stream.str();
        return std::nullopt;
    }

    if (!all_same)
    {
        last_error_ = "pattern resolved to multiple different targets";
        return std::nullopt;
    }

    return ResolvedOffset{std::string(signature.name), first, matches.size()};
}

const std::string& PatternScanner::last_error() const noexcept
{
    return last_error_;
}

PatternScanner::CompiledPattern PatternScanner::compile_pattern(std::string_view pattern) const
{
    std::istringstream stream{std::string(pattern)};
    std::string token;

    CompiledPattern compiled{};
    while (stream >> token)
    {
        if (is_wildcard_token(token))
        {
            compiled.bytes.push_back(-1);
            continue;
        }

        unsigned value = 0;
        const auto* begin = token.data();
        const auto* end = token.data() + token.size();
        const auto [ptr, error] = std::from_chars(begin, end, value, 16);
        if (error != std::errc{} || ptr != end || value > 0xFF)
        {
            throw std::runtime_error("invalid pattern token: " + token);
        }

        compiled.bytes.push_back(static_cast<int>(value));
    }

    std::size_t best_offset = 0;
    std::size_t best_length = 0;
    std::size_t current_offset = 0;
    std::size_t current_length = 0;
    bool in_anchor = false;

    for (std::size_t index = 0; index < compiled.bytes.size(); ++index)
    {
        if (compiled.bytes[index] >= 0)
        {
            if (!in_anchor)
            {
                current_offset = index;
                current_length = 0;
                in_anchor = true;
            }

            ++current_length;
            if (current_length > best_length)
            {
                best_offset = current_offset;
                best_length = current_length;
            }
        }
        else
        {
            in_anchor = false;
        }
    }

    if (best_length == 0)
    {
        throw std::runtime_error("pattern cannot be all wildcards");
    }

    compiled.anchor_offset = best_offset;
    compiled.anchor.reserve(best_length);
    for (std::size_t index = best_offset; index < best_offset + best_length; ++index)
    {
        compiled.anchor.push_back(static_cast<std::uint8_t>(compiled.bytes[index]));
    }

    return compiled;
}

std::vector<PatternScanner::MatchLocation> PatternScanner::find_matches(const CompiledPattern& pattern) const
{
    std::vector<MatchLocation> matches;
    const auto pattern_size = pattern.bytes.size();

    for (std::size_t section_index = 0; section_index < executable_sections_.size(); ++section_index)
    {
        const auto& section = executable_sections_[section_index];
        if (section.bytes.size() < pattern_size)
        {
            continue;
        }

        auto search_start = section.bytes.begin();
        const auto search_end = section.bytes.end();

        while (search_start != search_end)
        {
            const auto anchor_it = std::search(search_start, search_end, pattern.anchor.begin(), pattern.anchor.end());
            if (anchor_it == search_end)
            {
                break;
            }

            const auto anchor_index = static_cast<std::size_t>(std::distance(section.bytes.begin(), anchor_it));
            if (anchor_index < pattern.anchor_offset)
            {
                search_start = std::next(anchor_it);
                continue;
            }

            const auto match_offset = anchor_index - pattern.anchor_offset;
            if (match_offset + pattern_size > section.bytes.size())
            {
                break;
            }

            bool matched = true;
            for (std::size_t index = 0; index < pattern_size; ++index)
            {
                const auto expected = pattern.bytes[index];
                if (expected >= 0 && section.bytes[match_offset + index] != static_cast<std::uint8_t>(expected))
                {
                    matched = false;
                    break;
                }
            }

            if (matched)
            {
                matches.push_back(MatchLocation{
                    section_index,
                    match_offset,
                    section.rva + static_cast<std::uint32_t>(match_offset),
                });
            }

            search_start = std::next(anchor_it);
        }
    }

    return matches;
}

std::uint32_t PatternScanner::read_rip_relative_target(const MatchLocation& match, const SignatureDefinition& signature) const
{
    const auto& section = executable_sections_[match.section_index];
    const auto displacement_offset = static_cast<std::size_t>(signature.displacement_offset);
    if (match.section_offset + displacement_offset + sizeof(std::int32_t) > section.bytes.size())
    {
        throw std::runtime_error("rip-relative displacement goes out of section bounds");
    }

    std::int32_t displacement = 0;
    std::memcpy(&displacement, section.bytes.data() + match.section_offset + displacement_offset, sizeof(displacement));

    const auto next_instruction_rva = static_cast<std::int64_t>(match.rva) + static_cast<std::int64_t>(signature.instruction_length);
    const auto target_rva = next_instruction_rva + displacement;
    if (target_rva < 0)
    {
        throw std::runtime_error("rip-relative target resolved below image base");
    }

    return static_cast<std::uint32_t>(target_rva);
}
