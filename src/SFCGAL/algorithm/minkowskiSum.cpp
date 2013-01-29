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
#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/all.h>

#include <SFCGAL/detail/polygonSetToMultiPolygon.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/Aff_transformation_2.h>


typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2 ;
typedef CGAL::Polygon_set_2< SFCGAL::Kernel >        Polygon_set_2 ;


namespace SFCGAL {
namespace algorithm {

//-- private interface

/**
 * dispatch gA+gB sum
 */
void minkowskiSum( const Geometry& gA, const Polygon_2& gB, CGAL::Polygon_set_2< Kernel > & polygonSet ) ;
/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Point& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ) ;
/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const LineString& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ) ;
/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Polygon& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ) ;
/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Solid& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ) ;
/*
 * append gA+gB into the polygonSet
 */
void minkowskiSumCollection( const Geometry& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ) ;

//-- private interface implementation

///
///
///
void minkowskiSum( const Geometry& gA, const Polygon_2& gB, CGAL::Polygon_set_2< Kernel > & polygonSet )
{
	if ( gA.isEmpty() )
		return ;

	switch ( gA.geometryTypeId() ){
		case TYPE_POINT:
			return minkowskiSum( gA.as< Point >(), gB, polygonSet ) ;
		case TYPE_LINESTRING:
			return minkowskiSum( gA.as< LineString >(), gB, polygonSet ) ;
		case TYPE_POLYGON:
			return minkowskiSum( gA.as< Polygon >(), gB, polygonSet ) ;
		case TYPE_TRIANGLE:
			return minkowskiSum( gA.as< Triangle >().toPolygon(), gB, polygonSet ) ;
		case TYPE_SOLID:
			return minkowskiSum( gA.as< Solid >(), gB, polygonSet ) ;
		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_MULTISOLID:
		case TYPE_GEOMETRYCOLLECTION:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_POLYHEDRALSURFACE:
			return minkowskiSumCollection( gA, gB, polygonSet ) ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("minkowskiSum( %s, 'Polygon' ) is not defined")
			% gA.geometryType() ).str()
	));
}



/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Point& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){

	CGAL::Aff_transformation_2< Kernel > translate(
		CGAL::TRANSLATION,
		gA.toVector_2()
	);

	Polygon_2 sum ;
	for ( Polygon_2::Vertex_const_iterator it = gB.vertices_begin();
			it != gB.vertices_end(); ++it ){
		sum.push_back( translate.transform( *it ) );
	}

	if ( sum.is_clockwise_oriented() ){
		sum.reverse_orientation() ;
	}

	if ( polygonSet.is_empty() ){
		polygonSet.insert( sum );
	}else{
		polygonSet.join( sum );
	}
}


///
///
///
void minkowskiSum( const LineString& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){
	if ( gA.isEmpty() ){
		return ;
	}

	int npt = gA.numPoints() ;
	for ( int i = 0; i < npt - 1 ; i++ ){
		Polygon_2 P;
		P.push_back( gA.pointN(i).toPoint_2() );
		P.push_back( gA.pointN(i+1).toPoint_2() );

		Polygon_with_holes_2 part = minkowski_sum_2( P, gB );

		// merge into a polygon set
		if ( polygonSet.is_empty() ){
			polygonSet.insert( part );
		}else{
			polygonSet.join( part );
		}
	}
}

///
///
///
void minkowskiSum( const Polygon& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){
	if ( gA.isEmpty() ){
		return ;
	}

	/*
	 * Invoke minkowski_sum_2 for exterior ring
	 */
	{
		Polygon_with_holes_2 sum = minkowski_sum_2( gA.exteriorRing().toPolygon_2(), gB ) ;
		if ( polygonSet.is_empty() ){
			polygonSet.insert( sum );
		}else{
			polygonSet.join( sum );
		}
	}

	/*
	 * Compute the Minkowski sum for each segment of the interior rings
	 * and perform the union of the result. The result is a polygon, and its holes
	 * correspond to the inset.
	 *
	 */
	if ( gA.hasInteriorRings() ){
		Polygon_set_2 sumInteriorRings ;
		for ( size_t i = 0; i < gA.numInteriorRings(); i++ ){
			minkowskiSum( gA.interiorRingN(i), gB, sumInteriorRings ) ;
		}

		/*
		 * compute the difference for each hole of the resulting polygons
		 */
		std::list<Polygon_with_holes_2> interiorPolygons ;
		sumInteriorRings.polygons_with_holes( std::back_inserter(interiorPolygons) ) ;
		for ( std::list<Polygon_with_holes_2>::iterator it_p = interiorPolygons.begin();
				it_p != interiorPolygons.end(); ++it_p ){

			for ( Polygon_with_holes_2::Hole_iterator it_hole = it_p->holes_begin();
					it_hole != it_p->holes_end(); ++it_hole ){

				it_hole->reverse_orientation() ;
				polygonSet.difference( *it_hole ) ;
			} // foreach hole
		}// foreach polygon
	}
}

///
///
///
void minkowskiSum( const Solid& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet )
{
	//use only the projection of exterior shell
	minkowskiSumCollection( gA.exteriorShell(), gB, polygonSet );
}


///
///
///
void minkowskiSumCollection( const Geometry& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet )
{
	for ( size_t i = 0; i < gA.numGeometries(); i++ ){
		minkowskiSum( gA.geometryN(i), gB, polygonSet );
	}
}


//-- public interface implementation

///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const Geometry& gA, const Polygon& gB )
{
	Polygon_set_2 polygonSet ;
	minkowskiSum( gA, gB.toPolygon_2(), polygonSet ) ;
	return detail::polygonSetToMultiPolygon( polygonSet ) ;
}

} // namespace algorithm
} // namespace SFCGAL
