#ifndef _SFCGAL_VIEWER_GISMANIPULATOR_H_
#define _SFCGAL_VIEWER_GISMANIPULATOR_H_

#include <osgGA/FirstPersonManipulator>

namespace SFCGAL {
namespace viewer {

class GISManipulator : public osgGA::FirstPersonManipulator {
public:
    GISManipulator();

    void init ( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

protected:
    bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );

    bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
};

}//viewer
}//SFCGAL


#endif
