/*
  Copyright (C) 2026 Nicos Panayides

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file export.h
 *
 *  \brief Marks which functions and variables libcdio, libiso9660 and
 *  libudf export from a shared library.
 *
 *  Each public declaration carries CDIO_PUBLIC, ISO9660_PUBLIC or
 *  UDF_PUBLIC, for the library that defines it.  While that library is
 *  being built the macro exports the symbol: __declspec(dllexport) on
 *  Windows, default visibility elsewhere (the libraries are compiled
 *  with -fvisibility=hidden, so nothing unmarked is exported).  In
 *  programs that use the library, MSVC needs __declspec(dllimport) to
 *  use a variable from a DLL, so the macro adds it there.  MinGW and
 *  Cygwin import functions and variables from a DLL without it, so
 *  programs built with them link the static or the shared library
 *  alike.
 *
 *  CDIO_PRIVATE marks functions that are internal to one library.  It
 *  hides them from the dynamic symbol table where the compiler supports
 *  that, and does nothing on Windows, where only marked symbols are
 *  exported anyway.
 *
 *  With MSVC, define CDIO_STATIC before including any libcdio header
 *  when linking the static libraries.  It makes no difference with
 *  other compilers.
 */

#ifndef CDIO_EXPORT_H_
#define CDIO_EXPORT_H_

#if defined(_WIN32) || defined(__CYGWIN__)
#  define CDIO_EXPORT __declspec(dllexport)
#  if defined(_MSC_VER)
#    define CDIO_IMPORT __declspec(dllimport)
#  else
     /* The MinGW and Cygwin linkers import from a DLL by themselves. */
#    define CDIO_IMPORT
#  endif
#  define CDIO_PRIVATE
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define CDIO_EXPORT __attribute__((visibility("default")))
#  define CDIO_IMPORT __attribute__((visibility("default")))
#  define CDIO_PRIVATE __attribute__((visibility("hidden")))
#else
#  define CDIO_EXPORT
#  define CDIO_IMPORT
#  define CDIO_PRIVATE
#endif

#if defined(CDIO_STATIC)
#  define CDIO_PUBLIC
#  define ISO9660_PUBLIC
#  define UDF_PUBLIC
#else
#  if defined(CDIO_BUILDING_LIBCDIO)
#    define CDIO_PUBLIC CDIO_EXPORT
#  else
#    define CDIO_PUBLIC CDIO_IMPORT
#  endif
#  if defined(CDIO_BUILDING_LIBISO9660)
#    define ISO9660_PUBLIC CDIO_EXPORT
#  else
#    define ISO9660_PUBLIC CDIO_IMPORT
#  endif
#  if defined(CDIO_BUILDING_LIBUDF)
#    define UDF_PUBLIC CDIO_EXPORT
#  else
#    define UDF_PUBLIC CDIO_IMPORT
#  endif
#endif

#endif /* CDIO_EXPORT_H_ */
