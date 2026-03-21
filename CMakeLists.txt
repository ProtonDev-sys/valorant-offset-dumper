cmake_minimum_required(VERSION 3.20)

project(valorant_offset_dumper LANGUAGES CXX)

add_executable(valorant_offset_dumper
    src/main.cpp
    src/pattern_scanner.cpp
)

set_target_properties(valorant_offset_dumper PROPERTIES
    OUTPUT_NAME "valorant-offset-dumper"
)

target_include_directories(valorant_offset_dumper PRIVATE src)
target_compile_features(valorant_offset_dumper PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(valorant_offset_dumper PRIVATE /W4 /EHsc /permissive-)
else()
    target_compile_options(valorant_offset_dumper PRIVATE -Wall -Wextra -Wpedantic)
endif()
