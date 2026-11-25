subdir('OO')

# name: [dependencies, test arguments]
cpp_examples = {
  'device': [libcdio_dep, []],
  'eject': [libcdio_dep, []],
  'isofile': [libiso9660_dep, [test_data_dir / 'copying.iso']],
  'isofile2': [libiso9660_dep, [test_data_dir / 'copying.iso']],
  'isolist': [libiso9660_dep, [test_data_dir / 'copying.iso']],
  'mmc1': [libcdio_dep, []],
  'mmc2': [libcdio_dep, []],
}

foreach name, p : cpp_examples
  exe = executable('cpp-' + name, name + '.cpp',
    include_directories: top_inc,
    dependencies: p[0],
  )
  if get_option('tests')
    test(name, exe, suite: 'example-cpp',
         args: p[1],
         workdir: meson.current_build_dir())
  endif
endforeach
