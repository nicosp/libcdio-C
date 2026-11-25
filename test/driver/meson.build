driver_test_c_args = ['-DDATA_DIR="@0@"'.format(test_data_dir)]

# name: extra sources
driver_tests = {
  'abs_path': [],
  'bincue': ['helper.c'],
  'cdda': ['helper.c'],
  'cdrdao': ['helper.c'],
  'cdtext': [],
  'freebsd': [],
  'gnu_linux': ['helper.c'],
  'logger': [],
  'mmc_read': [],
  'mmc_write': [],
  'nrg': ['helper.c'],
  'osx': [],
  'realpath': [],
  'solaris': [],
  'track': [],
  'win32': [],
}

foreach name, extra : driver_tests
  exe = executable('driver-' + name, name + '.c', extra,
    c_args: driver_test_c_args,
    include_directories: test_inc,
    dependencies: libcdio_dep,
  )
  test(name, exe,
    suite: 'driver',
    workdir: meson.current_build_dir(),
  )
endforeach
