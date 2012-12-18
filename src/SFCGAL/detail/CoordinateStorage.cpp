#include <SFCGAL/detail/CoordinateStorage.h>
#include <SFCGAL/numeric.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>


namespace SFCGAL {
namespace detail {

/**
 * convert to point 2
 */
struct ToPoint_2Visitor : public boost::static_visitor< Kernel::Point_2 > {
public:
	Kernel::Point_2 operator() ( const Empty & coordinateStorage ) const {
		return Kernel::Point_2( CGAL::ORIGIN ) ;
	}
	Kernel::Point_2 operator() ( const Kernel::Point_2 & coordinateStorage ) const {
		return coordinateStorage ;
	}
	Kernel::Point_2 operator() ( const Kernel::Point_3 & coordinateStorage ) const {
		return Kernel::Point_2( coordinateStorage.cartesian(0), coordinateStorage.cartesian(1) ) ;
	}
};

/**
 * convert to point 3
 */
struct ToPoint_3Visitor : public boost::static_visitor< Kernel::Point_3 > {
public:
	Kernel::Point_3 operator() ( const Empty & coordinateStorage ) const {
		return Kernel::Point_3(0.,0.,0.) ;
	}
	Kernel::Point_3 operator() ( const Kernel::Point_2 & coordinateStorage ) const {
		return Kernel::Point_3(
			coordinateStorage.cartesian(0),
			coordinateStorage.cartesian(1),
			0
		) ;
	}
	Kernel::Point_3 operator() ( const Kernel::Point_3 & coordinateStorage ) const {
		return coordinateStorage ;
	}
};


struct GetXVisitor : public boost::static_visitor< Kernel::FT > {
public:
	Kernel::FT operator() ( const Empty & coordinateStorage ) const {
		return 0 ;
	}
	Kernel::FT operator() ( const Kernel::Point_2 & coordinateStorage ) const {
		return coordinateStorage.x() ;
	}
	Kernel::FT operator() ( const Kernel::Point_3 & coordinateStorage ) const {
		return coordinateStorage.x() ;
	}
};

struct GetYVisitor : public boost::static_visitor< Kernel::FT > {
public:
	Kernel::FT operator() ( const Empty & coordinateStorage ) const {
		return 0 ;
	}
	Kernel::FT operator() ( const Kernel::Point_2 & coordinateStorage ) const {
		return coordinateStorage.y() ;
	}
	Kernel::FT operator() ( const Kernel::Point_3 & coordinateStorage ) const {
		return coordinateStorage.y() ;
	}
};

struct GetZVisitor : public boost::static_visitor< Kernel::FT > {
public:
	Kernel::FT operator() ( const Empty & coordinateStorage ) const {
		return 0 ;
	}
	Kernel::FT operator() ( const Kernel::Point_2 & coordinateStorage ) const {
		return 0 ;
	}
	Kernel::FT operator() ( const Kernel::Point_3 & coordinateStorage ) const {
		return coordinateStorage.z() ;
	}
};

///
///
///
bool isEmpty( const CoordinateStorage & coordinateStorage )
{
	return coordinateStorage.which() == 0 ;
}

///
///
///
bool is3D( const CoordinateStorage & coordinateStorage )
{
	return coordinateStorage.which() == 2 ;
}

///
///
///
Kernel::FT x( const CoordinateStorage & coordinateStorage )
{
	GetXVisitor visitor ;
	return boost::apply_visitor( visitor, coordinateStorage );
}

///
///
///
Kernel::FT y( const CoordinateStorage & coordinateStorage )
{
	GetYVisitor visitor ;
	return boost::apply_visitor( visitor, coordinateStorage );
}

///
///
///
Kernel::FT z( const CoordinateStorage & coordinateStorage )
{
	GetZVisitor visitor ;
	return boost::apply_visitor( visitor, coordinateStorage );
}

///
///
///
Kernel::Point_2 toPoint_2( const CoordinateStorage & coordinateStorage )
{
	ToPoint_2Visitor toPoint_2Visitor;
	return boost::apply_visitor( toPoint_2Visitor, coordinateStorage );
}

///
///
///
Kernel::Point_3 toPoint_3( const CoordinateStorage & coordinateStorage )
{
	ToPoint_3Visitor toPoint_3Visitor;
	return boost::apply_visitor( toPoint_3Visitor, coordinateStorage );
}


///
///
///
Kernel::Vector_2 toVector_2( const CoordinateStorage & coordinateStorage )
{
	return toPoint_2(coordinateStorage) - CGAL::ORIGIN ;
}

///
///
///
Kernel::Vector_3 toVector_3( const CoordinateStorage & coordinateStorage )
{
	return toPoint_3(coordinateStorage) - CGAL::ORIGIN ;
}


}//namespace detail
}//namespace SFCGAL
