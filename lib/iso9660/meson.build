libiso9660_current = 13
libiso9660_revision = 0
libiso9660_age = 0
libiso9660_major = libiso9660_current - libiso9660_age

libiso9660_sources = files('iso9660.c', 'iso9660_fs.c', 'xa.c')
if enable_rock
  libiso9660_sources += files('rock.c')
endif

libiso9660_link_args = []
libiso9660_link_depends = []
if enable_versioned_libs
  libiso9660_ver = configure_file(
    input: version_script_template,
    output: 'libiso9660.ver',
    configuration: {
      'NAME': 'ISO9660_@0@'.format(libiso9660_major),
      'SYMBOLS': '    ' + ';\n    '.join(fs.read('libiso9660.sym').strip().split('\n')) + ';',
    })
  libiso9660_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libiso9660.ver'
  libiso9660_link_depends += libiso9660_ver
endif

libiso9660 = build_target('iso9660',
  libiso9660_sources,
  target_type: lib_type,
  version: '@0@.@1@.@2@'.format(libiso9660_major, libiso9660_age,
                                libiso9660_revision),
  soversion: libiso9660_major,
  include_directories: top_inc,
  link_args: libiso9660_link_args,
  link_depends: libiso9660_link_depends,
  dependencies: libcdio_dep,
  install: true,
)

libiso9660_dep = declare_dependency(
  link_with: libiso9660,
  include_directories: include_directories('.'),
  dependencies: libcdio_dep,
)
meson.override_dependency('libiso9660', libiso9660_dep)

pkg.generate(libiso9660,
  name: 'libiso9660',
  description: 'ISO-9660 library of libcdio',
  filebase: 'libiso9660',
  requires: 'libcdio',
)
