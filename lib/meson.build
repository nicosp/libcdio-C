# Tags every exported symbol with the library's version node (CDIO_21,
# ISO9660_13), as before.  Which symbols are exported is decided by the
# CDIO_PUBLIC / ISO9660_PUBLIC / UDF_PUBLIC marks in the headers, with
# the libraries compiled -fvisibility=hidden.
version_script_template = files('version-script.in')

subdir('driver')
subdir('iso9660')
subdir('udf')
if enable_cxx
  subdir('cdio++')
endif
