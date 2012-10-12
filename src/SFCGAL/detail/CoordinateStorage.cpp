#include <SFCGAL/detail/CoordinateStorage.h>

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


}//namespace detail
}//namespace SFCGAL
