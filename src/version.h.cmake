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

#ifndef _SFCGAL_VERSION_H_
#define _SFCGAL_VERSION_H_

#include <SFCGAL/export.h>

#define SFCGAL_VERSION_MAJOR @SFCGAL_VERSION_MAJOR@
#define SFCGAL_VERSION_MINOR @SFCGAL_VERSION_MINOR@
#define SFCGAL_VERSION_PATCH @SFCGAL_VERSION_PATCH@

#define SFCGAL_VERSION "@SFCGAL_VERSION@"

namespace SFCGAL {
    SFCGAL_CXX_API const char* Version();
}

#endif

