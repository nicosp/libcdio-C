# Writes a GNU ld version script from a .sym file (one symbol per line),
# the way the libcdio.la.ver and libiso9660.la.ver make rules do.
version_script_template = files('version-script.in')

subdir('driver')
subdir('iso9660')
subdir('udf')
if enable_cxx
  subdir('cdio++')
endif
