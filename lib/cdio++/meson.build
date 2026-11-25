libcdiopp_current = 2
libcdiopp_revision = 0
libcdiopp_age = 0
libcdiopp_major = libcdiopp_current - libcdiopp_age

libcdiopp = build_target('cdio++',
  'cdio.cpp', 'devices.cpp',
  target_type: lib_type,
  version: '@0@.@1@.@2@'.format(libcdiopp_major, libcdiopp_age,
                                libcdiopp_revision),
  soversion: libcdiopp_major,
  include_directories: top_inc,
  dependencies: libcdio_dep,
  install: true,
)

libcdiopp_dep = declare_dependency(
  link_with: libcdiopp,
  dependencies: libcdio_dep,
)
meson.override_dependency('libcdio++', libcdiopp_dep)

libiso9660pp_current = 1
libiso9660pp_revision = 0
libiso9660pp_age = 0
libiso9660pp_major = libiso9660pp_current - libiso9660pp_age

libiso9660pp = build_target('iso9660++',
  'iso9660.cpp',
  target_type: lib_type,
  version: '@0@.@1@.@2@'.format(libiso9660pp_major, libiso9660pp_age,
                                libiso9660pp_revision),
  soversion: libiso9660pp_major,
  include_directories: top_inc,
  dependencies: libiso9660_dep,
  install: true,
)

libiso9660pp_dep = declare_dependency(
  link_with: libiso9660pp,
  dependencies: [libiso9660_dep, libcdiopp_dep],
)
meson.override_dependency('libiso9660++', libiso9660pp_dep)

pkg.generate(libcdiopp,
  name: 'libcdio++',
  description: 'C++ OO Portable CD-ROM I/O library',
  filebase: 'libcdio++',
  requires: 'libcdio',
)
pkg.generate(libiso9660pp,
  name: 'libiso9660++',
  description: 'C++ OO ISO-9660 library of libcdio',
  filebase: 'libiso9660++',
  libraries: [libcdiopp],
  requires: ['libiso9660', 'libcdio++'],
)
