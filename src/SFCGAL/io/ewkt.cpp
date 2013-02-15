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
#include <SFCGAL/io/ewkt.h>

#include <SFCGAL/io/WktReader.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/tools/CharArrayBuffer.h>

namespace SFCGAL {
namespace io {

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( std::istream & s )
{
	WktReader wktReader(s);
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( const std::string & s )
{
	std::istringstream iss(s);
	WktReader wktReader(iss);
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

///
///
///
std::auto_ptr< PreparedGeometry > readEwkt( const char* str, size_t len )
{
	CharArrayBuffer buf( str, str + len );
	std::istream istr( &buf );
	WktReader wktReader( istr );
	srid_t srid = wktReader.readSRID();
	std::auto_ptr< Geometry > g( wktReader.readGeometry() );
	return std::auto_ptr<PreparedGeometry>(new PreparedGeometry( g, srid ));
}

}//io
}//SFCGAL


