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

#ifndef _SFCGAL_KERNEL_H_
#define _SFCGAL_KERNEL_H_

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

namespace SFCGAL {

/**
 * Required kernel for robutness in CGAL when predicates are invoked (line side for ex.)
 */
typedef CGAL::Exact_predicates_inexact_constructions_kernel Epick ;
/**
 * Required kernel for robutness in CGAL when geometries are built
 */
typedef CGAL::Exact_predicates_exact_constructions_kernel Epeck ;

/**
 * default working Kernel
 */
typedef Epeck Kernel ;


/**
 * Quotient type
 */
typedef CGAL::Gmpq QT;

}//namespace SFCGAL

#endif
