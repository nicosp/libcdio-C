# name: dependencies
example_programs = {
  'audio': [libcdio_dep],
  'cdchange': [libcdio_dep],
  'cdio-eject': [libcdio_dep],
  'cdtext': [libcdio_dep],
  'cdtext-raw': [libcdio_dep],
  'device': [libcdio_dep],
  'discid': [libcdio_dep],
  'drives': [libcdio_dep],
  'eject': [libcdio_dep],
  'extract': [libiso9660_dep, libudf_dep],
  'isofile': [libiso9660_dep],
  'isorr': [libiso9660_dep],
  'isofile2': [libiso9660_dep],
  'isofuzzy': [libiso9660_dep],
  'isolist': [libiso9660_dep],
  'isolsn': [libiso9660_dep],
  'logging': [libcdio_dep],
  'mmc1': [libcdio_dep],
  'mmc2': [libcdio_dep],
  'mmc2a': [libcdio_dep],
  'mmc3': [libcdio_dep],
  'sample3': [libcdio_dep],
  'sample4': [libcdio_dep],
  'tracks': [libcdio_dep],
  'udf1': [libudf_dep],
  'udf2': [libudf_dep],
  'udffile': [libudf_dep],
}

# The examples make check runs, with any arguments they need.  The
# examples default to images found by a path relative to an in-tree
# build, so the tests name the image explicitly.
test_data_dir = meson.project_source_root() / 'test' / 'data'
example_tests = {
  'device': [],
  'drives': [],
  'isofile2': [test_data_dir / 'isofs-m1.cue'],
  'mmc1': [],
  'mmc2': [],
  'mmc2a': [],
  'mmc3': [],
  'sample4': [],
}

examples = {}
example_targets = []
foreach name, deps : example_programs
  exe = executable(name, name + '.c',
    include_directories: top_inc,
    dependencies: deps,
    build_by_default: get_option('examples'),
  )
  examples += {name: exe}
  example_targets += exe
  if get_option('examples') and get_option('tests') and name in example_tests
    test(name, exe, suite: 'example',
         args: example_tests[name],
         workdir: meson.current_build_dir())
  endif
endforeach

if enable_cxx and get_option('examples') and get_option('cpp-examples')
  subdir('C++')
endif
