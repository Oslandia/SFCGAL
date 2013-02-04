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
#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/numeric.h>

namespace SFCGAL {

///
///
///
Coordinate::Coordinate():
	_storage( Coordinate::Empty() )
{
}

///
///
///
Coordinate::Coordinate( const Kernel::FT & x, const Kernel::FT & y ):
	_storage( Kernel::Point_2(x, y) )
{

}

///
///
///
Coordinate::Coordinate( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ):
	_storage( Kernel::Point_3(x, y, z) )
{

}


///
///
///
Coordinate::Coordinate( const double & x, const double & y, const double & z )
{
	if ( isNaN(x) || isNaN(y) ){
		BOOST_ASSERT( isNaN(x) && isNaN(y) );
	}else{
		if ( isNaN(z) ){
			_storage = Kernel::Point_2(x, y);
		}else{
			_storage = Kernel::Point_3(x, y, z);
		}
	}
}


///
///
///
Coordinate::Coordinate( const Coordinate & other ):
	_storage( other._storage )
{

}

///
///
///
Coordinate& Coordinate::operator = ( const Coordinate & other )
{
	_storage = other._storage;
	return *this ;
}

///
///
///
Coordinate::~Coordinate()
{

}


///
///
///
int Coordinate::coordinateDimension() const
{
	if ( _storage.which() == 2 )
		return 3;
	return _storage.which() * 2;
}


///
///
///
bool Coordinate::isEmpty() const
{
	return _storage.which() == 0;
}

///
///
///
bool Coordinate::is3D() const
{
	return _storage.which() == 2;
}

class GetXVisitor : public boost::static_visitor<Kernel::FT>
{
public:
	Kernel::FT operator()( const Coordinate::Empty& ) const {
		return 0.0;
	}
	Kernel::FT operator()( const Kernel::Point_2& storage ) const {
		return storage.x();
		}
	Kernel::FT operator()( const Kernel::Point_3& storage ) const {
		return storage.x();
	}
};

///
///
///
Kernel::FT Coordinate::x() const
{
	GetXVisitor visitor;
	return boost::apply_visitor( visitor, _storage );
}

class GetYVisitor : public boost::static_visitor<Kernel::FT>
{
public:
	Kernel::FT operator()( const Coordinate::Empty& ) const {
		return 0.0;
	}
	Kernel::FT operator()( const Kernel::Point_2& storage ) const {
		return storage.y();
	}
	Kernel::FT operator()( const Kernel::Point_3& storage ) const {
		return storage.y();
	}
};

///
///
///
Kernel::FT Coordinate::y() const
{
	GetYVisitor visitor;
	return boost::apply_visitor( visitor, _storage );
}

class GetZVisitor : public boost::static_visitor<Kernel::FT>
{
public:
	Kernel::FT operator()( const Coordinate::Empty& ) const {
		return 0.0;
	}
	Kernel::FT operator()( const Kernel::Point_2& ) const {
		return 0.0;
	}
	Kernel::FT operator()( const Kernel::Point_3& storage ) const {
		return storage.z();
	}
};

///
///
///
Kernel::FT Coordinate::z() const
{
	GetZVisitor visitor;
	return boost::apply_visitor( visitor, _storage );
}

//----------------------


class RoundVisitor : public boost::static_visitor<>
{
public:
	RoundVisitor( const long& scaleFactor ):
		_scaleFactor(scaleFactor)
	{

	}

	void operator()( Coordinate::Empty& ) const {

	}
	void operator()( Kernel::Point_2& storage ) const {
		storage = Kernel::Point_2(
			_roundFT( storage.x() ),
			_roundFT( storage.y() )
		);
	}
	void operator()( Kernel::Point_3& storage ) const {
		storage = Kernel::Point_3(
			_roundFT( storage.x() ),
			_roundFT( storage.y() ),
			_roundFT( storage.z() )
		);
	}



private:
	long _scaleFactor ;


	Kernel::FT _roundFT( const Kernel::FT& v ) const {
		return Kernel::FT( CGAL::Gmpq(
			SFCGAL::round( v.exact() * _scaleFactor ),
			_scaleFactor
		) ) ;
	}

};


Coordinate& Coordinate::round( const long& scaleFactor )
{
	RoundVisitor roundVisitor( scaleFactor ) ;
	boost::apply_visitor( roundVisitor, _storage ) ;
	return *this ;
}



//----------------------

class ToPoint2Visitor : public boost::static_visitor<Kernel::Point_2>
{
public:
	Kernel::Point_2 operator()( const Coordinate::Empty& ) const {
		return Kernel::Point_2( CGAL::ORIGIN );
	}
	Kernel::Point_2 operator()( const Kernel::Point_2& storage ) const {
		return storage;
	}
	Kernel::Point_2 operator()( const Kernel::Point_3& storage ) const {
		return Kernel::Point_2( storage.x(), storage.y() );
	}
};

///
///
///
Kernel::Point_2 Coordinate::toPoint_2() const
{
	ToPoint2Visitor visitor;
	return boost::apply_visitor( visitor, _storage );
}

class ToPoint3Visitor : public boost::static_visitor<Kernel::Point_3>
{
public:
	Kernel::Point_3 operator()( const Coordinate::Empty& storage ) const {
		return Kernel::Point_3( CGAL::ORIGIN );
	}
	Kernel::Point_3 operator()( const Kernel::Point_2& storage ) const {
		return Kernel::Point_3( storage.x(), storage.y(), 0.0 );
	}
	Kernel::Point_3 operator()( const Kernel::Point_3& storage ) const {
		return storage;
	}
};

///
///
///
Kernel::Point_3 Coordinate::toPoint_3() const
{
	ToPoint3Visitor visitor;
	return boost::apply_visitor( visitor, _storage );
}

///
///
///
bool Coordinate::operator < ( const Coordinate & other ) const
{
	// no empty comparison
	if ( isEmpty() || other.isEmpty() ){
		BOOST_THROW_EXCEPTION( Exception("try to compare empty points using a < b ") );
	}

	// no mixed dimension comparison
	if ( ( is3D() && ! other.is3D() ) || ( ! is3D() && other.is3D() ) ){
		BOOST_THROW_EXCEPTION( Exception("try to compare empty points with different coordinate dimension using a < b") );
	}

	// comparison along x
	if ( x() < other.x() ){
		return true ;
	}else if ( other.x() < x() ){
		return false;
	}

	// comparison along y
	if ( y() < other.y() ){
		return true ;
	}else if ( other.y() < y() ){
		return false;
	}

	// comparison along z if possible
	if ( is3D() ){
		if ( z() < other.z() ){
			return true ;
		}else if ( other.z() < z() ){
			return false;
		}
	}

	// points are equals
	return false;
}

///
///
///
bool Coordinate::operator == ( const Coordinate & other ) const
{
	if ( isEmpty() ){
		return other.isEmpty() ;
	}
	if ( is3D() || other.is3D() ){
		return x() == other.x() && y() == other.y()  && z() == other.z() ;
	}else{
		return x() == other.x() && y() == other.y() ;
	}
}

///
///
///
bool Coordinate::operator != ( const Coordinate & other ) const
{
	return ! ( *this == other );
}


}//SFCGAL

