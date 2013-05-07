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
#include <SFCGAL/generator/building.h>
#include <SFCGAL/all.h>

#include <SFCGAL/algorithm/force3D.h>
#include <SFCGAL/algorithm/orientation.h>

#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>

namespace SFCGAL {
namespace generator {

typedef Kernel::Point_2                    Point_2 ;
typedef Kernel::Point_3                    Point_3 ;
typedef CGAL::Polygon_2<Kernel>            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2 ;
typedef CGAL::Straight_skeleton_2<Kernel>  Straight_skeleton_2 ;

/**
 * @brief Basic building generator relying on a straight skeleton
 */
std::auto_ptr< Geometry > building(
	const Polygon & g,
	const Kernel::FT& wallHeight,
	const Kernel::FT& roofSlope
) ;
/**
 * @brief Basic building generator relying on a straight skeleton
 */
std::auto_ptr< Geometry > building(
	const MultiPolygon & g,
	const Kernel::FT& wallHeight,
	const Kernel::FT& roofSlope
) ;



void _buildingWall( const Polygon_2 & ring, const Kernel::FT & wallHeight, PolyhedralSurface & shell ){
	size_t npt = ring.size() ;
	for ( size_t i = 0; i < npt; i++ ){
		const Point_2 & a = ring.vertex( i ) ;
		const Point_2 & b = ring.vertex( (i+1) % npt ) ;

		LineString wallRing ;
		wallRing.addPoint( new Point( a.x(), a.y(), Kernel::FT(0) ) );
		wallRing.addPoint( new Point( b.x(), b.y(), Kernel::FT(0) ) );
		wallRing.addPoint( new Point( b.x(), b.y(), wallHeight ) );
		wallRing.addPoint( new Point( a.x(), a.y(), wallHeight ) );
		wallRing.addPoint( new Point( a.x(), a.y(), Kernel::FT(0) ) );
		shell.addPolygon( Polygon( wallRing ) );
	}
}


///
///
///
std::auto_ptr< Geometry > building(
	const Polygon & g,
	const Kernel::FT& wallHeight,
	const Kernel::FT& roofSlope
)
{
	typedef Straight_skeleton_2::Vertex_const_handle     Vertex_const_handle ;
	typedef Straight_skeleton_2::Halfedge_const_handle   Halfedge_const_handle ;
	typedef Straight_skeleton_2::Halfedge_const_iterator Halfedge_const_iterator ;
	typedef Straight_skeleton_2::Face_const_iterator     Face_const_iterator ;


	// convert to CGAL polygon and generate straight skeleton
	Polygon_with_holes_2 polygon = g.toPolygon_with_holes_2() ;

	// fix orientation
	algorithm::makeValidOrientation( polygon ) ;

	boost::shared_ptr< Straight_skeleton_2 > skeleton = CGAL::create_interior_straight_skeleton_2( polygon ) ;

	std::auto_ptr< PolyhedralSurface > shell( new PolyhedralSurface );
	// bottom part
	{
		Polygon bottom( polygon );
		bottom.reverse();
		algorithm::force3D( bottom );
		shell->addPolygon( bottom );
	}

	// walls
	{
		//exterior rings
		_buildingWall( polygon.outer_boundary(), wallHeight, *shell ) ;
		//interior rings
		for ( Polygon_with_holes_2::Hole_const_iterator it = polygon.holes_begin(); it != polygon.holes_end(); ++it ){
			_buildingWall( *it, wallHeight, *shell ) ;
		}
	}

	// roof
	{
		for ( Face_const_iterator it = skeleton->faces_begin(); it != skeleton->faces_end(); ++it ){

			LineString roofFaceRing ;
			Halfedge_const_handle h = it->halfedge(), done(h) ;
			bool infiniteTimeFound = false ;
			do {
				infiniteTimeFound = infiniteTimeFound || h->has_infinite_time() ;

				Point_2    point  = h->vertex()->point() ;
				Kernel::FT zPoint = wallHeight + h->vertex()->time() * roofSlope ;

				roofFaceRing.addPoint( Point( point.x(), point.y(), zPoint ) );

				h = h->next() ;
			} while ( h != done && ! infiniteTimeFound );

			if ( ! infiniteTimeFound ){
				roofFaceRing.addPoint( roofFaceRing.startPoint() );
				shell->addPolygon( Polygon( roofFaceRing ) );
			}
		}
	}

	return std::auto_ptr< Geometry >( new Solid( shell.release() ) );
}

///
///
///
std::auto_ptr< Geometry > building(
	const MultiPolygon & g,
	const Kernel::FT& wallHeight,
	const Kernel::FT& roofSlope
)
{
	std::auto_ptr< MultiSolid > multiSolid( new MultiSolid );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		multiSolid->addGeometry( building( g.polygonN(i), wallHeight, roofSlope ).release() );
	}
	return std::auto_ptr< Geometry >( multiSolid.release() );
}


///
///
///
std::auto_ptr< Geometry > building(
	const Geometry & g,
	const Kernel::FT& wallHeight,
	const Kernel::FT& roofSlope
)
{
	switch ( g.geometryTypeId() ){
	case TYPE_POLYGON:
		return building( g.as< Polygon >(), wallHeight, roofSlope ) ;
	case TYPE_MULTIPOLYGON:
		return building( g.as< MultiPolygon >(), wallHeight, roofSlope ) ;
	default:
		BOOST_THROW_EXCEPTION(Exception(
			( boost::format( "bad geometry type (%s) in generator::building" ) % g.geometryType() ).str()
		));
		return std::auto_ptr< Geometry >( new GeometryCollection() );
	}
}


} // namespace generator
} // namespace SFCGAL
