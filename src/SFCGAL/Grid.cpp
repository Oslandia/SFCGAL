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
#include <SFCGAL/Grid.h>

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
	const size_t & width,
	const size_t & heigth,
	const Envelope & limits,
	const PixelConvention & pixelType
):
	_data(ublas::scalar_matrix< double >(width,heigth,NaN())),
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


} // namespace SFCGAL
