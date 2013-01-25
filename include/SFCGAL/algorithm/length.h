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
	 * @brief Compute the 2D length for a Geometry (0 for incompatible types)
	 */
	double length( const Geometry & g ) ;
	/**
	 * @brief Compute the 2D length for a LineString
	 */
	double length( const LineString & g ) ;
	/**
	 * @brief Compute the 2D length for a GeometryCollection
	 */
	double length( const GeometryCollection & g ) ;

	/**
	 * @brief Compute the 2D length for a geometry
	 * @return the length of the Geometry, 0 for incompatible types
	 */
	double length3D( const Geometry & g ) ;
	/**
	 * @brief Compute the 3D length for a LineString
	 */
	double length3D( const LineString & g ) ;
	/**
	 * @brief Compute the 3D length for a GeometryCollection
	 */
	double length3D( const GeometryCollection & g ) ;


}//algorithm
}//SFCGAL


#endif
