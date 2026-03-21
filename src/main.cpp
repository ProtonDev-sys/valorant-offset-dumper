#include "pattern_scanner.hpp"
#include "signatures.hpp"

#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace
{
    struct CompareSummary
    {
        std::size_t shared_keys = 0;
        std::vector<std::string> mismatches;
    };

    std::map<std::string, std::uint32_t> parse_reference_file(const std::filesystem::path& path)
    {
        std::ifstream input(path);
        if (!input)
        {
            throw std::runtime_error("failed to open reference file: " + path.string());
        }

        const std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        const std::regex expression(R"(([A-Za-z_][A-Za-z0-9_]*):\s*0x([0-9A-Fa-f]+))");

        std::map<std::string, std::uint32_t> parsed;
        for (std::sregex_iterator it(text.begin(), text.end(), expression), end; it != end; ++it)
        {
            parsed[(*it)[1].str()] = static_cast<std::uint32_t>(std::stoul((*it)[2].str(), nullptr, 16));
        }

        return parsed;
    }

    CompareSummary compare_offsets(
        const std::map<std::string, std::uint32_t>& generated,
        const std::map<std::string, std::uint32_t>& reference)
    {
        CompareSummary summary{};

        for (const auto& [name, value] : generated)
        {
            const auto found = reference.find(name);
            if (found == reference.end())
            {
                continue;
            }

            ++summary.shared_keys;
            if (found->second == value)
            {
                continue;
            }

            std::ostringstream line;
            line << name << ": reference=0x" << std::uppercase << std::hex << found->second
                 << " generated=0x" << value;
            summary.mismatches.push_back(line.str());
        }

        return summary;
    }
}

int main(int argc, char** argv)
{
    try
    {
        std::filesystem::path image_path = "VALORANT.exe";
        std::vector<std::filesystem::path> reference_files;

        for (int index = 1; index < argc; ++index)
        {
            const std::string argument = argv[index];
            if (argument == "--compare")
            {
                for (int compare_index = index + 1; compare_index < argc; ++compare_index)
                {
                    reference_files.emplace_back(argv[compare_index]);
                }
                break;
            }

            image_path = argument;
        }

        PatternScanner scanner(image_path);

        bool had_errors = false;
        std::map<std::string, std::uint32_t> generated_offsets;

        for (const auto& signature : kSignatures)
        {
            const auto resolved = scanner.resolve(signature);
            if (!resolved)
            {
                had_errors = true;
                std::cout << "[FAILED]  -> " << signature.name << ": " << scanner.last_error() << '\n';
                continue;
            }

            generated_offsets.emplace(resolved->name, resolved->rva);
            std::cout << "[SUCCESS] -> " << resolved->name << ": 0x"
                      << std::uppercase << std::hex << resolved->rva << std::dec;

            if (resolved->match_count > 1)
            {
                std::cout << "    (resolved from " << resolved->match_count << " equivalent matches)";
            }

            std::cout << '\n';
        }

        for (const auto& reference_path : reference_files)
        {
            const auto reference_offsets = parse_reference_file(reference_path);
            const auto summary = compare_offsets(generated_offsets, reference_offsets);

            std::cout << "[COMPARE] -> " << reference_path.string()
                      << " (shared keys: " << summary.shared_keys << ")\n";

            if (summary.mismatches.empty())
            {
                std::cout << "[COMPARE] -> no mismatches\n";
                continue;
            }

            had_errors = true;
            for (const auto& mismatch : summary.mismatches)
            {
                std::cout << "[MISMATCH] -> " << mismatch << '\n';
            }
        }

        return had_errors ? 1 : 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "[ERROR] " << error.what() << '\n';
        return 1;
    }
}
