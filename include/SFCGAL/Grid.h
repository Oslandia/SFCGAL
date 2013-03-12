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
#ifndef _SFCGAL_GRID_H_
#define _SFCGAL_GRID_H_

#include <SFCGAL/ublas.h>

#include <SFCGAL/Envelope.h>
#include <SFCGAL/Point.h>

namespace SFCGAL {

	/**
	 * @brief represents a Grid.
	 *
	 * left-right, top-down orientation :
	 *                         i
	 * +----------------------->
	 * |
	 * |
	 * |
	 * |
	 * v j
	 *
	 *
	 * NaN represents undefined values
	 */
	class Grid {
	public:
		typedef enum {
			PIXEL_IS_POINT,
			PIXEL_IS_AREA
		} PixelType ;

		/**
		 * @brief default constructor
		 */
		Grid();
		/**
		 * @brief default constructor
		 */
		Grid(
			const size_t & width,
			const size_t& heigth,
			const Envelope & limits = Envelope(0.0,1.0,0.0,1.0),
			const PixelType & pixelType = PIXEL_IS_POINT
		);
		/**
		 * @brief copy constructor
		 */
		Grid(
			const ublas::matrix< double > & data,
			const Envelope & limits = Envelope(0.0,1.0,0.0,1.0),
			const PixelType & pixelType = PIXEL_IS_POINT
		);
		/**
		 * @brief copy constructor
		 */
		Grid( const Grid & other );
		/**
		 * @brief copy constructor
		 */
		Grid& operator = ( const Grid & other );
		/**
		 * @brief destructor
		 */
		~Grid();


		/**
		 * @brief test if the grid is empty
		 */
		inline bool isEmpty() const {
			return ( _data.size1() == 0U ) || ( _data.size2() == 0U ) ;
		}

		/**
		 * @brief get a point at a specific location
		 */
		inline Point point( const size_t & i, const size_t & j ) const {
			if ( _pixelType == PIXEL_IS_POINT ){
				return Point(
					_limits.xMin() + i * dx(),
					_limits.yMax() - j * dy(),
					_data(i,j)
				);
			}else{
				return Point(
					_limits.xMin() + ( 0.5 + i ) * dx(),
					_limits.yMax() - ( 0.5 + j ) * dy(),
					_data(i,j)
				);
			}
		}

		/**
		 * @brief get the width of a pixel
		 */
		inline double dx() const {
			if ( _pixelType == PIXEL_IS_POINT ){
				return _limits.boundsN(0).width() / ( _data.size2() - 1 ) ;
			}else{
				return _limits.boundsN(0).width() / ( _data.size2() ) ;
			}
		}

		/**
		 * @brief get the height of a pixel
		 */
		inline double dy() const {
			if ( _pixelType == PIXEL_IS_POINT ){
				return _limits.boundsN(1).width() / ( _data.size1() - 1 ) ;
			}else{
				return _limits.boundsN(1).width() / ( _data.size1() ) ;
			}
		}


		/**
		 * @brief get Z for a given location
		 */
		inline const double & z( const size_t& i, const size_t& j ) const {
			return _data(i,j) ;
		}
		/**
		 * @brief get Z for a given location
		 */
		inline double & z( const size_t& i, const size_t& j ) {
			return _data(i,j) ;
		}

		/**
		 * @brief gets the width of the grid
		 */
		inline const size_t ncols() const {
			return _data.size2() ;
		}
		/**
		 * @brief gets the height of the grid
		 */
		inline const size_t nrows() const {
			return _data.size1() ;
		}

		/**
		 * @brief access to grid limits
		 */
		inline const Envelope & limits() const { return _limits ; }
		/**
		 * @brief access to grid limits
		 */
		inline Envelope & limits() { return _limits ; }

		/**
		 * @brief access to grid data
		 */
		inline ublas::matrix< double > & data() {
			return _data ;
		}
		/**
		 * @brief access to grid data
		 */
		inline const ublas::matrix< double > & data() const {
			return _data ;
		}
	private:
		/**
		 * @brief grid data
		 */
		ublas::matrix< double > _data ;
		/**
		 * @brief grid extent
		 */
		Envelope _limits ;
		/**
		 * @brief pixel type
		 */
		PixelType _pixelType ;
	};

} // namespace SFCGAL

#endif
