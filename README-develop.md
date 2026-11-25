# Building `libcdio` from git source

Here is software you'll need to build a development version libcdio

- git
- GNU make (or even better "remake")
- autoconf version 2.67 or better
   (which contains programs autoconf and autoreconf)
- automake version 1.11.1 or better
- libtool (for building shared libraries)
- m4 (used by autoconf)
- texinfo (for building documentation)
- help2man (turns help for libcdio standalone programs into manual pages)

This is in addition to the software needed to build starting from a the
source tar. See README-libcdio.md for that additional software.

Older versions of autoconf and automake might work, I've just not
tested that.

The source code lives the [github](https://github.com/libcdio/libcdio.git).
The older GNU Savannah main page is [here](https://savannah.gnu.org/projects/libcdio/).

If you check out the source code, you'll need `git` installed.

Once you have git:

    git clone https://github.com/libcdio/libcdio.git

Change into the libcdio directory that just created and run the "autogen.sh"
shell script:

    cd libcdio
    sh ./autogen.sh

Please see [README-libcdio.md](README-libcdio.md) and follow those instructions starting at step 3.

## Building with Meson

There is also a Meson build, which runs alongside the autotools one and
needs neither autogen.sh nor the autotools. You need Meson 1.3.0 or
later and Ninja:

    meson setup build
    meson compile -C build
    meson test -C build
    meson install -C build

`meson configure build` lists the options. Most mirror a configure
switch, for example `-Dcxx=false` for `--disable-cxx`, `-Dcddb=disabled`
for `--disable-cddb` and `-Dcd-info=false` for `--without-cd-info`.
Tests are grouped into the suites `lib`, `driver`, `example` and
`regression`; run one with `meson test -C build --suite regression`.
`meson compile -C build doxygen` builds the API reference when Doxygen
is installed.

With Microsoft's compiler, run the same commands from a Visual Studio
developer prompt. The libraries are built static, since libcdio marks
no symbols for export from a DLL. To link the C runtime statically as
libcdio.sln does, add `-Db_vscrt=static_from_buildtype`.

When you add a source file, test or configure check, add it to the
`meson.build` files as well as to the Makefile.am files and configure.ac.
