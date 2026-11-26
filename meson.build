# Meson build for libcdio.  This runs alongside the autotools build
# (configure.ac / Makefile.am); keep the two in step when adding sources,
# checks or options.
#
#   meson setup build
#   meson compile -C build
#   meson test -C build
#
# Copyright (C) 2026 Nicos Panayides
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

project('libcdio', 'c',
  # Keep in step with RELEASE_NUM in configure.ac.
  version: '2.4.1dev0',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 1.3.0',
  default_options: [
    'warning_level=1',
    'buildtype=debugoptimized',
    # libtool builds shared and static libraries by default.
    'default_library=both',
  ],
)

fs = import('fs')
pkg = import('pkgconfig')

# Same as LIBCDIO_VERSION_NUM in configure.ac: x*10000 + y*100 + z.
version_parts = meson.project_version().split('.')
libcdio_version_num = (version_parts[0].to_int() * 10000
                       + version_parts[1].to_int() * 100
                       + version_parts[2].split('dev')[0].to_int())

cc = meson.get_compiler('c')
system = host_machine.system()

# Microsoft's compiler (cl, or clang-cl).  config.h comes from the same
# checks as everywhere else; Win32 calls replace iconv, and the
# libraries are static because nothing in libcdio is marked
# __declspec(dllexport).  To link
# the C runtime statically as libcdio.sln does, pass
# -Db_vscrt=static_from_buildtype.
is_msvc = cc.get_argument_syntax() == 'msvc'

enable_cxx = get_option('cxx')
if enable_cxx
  add_languages('cpp', native: false, required: true)
  cxx = meson.get_compiler('cpp')
endif

conf = configuration_data()

conf.set_quoted('PACKAGE', meson.project_name())
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_TARNAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('PACKAGE_STRING',
                meson.project_name() + ' ' + meson.project_version())
conf.set_quoted('PACKAGE_BUGREPORT',
                'https://savannah.gnu.org/bugs/?group=libcdio')
conf.set_quoted('PACKAGE_URL', '')
conf.set_quoted('VERSION', meson.project_version())
conf.set('LIBCDIO_CONFIG_H', 1,
  description: 'Is set when libcdio\'s config.h has been included.')
conf.set('STDC_HEADERS', 1)

# Every source file guards its config.h include with HAVE_CONFIG_H.
project_args = ['-DHAVE_CONFIG_H']

# ---------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------

if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
  project_args += cc.get_supported_arguments(
    '-Wbad-function-cast',
    '-Wcast-align',
    '-Wchar-subscripts',
    '-Wdeclaration-after-statement',
    '-Wdisabled-optimization',
    '-Wendif-labels',
    '-Winline',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wno-sign-compare',
    '-Wpointer-arith',
    '-Wshadow',
    '-Wstrict-prototypes',
    '-Wundef',
    '-Wunused',
    '-Wwrite-strings',
  )
elif is_msvc
  # win32_ioctl.c tests WIN32, which MinGW's gcc predefines and cl does
  # not.  UNICODE stays undefined: the driver passes char strings to
  # CreateFile and GetDriveType.
  project_args += '-DWIN32'
  # C4018 is -Wno-sign-compare's counterpart; C4996 flags the POSIX
  # names and unchecked CRT functions the code uses throughout.
  project_args += cc.get_supported_arguments('/wd4018', '/wd4996')
else
  warning('This package has not been tested very well with non GNU compilers.')
endif

if not is_msvc
  # Meson already puts -D_FILE_OFFSET_BITS=64 on the command line where
  # it applies; lseek64 and fseeko64 also need _LARGEFILE64_SOURCE on
  # Linux.
  project_args += '-D_LARGEFILE64_SOURCE'
endif

# ---------------------------------------------------------------------
# Headers
# ---------------------------------------------------------------------

if not cc.check_header('stdbool.h')
  error('Couldn\'t find or include stdbool.h')
endif

foreach h : [
    # AC_CHECK_INCLUDES_DEFAULT
    'dlfcn.h', 'inttypes.h', 'stdint.h', 'stdio.h', 'stdlib.h',
    'string.h', 'strings.h', 'sys/stat.h', 'sys/types.h', 'unistd.h',
    # configure.ac
    'alloca.h', 'errno.h', 'fcntl.h', 'glob.h', 'limits.h', 'pwd.h',
    'stdarg.h', 'stdbool.h', 'sys/cdio.h', 'sys/param.h', 'sys/time.h',
    'sys/timeb.h', 'sys/utsname.h', 'getopt.h',
  ]
  conf.set('HAVE_' + h.underscorify().to_upper(), cc.check_header(h) ? 1 : false)
endforeach

if system == 'windows' or system == 'cygwin'
  foreach h : ['ntddcdrm.h', 'ddk/ntddcdrm.h', 'ntddscsi.h',
               'ddk/ntddscsi.h', 'ddk/scsi.h']
    conf.set('HAVE_' + h.underscorify().to_upper(),
             cc.check_header(h, prefix: '#include <windows.h>') ? 1 : false)
  endforeach
endif

# ---------------------------------------------------------------------
# Compiler features
# ---------------------------------------------------------------------

conf.set('WORDS_BIGENDIAN', host_machine.endian() == 'big' ? 1 : false)

if cc.compiles('void f(void) { _Pragma("pack(1)") }',
               name: 'ISO C99 _Pragma()')
  conf.set('HAVE_ISOC99_PRAGMA', '')
endif

if cc.compiles('void f(void) { struct { int foo; int bar[]; } doo; (void)doo; }',
               name: 'empty arrays with []')
  conf.set('EMPTY_ARRAY_SIZE', '')
elif cc.compiles('void f(void) { struct { int foo; int bar[0]; } doo; (void)doo; }',
                 name: 'empty arrays with [0]')
  conf.set('EMPTY_ARRAY_SIZE', 0)
else
  error('compiler is unable to create empty arrays')
endif

# AC_C_INLINE
if not cc.compiles('static inline int f(void) { return 0; }', name: 'inline')
  foreach kw : ['__inline__', '__inline']
    if not conf.has('inline') and cc.compiles(
        'static @0@ int f(void) { return 0; }'.format(kw), name: kw)
      conf.set('inline', kw)
    endif
  endforeach
endif

stat_prefix = cc.check_header('sys/stat.h') ? '#include <sys/stat.h>\n' : ''
if cc.links(stat_prefix + 'int main(void) { return S_ISLNK(0); }',
            name: 'S_ISLNK() macro')
  conf.set('HAVE_S_ISLNK', '')
endif
if cc.links(stat_prefix + 'int main(void) { return S_ISSOCK(0); }',
            name: 'S_ISSOCK() macro')
  conf.set('HAVE_S_ISSOCK', '')
endif

if cc.compiles('''#include <sys/time.h>
                  #include <time.h>
                  struct timespec ts;''',
               name: 'struct timespec')
  conf.set('HAVE_STRUCT_TIMESPEC', '')
endif

# ---------------------------------------------------------------------
# Libraries and functions
# ---------------------------------------------------------------------

m_dep = cc.find_library('m', required: false)
extra_deps = [m_dep]

# The Microsoft C runtime defines snprintf, vsnprintf and others inline
# in its headers, so look for them through the headers there.  Other
# compilers keep autoconf's link-only check, which finds functions that
# a header hides without a feature macro.
function_check_prefix = ''
if is_msvc
  function_check_prefix = '''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
'''
endif

foreach f : ['chdir', 'drand48', 'fseeko', 'fseeko64', 'ftruncate',
             'geteuid', 'getgid', 'getuid', 'getpwuid', 'gettimeofday',
             'lseek64', 'lstat', 'memcpy', 'memset', 'mkstemp', 'rand',
             'seteuid', 'setegid', 'snprintf', 'setenv', 'strndup',
             'unsetenv', 'tzset', 'sleep', '_stati64', 'usleep',
             'vsnprintf', 'readlink', 'realpath', 'gmtime_r',
             'localtime_r', 'strtok_r', 'timegm']
  conf.set('HAVE_' + f.to_upper(),
           cc.has_function(f, args: project_args,
                           prefix: function_check_prefix) ? 1 : false)
endforeach

# AC_STRUCT_TIMEZONE
if cc.has_member('struct tm', 'tm_zone', prefix: '#include <time.h>')
  conf.set('HAVE_STRUCT_TM_TM_ZONE', 1)
  conf.set('HAVE_TM_ZONE', 1)
endif

has_tm_gmtoff = cc.has_member('struct tm', 'tm_gmtoff',
                              prefix: '#include <time.h>')
conf.set('HAVE_TM_GMTOFF', has_tm_gmtoff ? 1 : false)

if has_tm_gmtoff
  if cc.compiles('''#include <time.h>
                    int f(void) { return (timezone != 0) + daylight; }''',
                 name: 'daylight and timezone variables')
    conf.set('HAVE_DAYLIGHT', 1)
  endif
  if cc.compiles('''#include <time.h>
                    int f(void) { return (tzname != NULL); }''',
                 name: 'tzname variable')
    conf.set('HAVE_TZNAME', 1)
  endif
endif

# iconv (AM_ICONV).  The Visual Studio build converts with the Win32
# MultiByteToWideChar API instead.
if is_msvc
  iconv_dep = dependency('', required: false)
else
  iconv_dep = dependency('iconv', required: true)
  conf.set('HAVE_ICONV', 1)
  iconv_const_test = '''
#include <stdlib.h>
#include <iconv.h>
extern size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft,
                     char * *outbuf, size_t *outbytesleft);
'''
  if cc.compiles(iconv_const_test, dependencies: iconv_dep,
                 name: 'iconv() takes a non-const input buffer')
    conf.set('ICONV_CONST', '')
  else
    conf.set('ICONV_CONST', 'const')
  endif
endif

# AM_LANGINFO_CODESET
if cc.links('''#include <langinfo.h>
               int main(void) { char *cs = nl_langinfo(CODESET); return !cs; }''',
            name: 'nl_langinfo(CODESET)')
  conf.set('HAVE_LANGINFO_CODESET', 1)
endif

# ---------------------------------------------------------------------
# Operating system CD-ROM driver
# ---------------------------------------------------------------------

cd_drivers = ['cdrdao', 'BIN/CUE', 'NRG']
os_deps = []
libcdio_c_args = []
enable_cdda_player_os = true

if system == 'darwin'
  if cc.check_header('IOKit/IOKitLib.h') and cc.check_header('CoreFoundation/CFBase.h')
    conf.set('HAVE_IOKIT_IOKITLIB_H', 1)
    conf.set('HAVE_COREFOUNDATION_CFBASE_H', 1)
    conf.set('HAVE_DARWIN_CDROM', 1)
    os_deps += dependency('appleframeworks',
                          modules: ['CoreFoundation', 'IOKit'])
    da_dep = dependency('appleframeworks',
                        modules: ['CoreFoundation', 'DiskArbitration'],
                        required: false)
    if da_dep.found()
      conf.set('HAVE_DISKARBITRATION', 1)
      os_deps += da_dep
    endif
    cd_drivers += 'Darwin'
  endif
elif system == 'linux' or system == 'android'
  conf.set('HAVE_LINUX_VERSION_H', cc.check_header('linux/version.h') ? 1 : false)
  conf.set('HAVE_LINUX_MAJOR_H', cc.check_header('linux/major.h') ? 1 : false)
  if cc.check_header('linux/cdrom.h')
    conf.set('HAVE_LINUX_CDROM_H', 1)
    if cc.has_member('struct cdrom_generic_command', 'timeout',
                     prefix: '#include <linux/cdrom.h>')
      conf.set('HAVE_LINUX_CDROM_GENERIC_COMMAND_TIMEOUT', 1)
    endif
    conf.set('HAVE_LINUX_CDROM', 1)
    cd_drivers += 'GNU/Linux'
  endif
elif system == 'sunos'
  conf.set('HAVE_SOLARIS_CDROM', 1)
  cd_drivers += 'Solaris'
elif system == 'cygwin'
  conf.set('CYGWIN', 1)
  conf.set('HAVE_WIN32_CDROM', 1)
  conf.set('NEED_TIMEZONEVAR', 1)
  os_deps += cc.find_library('winmm')
  cd_drivers += 'MinGW'
elif system == 'windows' and is_msvc
  conf.set('HAVE_WINDOWS_H', cc.check_header('windows.h') ? 1 : false)
  conf.set('HAVE_WIN32_CDROM', 1)
  # winmm (mciSendCommand() in the ASPI driver) is part of Windows, so link
  # it without searching: with prefer_static, or static: either way, Meson
  # searches its compiler's library directories, and MSVC reports none.
  os_deps += declare_dependency(link_args: '-lwinmm')
  cd_drivers += 'Win32'
  enable_cdda_player_os = false
elif system == 'windows'
  mingw_w64 = cc.compiles('''#include <stdio.h>
                             #if !defined(__MINGW64_VERSION_MAJOR)
                             # error not mingw-w64 headers
                             #endif''',
                          name: 'MinGW-w64 headers')
  conf.set('MINGW_W64_HEADERS', mingw_w64 ? 1 : false)
  conf.set('HAVE_WINDOWS_H', cc.check_header('windows.h') ? 1 : false)
  conf.set('MINGW32', 1)
  conf.set('HAVE_WIN32_CDROM', 1)
  os_deps += cc.find_library('winmm')
  cd_drivers += 'MinGW'
  # Cross-platform large file support in MinGW needs an override of off_t.
  libcdio_c_args += '-D_FILE_OFFSET_BITS=64'
  if not mingw_w64
    libcdio_c_args += ['-D_OFF_T_DEFINED', '-D_OFF_T_', '-D_off_t=off64_t',
                       '-Doff_t=off64_t']
  endif
  enable_cdda_player_os = false
elif system == 'freebsd' or system == 'dragonfly' or system == 'kfreebsd'
  conf.set('HAVE_FREEBSD_CDROM', 1)
  os_deps += cc.find_library('cam')
  cd_drivers += 'FreeBSD'
elif system == 'netbsd' or system == 'openbsd'
  conf.set('HAVE_NETBSD_CDROM', 1)
  cd_drivers += 'NetBSD'
else
  warning('Don\'t have OS CD-reading support for @0@; will use generic support.'.format(system))
endif

timezone_test = '''
#ifdef NEED_TIMEZONEVAR
#define timezonevar 1
#endif
#include <time.h>
extern long timezone;
int main(void) { long test_timezone = timezone; return (int) test_timezone * 0; }
'''
if system == 'cygwin'
  timezone_test = '#define NEED_TIMEZONEVAR 1\n' + timezone_test
endif
if cc.links(timezone_test, name: 'extern long timezone variable')
  conf.set('HAVE_TIMEZONE_VAR', 1)
endif

# ---------------------------------------------------------------------
# Features
# ---------------------------------------------------------------------

enable_joliet = get_option('joliet')
conf.set('HAVE_JOLIET', enable_joliet ? 1 : false)

enable_rock = get_option('rock')
conf.set('HAVE_ROCK', enable_rock ? 1 : false)

if not get_option('compatibility')
  conf.set('DO_NOT_WANT_COMPATIBILITY', 1)
endif

cddb_dep = dependency('libcddb', version: '>= 1.0.1',
                      required: get_option('cddb'))
# cddb_deps adds the socket libraries some systems need for libcddb.
cddb_deps = [cddb_dep]
if cddb_dep.found()
  conf.set('HAVE_CDDB', 1)
  if not cc.has_function('connect')
    socket_dep = cc.find_library('socket', required: false)
    if socket_dep.found()
      conf.set('HAVE_LIBSOCKET', 1)
      cddb_deps += socket_dep
    endif
  endif
  if not cc.has_function('gethostbyname')
    nsl_dep = cc.find_library('nsl', required: false)
    if nsl_dep.found()
      conf.set('HAVE_LIBNSL', 1)
      cddb_deps += nsl_dep
    endif
  endif
elif get_option('cddb').auto()
  message('new enough libcddb not found; CDDB access disabled. ' +
          'Get libcddb from http://libcddb.sourceforge.net')
endif

vcdinfo_dep = dependency('libvcdinfo', version: '>= 0.7.21',
                         required: get_option('vcd-info'))
conf.set('HAVE_VCDINFO', vcdinfo_dep.found() ? 1 : false)

# cdda-player needs ncurses.h, curses.h or ncurses/ncurses.h, and a
# library that has keypad().
curses_dep = dependency('', required: false)
enable_cdda_player = false
cdda_player_opt = get_option('cdda-player')
if not enable_cdda_player_os
  cdda_player_opt = cdda_player_opt.require(false,
    error_message: 'cdda-player is not supported on this platform')
endif
if cdda_player_opt.allowed()
  have_curses_header = false
  foreach h : ['ncurses.h', 'curses.h', 'ncurses/ncurses.h']
    if not have_curses_header and cc.check_header(h)
      conf.set('HAVE_' + h.underscorify().to_upper(), 1)
      have_curses_header = true
    endif
  endforeach
  if have_curses_header
    curses_dep = dependency('ncurses', required: false)
    if curses_dep.found()
      conf.set('HAVE_KEYPAD', 1)
    else
      curses_dep = cc.find_library('curses', required: false)
      if curses_dep.found() and not cc.has_function('keypad',
                                                    dependencies: curses_dep)
        curses_dep = dependency('', required: false)
      endif
    endif
  endif
  enable_cdda_player = curses_dep.found()
  if cdda_player_opt.enabled() and not enable_cdda_player
    error('cdda-player needs libncurses or libcurses')
  endif
endif

# Versioned symbols need a linker that takes a GNU-style version script.
versioned_libs = get_option('versioned-libs').require(
  cc.get_linker_id() in ['ld.bfd', 'ld.gold', 'ld.lld', 'ld.mold'],
  error_message: 'the linker does not take --version-script')
enable_versioned_libs = versioned_libs.allowed()

# ---------------------------------------------------------------------
# Programs used by the docs and tests
# ---------------------------------------------------------------------

help2man = find_program('help2man', required: get_option('man'))
makeinfo = find_program('makeinfo', required: get_option('info'))
doxygen = find_program('doxygen', required: false)
iso_making_command = find_program('genisoimage', 'mkisofs', required: false)

diff = find_program('diff', required: false)
diff_opts = []
if diff.found()
  foreach opt : ['-w', '--unified']
    if run_command(diff, opt, files('meson.build'), files('meson.build'),
                   check: false).returncode() == 0
      diff_opts += opt
    endif
  endforeach
else
  diff = find_program('cmp', required: false)
endif
grep = find_program('grep', required: false)

# ---------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------

add_project_arguments(project_args, language: 'c')
if enable_cxx
  # The C++ bindings need the same defines, without the C-only warnings.
  add_project_arguments(
    is_msvc ? project_args : ['-DHAVE_CONFIG_H', '-D_LARGEFILE64_SOURCE'],
    language: 'cpp')
endif

if is_msvc
  if cc.links('''#include <malloc.h>
                 int main(void) { char *p = _alloca(4); return p == 0; }''',
              name: '_alloca')
    conf.set('alloca', '_alloca')
  endif
  # The examples and programs call ftruncate; the CRT calls it _chsize.
  # lib/driver/portable.h does the same for the library.
  if (not cc.has_function('ftruncate', args: project_args,
                          prefix: function_check_prefix)
      and cc.has_function('_chsize', prefix: function_check_prefix))
    conf.set('ftruncate', '_chsize')
  endif
endif

configure_file(output: 'config.h', configuration: conf)

# Static only under MSVC; see is_msvc above.
lib_type = is_msvc ? 'static_library' : 'library'

top_inc = include_directories('.')
include_inc = include_directories('include')
driver_inc = include_directories('lib/driver')

# Absolute paths handed to tests and shell scripts, with forward
# slashes: on Windows meson gives backslashes, which C string literals
# and the shell treat as escapes.
abs_srcdir = meson.project_source_root().replace('\\', '/')
abs_builddir = meson.project_build_root().replace('\\', '/')

# The scripts use these commands unquoted, so give the name meson found
# on PATH rather than a full path that may contain spaces.
diff_cmd = diff.found() ? fs.name(diff.full_path()) : 'no'
grep_cmd = grep.found() ? fs.name(grep.full_path()) : 'no'
iso_cmd = iso_making_command.found() ? fs.name(iso_making_command.full_path()) : ''

# Substitutions shared by the .in files that autotools also processes.
subst = configuration_data()
subst.set('PACKAGE', meson.project_name())
subst.set('PACKAGE_NAME', meson.project_name())
subst.set('PACKAGE_VERSION', meson.project_version())
subst.set('VERSION', meson.project_version())
subst.set('LIBCDIO_VERSION_NUM', libcdio_version_num)
subst.set('build', '@0@-@1@'.format(host_machine.cpu(), system))
subst.set('prefix', get_option('prefix'))
subst.set('exec_prefix', '${prefix}')
subst.set('libdir', '${exec_prefix}' / get_option('libdir'))
subst.set('includedir', '${prefix}' / get_option('includedir'))
subst.set('abs_top_srcdir', abs_srcdir)
subst.set('abs_top_builddir', abs_builddir)
subst.set('EXEEXT', system == 'windows' or system == 'cygwin' ? '.exe' : '')
subst.set('SHELL', '/bin/sh')
subst.set('RM', 'rm -f')
subst.set('DIFF', diff_cmd)
subst.set('DIFF_OPTS', ' '.join(diff_opts))
subst.set('GREP', grep_cmd)
subst.set('HAVE_ROCK', enable_rock ? '1' : '')
subst.set('HAVE_JOLIET', enable_joliet ? '1' : '')
subst.set('VCDINFO_LIBS', vcdinfo_dep.found() ? '-lvcdinfo' : '')
subst.set('ISO_MAKING_COMMAND', iso_cmd)

subdir('include')
subdir('lib')
subdir('src')
# The regression tests run some of the examples, so the example
# directory is read whenever tests are on; -Dexamples=false then only
# stops the examples from being built by default.
if get_option('examples') or get_option('tests')
  subdir('example')
endif
subdir('doc')
if get_option('tests')
  subdir('test')
endif

configure_file(input: 'package/libcdio.spec.in',
               output: 'libcdio.spec',
               configuration: subst,
               encoding: 'latin-1')

summary({
  'CD-ROM drivers': ', '.join(cd_drivers),
  'Joliet': enable_joliet,
  'Rock-Ridge': enable_rock,
  'Versioned symbols': enable_versioned_libs,
  'C++ bindings': enable_cxx,
}, section: 'Libraries')
summary({
  'cd-drive': get_option('cd-drive'),
  'cd-info': get_option('cd-info'),
  'cd-read': get_option('cd-read'),
  'cdda-player': enable_cdda_player,
  'iso-info': get_option('iso-info'),
  'iso-read': get_option('iso-read'),
  'examples': get_option('examples'),
  'C++ examples': enable_cxx and get_option('cpp-examples'),
}, section: 'Programs')
summary({
  'CDDB (libcddb)': cddb_dep.found(),
  'VCD info (libvcdinfo)': vcdinfo_dep.found(),
  'man pages (help2man)': help2man.found(),
  'info manual (makeinfo)': makeinfo.found(),
  'doxygen': doxygen.found(),
}, section: 'Optional')
