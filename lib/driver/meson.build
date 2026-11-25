# libtool -version-info current:revision:age
libcdio_current = 21
libcdio_revision = 0
libcdio_age = 0
libcdio_major = libcdio_current - libcdio_age

libcdio_sources = files(
  '_cdio_generic.c',
  '_cdio_stdio.c',
  '_cdio_stream.c',
  'abs_path.c',
  'aix.c',
  'audio.c',
  'cd_types.c',
  'cdio.c',
  'cdtext.c',
  'device.c',
  'disc.c',
  'ds.c',
  'FreeBSD/freebsd.c',
  'FreeBSD/freebsd_cam.c',
  'FreeBSD/freebsd_ioctl.c',
  'gnu_linux.c',
  'image/bincue.c',
  'image/cdrdao.c',
  'image_common.c',
  'image/nrg.c',
  'logging.c',
  'memory.c',
  'mmc/mmc.c',
  'mmc/mmc_hl_cmds.c',
  'mmc/mmc_ll_cmds.c',
  'mmc/mmc_util.c',
  'MSWindows/aspi32.c',
  'MSWindows/win32_ioctl.c',
  'MSWindows/win32.c',
  'netbsd.c',
  'osx.c',
  'read.c',
  'realpath.c',
  'sector.c',
  'solaris.c',
  'track.c',
  'utf8.c',
  'util.c',
)

libcdio_link_args = []
libcdio_link_depends = []
if enable_versioned_libs
  libcdio_ver = configure_file(
    input: version_script_template,
    output: 'libcdio.ver',
    configuration: {
      'NAME': 'CDIO_@0@'.format(libcdio_major),
      'SYMBOLS': '    ' + ';\n    '.join(fs.read('libcdio.sym').strip().split('\n')) + ';',
    })
  libcdio_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libcdio.ver'
  libcdio_link_depends += libcdio_ver
endif

libcdio_deps = [iconv_dep, os_deps, extra_deps]

libcdio = build_target('cdio',
  libcdio_sources, version_h,
  target_type: lib_type,
  version: '@0@.@1@.@2@'.format(libcdio_major, libcdio_age, libcdio_revision),
  soversion: libcdio_major,
  include_directories: [top_inc, include_inc, driver_inc],
  c_args: libcdio_c_args,
  link_args: libcdio_link_args,
  link_depends: libcdio_link_depends,
  dependencies: libcdio_deps,
  install: true,
)

libcdio_dep = declare_dependency(
  link_with: libcdio,
  include_directories: [include_inc, driver_inc],
  compile_args: libcdio_c_args,
  dependencies: [iconv_dep, extra_deps],
  sources: version_h,
)
meson.override_dependency('libcdio', libcdio_dep)

pkg.generate(libcdio,
  name: 'libcdio',
  description: 'Portable CD-ROM I/O library',
  filebase: 'libcdio',
)
