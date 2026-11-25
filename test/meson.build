test_srcdir = abs_srcdir / 'test'
test_data_dir = test_srcdir / 'data'
test_c_args = ['-DDATA_DIR="@0@"'.format(test_data_dir)]
test_inc = [top_inc, include_directories('../lib/iso9660')]

# Shell helpers and scripts that need configure-time substitutions.
configure_file(input: 'check_common_fn.in', output: 'check_common_fn',
               configuration: subst)
generated_scripts = {}
foreach s : ['check_cue.sh', 'check_iso.sh', 'check_nrg.sh', 'check_udf.sh',
             'check_iso_read.sh', 'check_iso_long_path.sh']
  generated_scripts += {s: configure_file(input: s + '.in', output: s,
                                          configuration: subst)}
endforeach

# testiso9660 opens ./data/xa.iso and ./data/isofs-m1.bin from the
# directory it runs in; test/data/meson.build copies them there.
subdir('data')

# name: [dependencies, extra sources, extra include directories]
test_programs = {
  'check_sizeof': [libiso9660_dep, [], []],
  'testassert': [libcdio_dep, [], []],
  'testgetdevices': [libcdio_dep, [], []],
  'testischar': [libiso9660_dep, [], []],
  'testisocd': [libiso9660_dep, [], []],
  'testisocd2': [libiso9660_dep, [], []],
  'testisocd_joliet': [libiso9660_dep, [], []],
  'testiso9660': [libiso9660_dep, [], []],
  'testisorr': [libiso9660_dep, [], []],
  'testisorr_ce': [libcdio_dep, files('../lib/iso9660/rock.c'), []],
  'test_lib_driver_util': [libcdio_dep, [], []],
  'testudf': [libudf_dep, [], []],
  'testudf_ad': [libudf_dep, [], [include_directories('../lib/udf')]],
  'testudf_efe': [libudf_dep, [], []],
  'testpregap': [libcdio_dep, [], []],
}

foreach name, p : test_programs
  exe = executable(name, name + '.c', p[1],
    c_args: test_c_args,
    include_directories: test_inc + p[2],
    dependencies: p[0],
  )
  test(name, exe,
    suite: 'lib',
    workdir: meson.current_build_dir(),
    # testassert checks that cdio_assert aborts.
    should_fail: name == 'testassert',
  )
endforeach

subdir('driver')

# Regression scripts.  They find the programs through top_builddir and
# their input through srcdir, as they do under make check.
sh = find_program('sh', required: false)
script_env = environment({
  'srcdir': test_srcdir,
  'top_srcdir': abs_srcdir,
  'abs_top_srcdir': abs_srcdir,
  'top_builddir': abs_builddir,
  'abs_top_builddir': abs_builddir,
})

regression_scripts = []
if sh.found()
  regression_scripts = [
    'check_nrg.sh', 'check_cue.sh', 'check_cd_read.sh', 'check_udf.sh',
    'check_iso.sh', 'check_iso_long_path.sh', 'check_bad_iso.sh',
    'check_multiextent.sh', 'check_fuzzyiso.sh', 'check_opts.sh',
    'check_deep_directory.sh', 'check_iso_read.sh', 'check_cdtext.sh',
  ]
endif
foreach s : regression_scripts
  script = s in generated_scripts ? generated_scripts[s] : files(s)
  test(s.split('.')[0], sh,
    args: script,
    suite: 'regression',
    env: script_env,
    workdir: meson.current_build_dir(),
    depends: src_program_targets + example_targets,
    timeout: 300,
  )
endforeach
