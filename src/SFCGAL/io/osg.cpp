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

#include <SFCGAL/config.h>
#ifdef SFCGAL_WITH_OSG

#include <SFCGAL/io/osg.h>
#include <SFCGAL/detail/io/OsgFactory.h>

#include <osg/Geometry>
#include <osg/Group>
#include <osg/Geode>

#include <osgDB/WriteFile>

namespace SFCGAL {
namespace io {

///
///
///
void osgWriteFile( const Geometry& g, const std::string& filepath )
{
    SFCGAL::detail::io::OsgFactory factory ;
    osg::ref_ptr< osg::Geometry > osgGeometry = factory.createGeometry( g );
    osg::ref_ptr< osg::Geode > geode = new osg::Geode;
    geode->setName( g.geometryType() );
    geode->addDrawable( osgGeometry );
    osgDB::writeNodeFile( *geode, filepath ) ;
}


///
///
///
osg::Geometry* toOsgGeometry( const Geometry& g )
{
    SFCGAL::detail::io::OsgFactory factory ;
    return factory.createGeometry( g ) ;
}


} // namespace io
} // namespace SFCGAL


#endif //SFCGAL_WITH_OSG
