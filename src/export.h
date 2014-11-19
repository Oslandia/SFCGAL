/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_EXPORT_H_
#define _SFCGAL_EXPORT_H_

//----- DLL MANAGEMENT ------------------

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#define SFCGAL_API_IMPORT __declspec(dllimport)
#define SFCGAL_API_EXPORT __declspec(dllexport)

#define SFCGAL_API_TEMPLATE_EXPORT extern
#define SFCGAL_API_TEMPLATE_IMPORT
#else
#define SFCGAL_API_IMPORT
#define SFCGAL_API_EXPORT
#define SFCGAL_API_TEMPLATE_EXPORT
#define SFCGAL_API_TEMPLATE_IMPORT
#endif


//TODO : split cxx api and capi?

#ifdef SFCGAL_USE_STATIC_LIBS
// static libraries
#  define SFCGAL_API
#  define SFCGAL_API_TEMPLATE
#else
// shared libraries
#  ifdef SFCGAL_BUILD_SHARED
#     define SFCGAL_API SFCGAL_API_EXPORT
#     define SFCGAL_API_TEMPLATE SFCGAL_API_TEMPLATE_EXPORT
#  else
#     define SFCGAL_API SFCGAL_API_IMPORT
#     define SFCGAL_API_TEMPLATE SFCGAL_API_TEMPLATE_IMPORT
#  endif
#endif

#endif // _SFCGAL_EXPORT_H_
