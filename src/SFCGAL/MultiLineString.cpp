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
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
MultiLineString::MultiLineString():
	GeometryCollection()
{

}

///
///
///
MultiLineString::MultiLineString( MultiLineString const& other ):
	GeometryCollection(other)
{

}

///
///
///
MultiLineString& MultiLineString::operator = ( MultiLineString other )
{
	swap(other) ;
	return *this ;
}

///
///
///
MultiLineString::~MultiLineString()
{

}

///
///
///
MultiLineString *   MultiLineString::clone() const
{
	return new MultiLineString(*this);
}

///
///
///
std::string    MultiLineString::geometryType() const
{
	return "MultiLineString" ;
}

///
///
///
GeometryType   MultiLineString::geometryTypeId() const
{
	return TYPE_MULTILINESTRING ;
}

///
///
///
bool           MultiLineString::isAllowed( Geometry const& g )
{
	return g.geometryTypeId() == TYPE_LINESTRING ;
}

///
///
///
void MultiLineString::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void MultiLineString::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

}//SFCGAL

