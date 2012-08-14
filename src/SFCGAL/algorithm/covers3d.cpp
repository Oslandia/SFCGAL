#include <SFCGAL/algorithm/covers.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_inside_polyhedron_3.h>

#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/all.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

namespace SFCGAL {
namespace algorithm
{
	bool covers3D_( const Point& pta, const Solid& solid )
	{
		typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
		
		bool is_inside = false;
		Polyhedron ext_shell_poly = solid.exteriorShell().toPolyhedron_3<Kernel>();
		
		CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside_ext( ext_shell_poly );
		if ( point_inside_ext( pta.toPoint_3<Kernel>() )) {
			is_inside = true;
			//
			// test whether the point is inside interior shells or not
			//
			for ( size_t i = 0; i < solid.numInteriorShells(); ++i ) {
				Polyhedron shell_poly = solid.interiorShellN(i).toPolyhedron_3<Kernel>();
				CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside( shell_poly );
				if ( point_inside( pta.toPoint_3<Kernel>() )) {
					// FIXME : process nested holes
					is_inside = false;
					break;
				}
			}
		}
		
		return is_inside;
	}

	//
	// Test points inside a volume. Optimisation preventing recomputing of AABB trees needed by point_inside_polyhedron
	//
	bool covers3D( std::vector<const Point*>& pts, const Solid& solid )
	{
		typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
		
		Polyhedron ext_shell_poly = solid.exteriorShell().toPolyhedron_3<Kernel>();
		
		CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside_ext( ext_shell_poly );
		for ( size_t j = 0; j < pts.size(); ++j ) {
			if ( !point_inside_ext( pts[j]->toPoint_3<Kernel>() )) {
				return false;
			}
		}
		
		//
		// test whether points are inside interior shells or not
		//
		for ( size_t i = 0; i < solid.numInteriorShells(); ++i ) {
			Polyhedron shell_poly = solid.interiorShellN(i).toPolyhedron_3<Kernel>();
			CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside( shell_poly );
			for ( size_t j = 0; j < pts.size(); ++j ) {
				if ( point_inside( pts[j]->toPoint_3<Kernel>() )) {
					// FIXME : process nested holes
					return false;
				}
			}
		}
		
		return true;
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

		if ( ga.geometryTypeId() == TYPE_POINT && gb.geometryTypeId() == TYPE_SOLID ) {
		    return covers3D_( static_cast<const Point&>(ga), static_cast<const Solid&>(gb) );
		}
		else if ( ga.geometryTypeId() == TYPE_SOLID && gb.geometryTypeId() == TYPE_POINT ) {
		    return covers3D_( static_cast<const Point&>(gb), static_cast<const Solid&>(ga) );
		}

		// default behaviour: calls intersects (NOT VALID !)
		return intersects3D( ga, gb );
	}
}
}
