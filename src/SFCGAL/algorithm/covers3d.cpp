#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_inside_polyhedron_3.h>

#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/all.h>

#include <SFCGAL/io/GeometryStreams.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

namespace SFCGAL {
namespace algorithm
{
	//
	// Test points inside a volume. Optimisation preventing recomputing of AABB trees needed by point_inside_polyhedron
	//
	bool covers3D( std::vector<const Point*>& pts, const Solid& solid )
	{
		typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
		
		std::auto_ptr<Polyhedron> ext_shell_poly = solid.exteriorShell().toPolyhedron_3<Kernel, Polyhedron>();
		BOOST_ASSERT( ext_shell_poly.get() != 0 );
		
		CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside_ext( *ext_shell_poly );
		for ( size_t j = 0; j < pts.size(); ++j ) {
			if ( !point_inside_ext( pts[j]->toPoint_3<Kernel>() )) {
				return false;
			}
		}
		
		//
		// test whether points are inside interior shells or not
		//
		for ( size_t i = 0; i < solid.numInteriorShells(); ++i ) {
			std::auto_ptr<Polyhedron> shell_poly = solid.interiorShellN(i).toPolyhedron_3<Kernel, Polyhedron>();
			CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside( *shell_poly );
			for ( size_t j = 0; j < pts.size(); ++j ) {
				if ( point_inside( pts[j]->toPoint_3<Kernel>() )) {
					// FIXME : process nested holes
					return false;
				}
			}
		}
		
		return true;
	}

	static bool covers3D_x_solid_( const Geometry& ga, const Solid& solid )
	{
		//
		// Now consider the geometry as a bunch of points
		detail::GetPointsVisitor visitor;
		ga.accept( visitor );
		return covers3D( visitor.points, solid );
	}

	bool covers3D( const Geometry& ga, const Geometry& gb )
	{
		// deal with geometry collection
		// call intersects on each geometry of the collection
		const GeometryCollection* coll;
		if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( covers( coll->geometryN( i ), gb ) ) {
					return true;
				}
			}
			return false;
		}
		if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( covers( ga, coll->geometryN( i ) ) ) {
					return true;
				}
			}
			return false;
		}

		//
		// first test if bounding boxes are compliants
		if ( !Envelope::contains( ga.envelope(), gb.envelope() )) {
			return false;
		}

		if ( ga.geometryTypeId() == TYPE_SOLID ) {
		    // Only another solid can be covered by a solid
		    if ( gb.geometryTypeId() != TYPE_SOLID ) {
			return false;
		    }
		    return covers3D_x_solid_( static_cast<const Solid&>(ga), static_cast<const Solid&>(gb) );
		}
		else if ( gb.geometryTypeId() == TYPE_SOLID ) {
			return covers3D_x_solid_( ga, static_cast<const Solid&>(gb) );
		}

		// default behaviour
		return false;
	}
}
}
