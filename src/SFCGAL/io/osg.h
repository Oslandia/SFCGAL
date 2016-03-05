#ifndef _SFCGAL_IO_OSG_H_
#define _SFCGAL_IO_OSG_H_

#include <osg/Geometry>
#include <osg/Group>
#include <osg/Geode>

#include <osgDB/WriteFile>

#include <SFCGAL/io/detail/OsgGeometryBuilder.h>

namespace SFCGAL {
namespace io {


/**
 * @brief [helper] converts a SFCGAL::Geometry to an OSG geometry
 * @warning requires OpenSceneGraph (osg and osgDB), should not be linked to core library
 */
template < typename K >
osg::ref_ptr< osg::Geometry > toOsgGeometry( const Geometry<K>& g ) {
    SFCGAL::io::detail::OsgGeometryBuilder<K> builder ;
    builder.addGeometry(g);
    return builder.getOsgGeometry() ;
}

/**
 * @brief [helper] write an OSG geometry in a file. The format depends on the extension (http://www.openscenegraph.org/projects/osg/wiki/Support/UserGuides/Plugins),
 *  but you can use for example the .osg extension.
 *
 * Note that osgviewer command line executable allows the visualization of the resulting files, screenshots and co.
  
 * example : osgviewer "yourfile.osg"
 *
 * @warning requires OpenSceneGraph (osg and osgDB), should not be linked to core library
 * @todo list of geometries?
 */
template < typename K >
void osgWriteFile( const Geometry<K>& g, const std::string& filepath ){
    osg::ref_ptr< osg::Geometry > osgGeometry = toOsgGeometry<K>(g) ;
    osg::ref_ptr< osg::Geode > geode = new osg::Geode;
    //TODO restore
    //geode->setName( g.geometryType() );
    geode->addDrawable( osgGeometry );
    osgDB::writeNodeFile( *geode, filepath ) ;
}

} // namespace io    
} // namespace SFCGAL


#endif
