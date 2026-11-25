# Makefile.am gives libudf no -version-info, so libtool names it
# libudf.so.0.0.0.
libudf_sources = files('udf.c', 'udf_file.c', 'udf_fs.c', 'udf_time.c',
                       'filemode.c')
libudf_common = {
  'include_directories': top_inc,
  'dependencies': libcdio_dep,
  'install': true,
}
libudf_shared_kwargs = {'version': '0.0.0', 'soversion': 0}
libudf_build_args = lib_visibility_args + ['-DCDIO_BUILDING_LIBUDF']

if split_static
  libudf = shared_library('udf', libudf_sources,
    c_args: libudf_build_args,
    kwargs: libudf_common + libudf_shared_kwargs)
  static_library('udf', libudf_sources,
    c_args: ['-DCDIO_STATIC'],
    kwargs: libudf_common)
else
  libudf = build_target('udf', libudf_sources,
    target_type: lib_type,
    c_args: windows_static_only ? ['-DCDIO_STATIC'] : libudf_build_args,
    kwargs: libudf_common + libudf_shared_kwargs)
endif

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
