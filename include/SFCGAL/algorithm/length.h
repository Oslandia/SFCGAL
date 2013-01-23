#ifndef _SFCGAL_ALGORITHM_LENGTH_H_
#define _SFCGAL_ALGORITHM_LENGTH_H_


namespace SFCGAL {
	class Geometry ;
	class LineString ;
	class GeometryCollection ;
} // namespace SFCGAL

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief Get the 2D length for a LineString
	 */
	double length( const LineString & g ) ;
	/**
	 * @brief Get the 2D length for a GeometryCollection
	 */
	double length( const GeometryCollection & g ) ;
	/**
	 * @brief Get the 2D length for a geometry
	 */
	double length( const Geometry & g ) ;

	/**
	 * @brief Get the 3D length for a LineString
	 */
	double length3D( const LineString & g ) ;
	/**
	 * @brief Get the 3D length for a GeometryCollection
	 */
	double length3D( const GeometryCollection & g ) ;
	/**
	 * @brief Get the 2D length for a geometry
	 */
	double length3D( const Geometry & g ) ;

}//algorithm
}//SFCGAL


#endif
