cdio_headers = files(
  'audio.h',
  'bytesex.h',
  'bytesex_asm.h',
  'cdio.h',
  'cd_types.h',
  'cdtext.h',
  'device.h',
  'disc.h',
  'ds.h',
  'dvd.h',
  'ecma_167.h',
  'iso9660.h',
  'logging.h',
  'memory.h',
  'mmc.h',
  'mmc_cmds.h',
  'mmc_hl_cmds.h',
  'mmc_ll_cmds.h',
  'mmc_util.h',
  'posix.h',
  'read.h',
  'rock.h',
  'sector.h',
  'track.h',
  'types.h',
  'udf.h',
  'udf_file.h',
  'udf_time.h',
  'utf8.h',
  'util.h',
  'xa.h',
)
install_headers(cdio_headers, subdir: 'cdio')

version_h = configure_file(input: 'version.h.in',
                           output: 'version.h',
                           configuration: subst,
                           install: true,
                           install_dir: get_option('includedir') / 'cdio')

# cdio_config.h is config.h with every macro prefixed by CDIO_, so that
# programs using libcdio can see how it was configured without the
# names clashing with their own config.h.
cdio_conf = configuration_data()
foreach key : conf.keys()
  cdio_conf.set('CDIO_' + key, conf.get(key))
endforeach
configure_file(output: 'cdio_config.h',
               configuration: cdio_conf,
               macro_name: '__CDIO_CONFIG_H__',
               install: true,
               install_dir: get_option('includedir') / 'cdio')
