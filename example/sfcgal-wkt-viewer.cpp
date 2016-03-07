#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/wkt.h>

#ifdef SFCGAL_WITH_OSG
#include <SFCGAL/io/osg.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#endif

using namespace SFCGAL ;

/**
 * Note : If your geometry is 2D, drag to rotate camera to see it. 
 */
int main( int argc, char* argv[] ){
    std::string wkt = "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0),(0.2 0.2,0.2 0.8,0.8 0.8,0.8 0.2,0.2 0.2))";
    if ( argc > 1 ){
        wkt = argv[1];
    }
    std::unique_ptr< Geometry<Epeck> > g = io::readWkt<Epeck>(wkt);

#ifdef SFCGAL_WITH_OSG
    osg::ref_ptr< osg::Geometry > osgGeometry = io::toOsgGeometry<Epeck>(*g) ;
    osg::ref_ptr< osg::Geode > geode = new osg::Geode;
    geode->setName("WKT");
    geode->addDrawable( osgGeometry );
    
    osgViewer::Viewer viewer;
    viewer.setSceneData(geode);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    return viewer.run();
#else
    std::cout << "OSG disabled" << std::endl;
    return 0 ;
#endif
}
