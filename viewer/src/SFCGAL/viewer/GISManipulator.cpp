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
#include <iostream>

#include <osg/io_utils>

#include <SFCGAL/viewer/GISManipulator.h>

namespace SFCGAL {
namespace viewer {

	GISManipulator::GISManipulator() : osgGA::FirstPersonManipulator()
	{
	}

	void GISManipulator::init (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
	{
		std::cout << "init" << std::endl;
		osgGA::FirstPersonManipulator::init( ea, us );
		setTransformation( /*eye*/ osg::Vec3f( 0.0, 0.0, 100.0 ), /*center*/ osg::Vec3f( 0.0, 0.0, 0.0 ), /* up */ osg::Vec3f( 0.0, 0.0, 1.0 ) );
	}

	bool GISManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
	{
		// TODO: compute the right amount in order for one mouse's pixel movement to correspond to a movement of one object's pixel
		osg::Vec3d eye, center, up;
		getTransformation( eye, center, up );

		double altitude = eye[2];

		moveRight( -dx * altitude / 8.0 );
		moveUp( -dy * altitude / 8.0 );

		return true;
	}
	
	bool GISManipulator::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
	{
		osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
		
		osg::Vec3d eye, center, up;
		getTransformation( eye, center, up );
		double altitude = eye[2];

		if ( sm == osgGA::GUIEventAdapter::SCROLL_UP ) {
			altitude /= 1.41;
		}
		else if ( sm == osgGA::GUIEventAdapter::SCROLL_DOWN ) {
			altitude *= 1.41;
		}
		else {
			return false;
		}
		
		eye[2] = altitude;
		center[2] = eye[2] - 1.0;
		setTransformation( eye, center, up );
		
		us.requestRedraw();
		us.requestContinuousUpdate( isAnimating() );
		return true;
	}

} // namespace viewer
} // namespace SFCGAL
