# name: [dependencies, test arguments]
cpp_oo_examples = {
  'cdtext': [libcdiopp_dep, []],
  'device': [libcdiopp_dep, []],
  'drives': [libcdiopp_dep, []],
  'eject': [libcdiopp_dep, []],
  'isofile': [libiso9660pp_dep, [test_data_dir / 'copying.iso']],
  'isofile2': [libiso9660pp_dep, [test_data_dir / 'isofs-m1.cue']],
  'isolist': [libiso9660pp_dep, [test_data_dir / 'copying.iso']],
  'iso4': [libiso9660pp_dep, [test_data_dir / 'isofs-m1.cue']],
  'mmc1': [libcdiopp_dep, []],
  'mmc2': [libcdiopp_dep, []],
  'tracks': [libcdiopp_dep, []],
}

foreach name, p : cpp_oo_examples
  exe = executable('cpp-oo-' + name, name + '.cpp',
    include_directories: top_inc,
    dependencies: p[0],
  )
  if get_option('tests')
    test(name, exe, suite: 'example-cpp-oo',
         args: p[1],
         workdir: meson.current_build_dir())
  endif
endforeach
