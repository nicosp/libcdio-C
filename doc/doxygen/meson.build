# API reference: meson compile -C build doxygen
doxygen_subst = configuration_data()
doxygen_subst.merge_from(subst)
doxygen_subst.set('abs_builddir', meson.current_build_dir())
doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: doxygen_subst)
if doxygen.found()
  run_target('doxygen', command: [doxygen, doxyfile])
endif
