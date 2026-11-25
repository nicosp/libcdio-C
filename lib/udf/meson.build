# Makefile.am gives libudf no -version-info, so libtool names it
# libudf.so.0.0.0.
libudf = build_target('udf',
  'udf.c', 'udf_file.c', 'udf_fs.c', 'udf_time.c', 'filemode.c',
  target_type: lib_type,
  version: '0.0.0',
  soversion: 0,
  include_directories: top_inc,
  c_args: lib_visibility_args + ['-DCDIO_BUILDING_LIBUDF'],
  dependencies: libcdio_dep,
  install: true,
)

libudf_dep = declare_dependency(
  link_with: libudf,
  include_directories: include_directories('.'),
  dependencies: libcdio_dep,
)
meson.override_dependency('libudf', libudf_dep)

pkg.generate(libudf,
  name: 'libudf',
  description: 'UDF library of libcdio',
  filebase: 'libudf',
  extra_cflags: cdio_static_args,
  requires: 'libcdio',
)
