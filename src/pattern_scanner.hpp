#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

enum class SignatureKind
{
    DirectRva,
    RipRelative32
};

struct SignatureDefinition
{
    std::string_view name;
    std::string_view pattern;
    SignatureKind kind;
    std::uint32_t displacement_offset;
    std::uint32_t instruction_length;
    bool allow_multiple_matches_with_same_target;
};

struct ResolvedOffset
{
    std::string name;
    std::uint32_t rva;
    std::size_t match_count;
};

class PatternScanner
{
public:
    explicit PatternScanner(const std::filesystem::path& image_path);

    std::optional<ResolvedOffset> resolve(const SignatureDefinition& signature) const;
    const std::string& last_error() const noexcept;

private:
    struct SectionView
    {
        std::string name;
        std::uint32_t rva;
        std::vector<std::uint8_t> bytes;
    };

    struct CompiledPattern
    {
        std::vector<int> bytes;
        std::size_t anchor_offset;
        std::vector<std::uint8_t> anchor;
    };

    struct MatchLocation
    {
        std::size_t section_index;
        std::size_t section_offset;
        std::uint32_t rva;
    };

    CompiledPattern compile_pattern(std::string_view pattern) const;
    std::vector<MatchLocation> find_matches(const CompiledPattern& pattern) const;
    std::uint32_t read_rip_relative_target(const MatchLocation& match, const SignatureDefinition& signature) const;

    std::vector<std::uint8_t> image_;
    std::vector<SectionView> executable_sections_;
    mutable std::string last_error_;
};
