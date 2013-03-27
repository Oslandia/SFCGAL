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
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/GeometryVisitor.h>


namespace SFCGAL {

///
///
///
MultiPoint::MultiPoint():
	GeometryCollection()
{

}

///
///
///
MultiPoint::MultiPoint( MultiPoint const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiPoint& MultiPoint::operator = ( const MultiPoint & other )
{
	assign(other) ;
	return *this ;
}

///
///
///
MultiPoint::~MultiPoint()
{

}

///
///
///
MultiPoint *   MultiPoint::clone() const
{
	return new MultiPoint(*this);
}

///
///
///
std::string    MultiPoint::geometryType() const
{
	return "MultiPoint" ;
}

///
///
///
GeometryType   MultiPoint::geometryTypeId() const
{
	return TYPE_MULTIPOINT ;
}

///
///
///
bool           MultiPoint::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_POINT ;
}



///
///
///
void MultiPoint::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void MultiPoint::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL

