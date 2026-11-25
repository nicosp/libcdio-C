# Texinfo manual.  version.texi stands in for the one automake's
# mdate-sh writes: the version, and the date libcdio.texi last changed.
if makeinfo.found()
  python = import('python').find_installation()
  texi_mtime = run_command(python, '-c', '''
import os, sys, time
epoch = os.environ.get("SOURCE_DATE_EPOCH")
t = time.gmtime(int(epoch) if epoch else os.stat(sys.argv[1]).st_mtime)
print("%d %s" % (t.tm_mday, time.strftime("%B %Y", t)))
''', files('libcdio.texi'), check: true).stdout().strip()

  version_texi = configure_file(
    output: 'version.texi',
    configuration: {
      'VERSION': meson.project_version(),
      'UPDATED': texi_mtime,
      'UPDATED_MONTH': texi_mtime.split(' ')[1] + ' ' + texi_mtime.split(' ')[2],
    },
    input: 'version.texi.in',
  )

  custom_target('libcdio.info',
    input: 'libcdio.texi',
    output: 'libcdio.info',
    depend_files: files('fdl.texi', 'glossary.texi') + [version_texi],
    command: [makeinfo, '--no-split',
              '-I', meson.current_build_dir(),
              '-I', meson.current_source_dir(),
              '-o', '@OUTPUT@', '@INPUT@'],
    install: true,
    install_dir: get_option('infodir'),
  )
endif

subdir('doxygen')
