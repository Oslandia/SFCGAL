#include <SFCGAL/config.h>
#ifdef SFCGAL_WITH_OSG

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
void osgWriteFile( const Geometry & g, const std::string& filepath )
{
	SFCGAL::detail::io::OsgFactory factory ;
	osg::ref_ptr< osg::Geometry > osgGeometry = factory.createGeometry( g );
	osg::ref_ptr< osg::Geode > geode = new osg::Geode;
	geode->setName( g.geometryType() );
	geode->addDrawable( osgGeometry );
	osgDB::writeNodeFile(*geode, filepath ) ;
}

} // namespace io
} // namespace SFCGAL


#endif //SFCGAL_WITH_OSG
