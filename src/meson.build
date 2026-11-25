getopt_sources = files('getopt.c', 'getopt1.c')
util_sources = files('util.c')
cddb_sources = files('cddb.c')

src_inc = [top_inc]

# name: [option, sources, dependencies, man page]
programs = {
  'cd-drive': [get_option('cd-drive'), util_sources,
               [libiso9660_dep], true],
  'cd-info': [get_option('cd-info'), util_sources + cddb_sources,
              [libiso9660_dep, cddb_deps, vcdinfo_dep], true],
  'cd-read': [get_option('cd-read'), util_sources,
              [libiso9660_dep], true],
  'iso-info': [get_option('iso-info'), util_sources,
               [libudf_dep, libiso9660_dep], true],
  'iso-read': [get_option('iso-read'), util_sources,
               [libudf_dep, libiso9660_dep], true],
  'cdda-player': [enable_cdda_player, cddb_sources,
                  [libcdio_dep, cddb_deps, curses_dep], false],
  'mmc-tool': [true, util_sources,
               [libiso9660_dep], false],
}

src_programs = {}
src_program_targets = []
can_run = meson.can_run_host_binaries()
foreach name, p : programs
  if not p[0]
    continue
  endif
  exe = executable(name, name + '.c', p[1], getopt_sources,
    include_directories: src_inc,
    dependencies: p[2],
    install: true,
  )
  src_programs += {name: exe}
  src_program_targets += exe

  if p[3] and help2man.found() and can_run
    custom_target(name + '.1',
      output: name + '.1',
      input: name + '.help2man',
      command: [help2man, '--opt-include=@INPUT@', '--no-info',
                '--output=@OUTPUT@', exe],
      install: true,
      install_dir: get_option('mandir') / 'man1',
    )
  endif
endforeach
