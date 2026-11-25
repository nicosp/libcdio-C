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
    configuration: {'NAME': 'ISO9660_@0@'.format(libiso9660_major)})
  libiso9660_link_args += '-Wl,--version-script,' + meson.current_build_dir() / 'libiso9660.ver'
  libiso9660_link_depends += libiso9660_ver
endif

libiso9660_common = {
  'include_directories': top_inc,
  'dependencies': libcdio_dep,
  'install': true,
}
libiso9660_shared_kwargs = {
  'version': '@0@.@1@.@2@'.format(libiso9660_major, libiso9660_age,
                                  libiso9660_revision),
  'soversion': libiso9660_major,
  'link_args': libiso9660_link_args,
  'link_depends': libiso9660_link_depends,
}
libiso9660_build_args = lib_visibility_args + ['-DCDIO_BUILDING_LIBISO9660']

if split_static
  libiso9660 = shared_library('iso9660', libiso9660_sources,
    c_args: libiso9660_build_args,
    kwargs: libiso9660_common + libiso9660_shared_kwargs)
  static_library('iso9660', libiso9660_sources,
    c_args: ['-DCDIO_STATIC'],
    kwargs: libiso9660_common)
else
  libiso9660 = build_target('iso9660', libiso9660_sources,
    target_type: lib_type,
    c_args: windows_static_only ? ['-DCDIO_STATIC'] : libiso9660_build_args,
    kwargs: libiso9660_common + libiso9660_shared_kwargs)
endif

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
  extra_cflags: cdio_static_args,
  requires: 'libcdio',
)
