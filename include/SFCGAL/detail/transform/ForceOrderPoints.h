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
#ifndef _SFCGAL_TRANSFORM_FORCE_ORDER_POINTS_H_
#define _SFCGAL_TRANSFORM_FORCE_ORDER_POINTS_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Transform.h>

namespace SFCGAL {

namespace transform {

	/**
	 * If the 2D surface is pointing down, reverse its points
	 * @todo unittest
	 * @todo move outside (it's not a coordinate transform)?
	 */
	class SFCGAL_API ForceOrderPoints : public Transform {
	public:
		/**
		 * Pass the forced orientation as parameter
		 */
		ForceOrderPoints( bool orientCCW = true ) ;

		/*
		 * [SFCGAL::Transform]
		 */
		virtual void transform( Point & p ) ;

		virtual void visit( Triangle& );
		virtual void visit( Polygon& );

	private:
		bool _orientCCW;
	};


}//transform
}//SFCGAL




#endif

