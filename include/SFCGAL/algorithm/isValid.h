/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_ALGORITHM_ISVALID_H_
#define _SFCGAL_ALGORITHM_ISVALID_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Validity.h>
#include <SFCGAL/algorithm/force2D.h>
#include <SFCGAL/algorithm/force3D.h>

namespace SFCGAL {
namespace algorithm {

/**
 * @brief Check validity of a geometry
 * @ingroup public_api
 */
SFCGAL_API const Validity isValid( const Geometry& g, const double& toleranceAbs= 1e-9 );

/**
 * Macro used to by-pass validity check
 * @note do not convert to function since BOOST_THROW_EXCEPTION locates the throwing point (function and line)
 * @note exception message is apparently limited in length, thus print the reason for invalidity before its text representation (that can be very long)
 */
#ifndef SFCGAL_NEVER_CHECK_VALIDITY
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY(g) \
    {\
        using namespace SFCGAL;\
        const Validity sfcgalAssertGeometryValidity = algorithm::isValid( g );\
        if ( ! sfcgalAssertGeometryValidity ) {\
            BOOST_THROW_EXCEPTION(GeometryInvalidityException(\
                ( boost::format("%s is invalid : %s : %s")\
                  % g.geometryType()\
                  % sfcgalAssertGeometryValidity.reason()\
                  % g.asText()\
                ).str()\
            ));\
        }\
    }
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D(g) \
    {\
        using namespace SFCGAL;\
        if ( g.is3D() ) {\
            std::auto_ptr<SFCGAL::Geometry> sfcgalAssertGeometryValidityClone( g.clone() );\
            algorithm::force2D( *sfcgalAssertGeometryValidityClone );\
            SFCGAL_ASSERT_GEOMETRY_VALIDITY( (*sfcgalAssertGeometryValidityClone) );\
        }\
        else {\
            SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );\
        }\
    }
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D(g) \
    {\
        using namespace SFCGAL;\
        if ( !g.is3D() ) {\
            std::auto_ptr<Geometry> sfcgalAssertGeometryValidityClone( g.clone() );\
            algorithm::force3D( *sfcgalAssertGeometryValidityClone );\
            SFCGAL_ASSERT_GEOMETRY_VALIDITY( (*sfcgalAssertGeometryValidityClone) );\
        }\
        else {\
            SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );\
        }\
    }
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_ON_PLANE(g, plane) \
   BOOST_THROW_EXCEPTION( NotImplementedException("validation on geometry projected on arbitrary plane is not implemented") );
#else
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY(g)
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D(g)
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D(g)
#  define SFCGAL_ASSERT_GEOMETRY_VALIDITY_ON_PLANE(g, plane)
#endif

/**
 * Tag used for variants of algorithm that do not do validity check
 * @ingroup public_api
 */
struct NoValidityCheck {};

}//algorithm
}//SFCGAL


#endif
