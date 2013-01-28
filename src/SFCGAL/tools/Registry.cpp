/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#include <SFCGAL/tools/Registry.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/all.h>
#include <SFCGAL/tools/Log.h>

namespace SFCGAL {
namespace tools {

Registry * Registry::_instance = 0 ;

///
///
///
Registry::~Registry()
{
	for ( prototype_iterator it = _prototypes.begin(); it != _prototypes.end(); ++it ){
		delete *it ;
	}
}

///
///
///
void Registry::addPrototype( const Geometry & g )
{
	//find prototype by name
	const_prototype_iterator it = _prototypes.begin() ;
	for ( ; it != _prototypes.end(); ++it ){
		if ( (*it)->geometryTypeId() == g.geometryTypeId() )
			break;
	}

	if ( it != _prototypes.end() ){
		return ;
	}
	_prototypes.push_back( g.clone() );
}

///
///
///
std::vector< std::string > Registry::getGeometryTypes() const
{
	std::vector< std::string > names ;
	for ( const_prototype_iterator it = _prototypes.begin(); it != _prototypes.end(); ++it ){
		names.push_back( (*it)->geometryType() );
	}
	return names ;
}

///
///
///
Geometry*  Registry::newGeometryByTypeName( const std::string & geometryTypeName ) const
{
	for ( const_prototype_iterator it = _prototypes.begin(); it != _prototypes.end(); ++it ){
		if ( geometryTypeName == (*it)->geometryType() ){
			return (*it)->clone() ;
		}
	}

	SFCGAL_WARNING( boost::format("Registry can't create a new Geometry for the type '%s' (returning null pointer)") % geometryTypeName );
	return NULL ;
}

///
///
///
Registry & Registry::instance()
{
	if ( ! Registry::_instance ){
		Registry::_instance = new Registry();
	}
	return *_instance ;
}

///
///
///
Registry::Registry():
	_prototypes()
{
	addPrototype(Point());
	addPrototype(LineString());
	addPrototype(Polygon());
	addPrototype(Triangle());
	addPrototype(Solid());

	addPrototype(GeometryCollection());

	addPrototype(MultiPoint());
	addPrototype(MultiLineString());
	addPrototype(MultiPolygon());
	addPrototype(MultiSolid());

	addPrototype(TriangulatedSurface());
	addPrototype(PolyhedralSurface());
}



}//namespace tools
}//namespace SFCGAL


