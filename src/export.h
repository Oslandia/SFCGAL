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
#  define SFCGAL_API_IMPORT __declspec(dllimport)
#  define SFCGAL_API_EXPORT __declspec(dllexport)
#  define SFCGAL_NO_EXPORT
#else
#  define SFCGAL_API_IMPORT
#  define SFCGAL_API_EXPORT __attribute__ ((visibility ("default")))
#  define SFCGAL_NO_EXPORT __attribute__ ((visibility ("hidden")))
#endif


#ifdef SFCGAL_EXPORT_CXX_API
#  define SFCGAL_CXX_API SFCGAL_API_EXPORT
#else
#  define SFCGAL_CXX_API SFCGAL_NO_EXPORT
#endif

#ifdef SFCGAL_EXPORT_C_API
#  define SFCGAL_C_API SFCGAL_API_EXPORT
#else
#  define SFCGAL_C_API SFCGAL_NO_EXPORT
#endif

#endif // _SFCGAL_EXPORT_H_
