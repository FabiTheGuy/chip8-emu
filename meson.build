project(
  'chip8', 
  'c',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'c_std=c11'
  ]
)

sources = files('src/main.c')
headers = include_directories('include')

exe = executable(
  'chip8', 
  sources: sources,
  include_directories: headers,
  install : true
)

test('basic', exe)
