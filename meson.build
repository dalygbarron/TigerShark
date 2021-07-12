project('Tiger Shark', 'cpp', default_options: ['c_std=c11', 'cpp_std=c++20'])
core_sources = [
  'src/Gfx.cc',
  'src/Batch2D.cc',
  'src/Shader.cc',
  'src/Texture.cc',
  'src/Atlas.cc',
  'src/IO.cc',
  'src/Rect.cc'
]
test_sources = [
  'src/test/testIO.cc',
  'src/test/testXmlLoad.cc',
  'src/test/test.cc'
]
deps = [
  dependency('gl'),
  dependency('openal'),
  dependency('sfml-window', version : '>=2.5'),
  dependency('sfml-system', version : '>=2.5'),
  dependency('sfml-audio', version : '>=2.5'),
  dependency('sfml-graphics', version : '>=2.5'),
  dependency('glm', version: '>=0.9.9'),
  dependency('spdlog', version: '>=1.8.5'),
  dependency('pugixml', version: '>=1.11')
]
test_deps = [
  dependency('Catch2', version: '>=2.13.6')
]
main = executable(
  'main',
  sources: core_sources + ['src/main.cc'],
  dependencies: deps
)
unit_test = executable(
  'unit_test',
  sources: core_sources + test_sources,
  dependencies: deps + test_deps
)
test('unit', unit_test, workdir: meson.current_source_dir())
test('smoke', main, args: '-b')

