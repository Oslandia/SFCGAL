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
#include <SFCGAL/Grid.h>

#include <SFCGAL/Triangle.h>
#include <SFCGAL/TriangulatedSurface.h>

namespace SFCGAL {

///
///
///
Grid::Grid():
	_data(),
	_limits(Envelope(0.0,1.0,0.0,1.0)),
	_pixelConvention(PIXEL_IS_POINT)
{

}

///
///
///
Grid::Grid(
	const size_t & nrows,
	const size_t & ncols,
	const double& fillValue,
	const Envelope & limits,
	const PixelConvention & pixelType
):
	_data( ublas::scalar_matrix< double >( nrows, ncols, fillValue ) ),
	_limits(limits),
	_pixelConvention(pixelType)
{

}


///
///
///
Grid::Grid(
	const ublas::matrix< double > & data,
	const Envelope & limits,
	const PixelConvention & pixelType
):
	_data(data),
	_limits(limits),
	_pixelConvention(pixelType)
{

}

///
///
///
Grid::Grid( const Grid & other )
{
	(*this) = other ;
}

///
///
///
Grid& Grid::operator = ( const Grid & other )
{
	_data      = other._data ;
	_limits    = other._limits ;
	_pixelConvention = other._pixelConvention ;
	return *this ;
}

///
///
///
Grid::~Grid()
{

}


///
///
///
std::auto_ptr< TriangulatedSurface > Grid::toTrianguledSurface() const
{
	std::auto_ptr< TriangulatedSurface > result( new TriangulatedSurface() ) ;
	if ( nrows() <= 1 || ncols() <= 1 ){
		return result ;
	}

	for ( size_t i = 0; i < nrows() - 1; i++ ){
		for ( size_t j = 0; j < ncols() - 1; j++ ){
			/*
			 * a    b
			 *
			 * d    c
			 */
			Point a = point(i,j);
			Point b = point(i,j+1);
			Point c = point(i+1,j+1);
			Point d = point(i+1,j);

			if ( d.is3D() && c.is3D() && b.is3D() ){
				result->addTriangle( new Triangle( d, c, b ) );
			}
			if ( d.is3D() && b.is3D() && a.is3D() ){
				result->addTriangle( new Triangle( d, b, a ) );
			}
		}
	}
	return result ;
}

} // namespace SFCGAL
