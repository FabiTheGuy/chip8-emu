project('chip8-emu', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

dependencies = [ 
  dependency('raylib')
]

sources = [
  'src/main.c',
  'src/chip8.h',
  'src/chip8.c',
  'src/utils.h',
  'src/utils.c',
  'src/raygui.h',
]


executable('chip8-emu', sources: sources, dependencies: dependencies)
