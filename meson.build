project('chip8-emu', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20']
)


sources = files('src/main.cpp')

chip8_exec = executable(
        'chip8_emu',
        sources: sources,
        install : true
)


test('chip8-exec', chip8_exec)