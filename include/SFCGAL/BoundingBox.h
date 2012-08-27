#ifndef _SFCGAL_BOUNDING_BOX_H_
#define _SFCGAL_BOUNDING_BOX_H_

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>

#include <SFCGAL/Coordinate.h>

namespace SFCGAL {

class BoundingBox
{
 public:
	BoundingBox() {}

	BoundingBox( const Coordinate& cmin, const Coordinate& cmax ) :
		cmin_(cmin), cmax_(cmax) {}

	BoundingBox( double xmin, double xmax,
		     double ymin, double ymax ) :
		cmin_( xmin, ymin ), cmax_( xmax, ymax ) {}

	BoundingBox( double xmin, double xmax,
		     double ymin, double ymax,
		     double zmin, double zmax ) :
		cmin_( xmin, ymin, zmin ), cmax_( xmax, ymax, zmax ) {}
	
	bool is3D() const { return cmin_.is3D(); }
	
	const double& xMin() const { return cmin_.x(); }
	double& xMin() { return cmin_.x(); }
	
	const double& yMin() const { return cmin_.y(); }
	double& yMin() { return cmin_.y(); }
	
	const double& zMin() const { return cmin_.z(); }
	double& zMin() { return cmin_.z(); }
	
	const double& xMax() const { return cmax_.x(); }
	double& xMax() { return cmax_.x(); }
	
	const double& yMax() const { return cmax_.y(); }
	double& yMax() { return cmax_.y(); }
	
	const double& zMax() const { return cmax_.z(); }
	double& zMax() { return cmax_.z(); }

	/**
	 * Convenience function. Convert to CGAL::BBox_3
	 */
	CGAL::Bbox_3 toBbox_3() {
		return CGAL::Bbox_3( cmin_.x(), cmin_.y(), cmin_.z(),
				     cmax_.x(), cmax_.y(), cmax_.z() );
	}
	
	/**
	 * Convenience function. Convert to CGAL::BBox_2
	 */
	CGAL::Bbox_2 toBbox_2() {
		return CGAL::Bbox_2( cmin_.x(), cmin_.y(),
				     cmax_.x(), cmax_.y() );
	}

 private:
	Coordinate cmin_, cmax_;
};

}
#endif
