#include <SFCGAL/Geometry.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

namespace SFCGAL {

///
///
///
Geometry::~Geometry()
{

}

///
///
///
std::string Geometry::asText( const int & numDecimals ) const
{
	std::ostringstream oss;
	if ( numDecimals >= 0 ){
		oss << std::fixed ;
		oss.precision( numDecimals );
	}
	io::WktWriter writer( oss );
	writer.write( *this );
	return oss.str();
}



///
///
///
Geometry::Geometry() : dirty_(true)
{

}

///
///
///
Geometry::Geometry( Geometry const& other ) : dirty_(true)
{

}

BoundingBox Geometry::envelope() const
{
	if ( dirty_ ) {
		computeBoundingBox();
		dirty_ = false;
	}
	return bbox_;
}

void Geometry::computeBoundingBox() const
{
	// default behaviour for the bounding box computation

	double minf = std::numeric_limits<double>::infinity();
	double xmin = +minf;
	double ymin = +minf;
	double zmin = +minf;
	double xmax = -minf;
	double ymax = -minf;
	double zmax = -minf;

	detail::GetPointsVisitor pointVisitor;
	accept( pointVisitor );

	if ( is3D() ) {
		for ( size_t i = 0; i < pointVisitor.points.size(); ++i ) {
			const Point& p = *(pointVisitor.points[i]);
			if ( p.x() < xmin ) {
				xmin = p.x();
			}
			if ( p.x() > xmax ) {
				xmax = p.x();
			}
			if ( p.y() < ymin ) {
				ymin = p.y();
			}
			if ( p.y() > ymax ) {
				ymax = p.y();
			}
		}
		bbox_ = BoundingBox( xmin, xmax, ymin, ymax );
	}
	else {
		for ( size_t i = 0; i < pointVisitor.points.size(); ++i ) {
			const Point& p = *(pointVisitor.points[i]);
			if ( p.x() < xmin ) {
				xmin = p.x();
			}
			if ( p.x() > xmax ) {
				xmax = p.x();
			}
			if ( p.y() < ymin ) {
				ymin = p.y();
			}
			if ( p.y() > ymax ) {
				ymax = p.y();
			}
			if ( p.z() < zmin ) {
				zmin = p.z();
			}
			if ( p.z() > zmax ) {
				zmax = p.z();
			}
		}
		bbox_ = BoundingBox( xmin, xmax, ymin, ymax, zmin, zmax );
	}
}

}//SFCGAL

