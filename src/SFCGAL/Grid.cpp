#include <SFCGAL/Grid.h>
#include <SFCGAL/all.h>


namespace SFCGAL {

///
///
///
Grid::Grid():
	_data(),
	_xMin(0.0),
	_yMax(0.0),
	_dx(1.0),
	_dy(1.0)
{

}

///
///
///
Grid::Grid( const ublas::matrix< double > & data, const Envelope & extent ):
	_data(data),
	_xMin(extent.xMin()),
	_yMax(extent.yMax()),
	_dx(extent.boundsN(0).width() / ( data.size1() - 1 ) ),
	_dy(extent.boundsN(1).width() / ( data.size2() - 1 ) )
{

}

///
///
///
Grid::Grid( const Grid & other ):
	_data(other._data),
	_xMin(other._xMin),
	_yMax(other._yMax),
	_dx(other._dx),
	_dy(other._dy)
{

}


///
///
///
Grid& Grid::operator = ( const Grid & other )
{
	_data     = other._data ;
	_xMin     = other._xMin ;
	_yMax     = other._yMax ;
	_dx       = other._dx   ;
	_dy       = other._dy   ;
	return * this ;
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
bool  Grid::isEmpty() const
{
	return _data.size1() == 0 || _data.size2() == 0 ;
}



///
///
///
TriangulatedSurface Grid::toTriangulatedSurface() const
{
	TriangulatedSurface triangulatedSurface ;

	if ( isEmpty() ){
		return triangulatedSurface ;
	}

	for ( size_t i = 0; i < width() - 1 ; i++ ){
		for ( size_t j = 0; j < height() - 1 ; j++ ){
			/*
			 * a    b
			 *
			 * c    d
			 */
			Point a = point( i  , j   );
			Point b = point( i+1, j   );
			Point c = point( i  , j+1 );
			Point d = point( i+1, j+1 );

			if ( a.is3D() && b.is3D() && d.is3D() )
				triangulatedSurface.addTriangle( Triangle(a,c,d) );

			if ( a.is3D() && d.is3D() && b.is3D() )
				triangulatedSurface.addTriangle( Triangle(a,d,b) );
		}
	}

	return triangulatedSurface ;
}



}//SFCGAL


