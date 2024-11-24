project('chip8-emu', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20']
)


sources = files(
        'src/types.hpp'
)

chip8_exec = executable(
        'chip8_emu',
        sources: sources + files('src/main.cpp'),
        install : true
)

chip8_lib = library(
        'chip8_emu_test_lib',
        sources: sources,
)


gtest_dep = subproject('gtest').get_variable('gtest_dep')
test_sources = files(
        'test/main.cpp'
)

chip8_test_exec = executable(
        'chip8_emu_test',
        sources: test_sources,
        link_with: chip8_lib,
        dependencies: gtest_dep,
)


test('chip8-exec', chip8_exec)
test('chip8-test-exec', chip8_test_exec)
