/**
 *
 * @mainpage SFCGAL
 *
 * SFCGAL is a wrapper around CGAL library that intents to implement 2D and 3D
 * operations on OGC standards models (Simple Feature Access, CityGML,...)
 *
 * @section intro_sec Introduction
 *
 * SFCGAL rely on the a single CGAL Kernel : The Exact Predicate Exact Construction Kernel (Epeck). SFCGAL::Kernel is a typedef
 * on Epeck on this Kernel.
 *
 * SFCGAL::Coordinate class provides a type erasure on CGAL points in order to fit the dynamic is3D() world. SFCGAL::Coordinate
 * hides either a dummy empty entity, either a CGAL::Point_2, or a CGAL::Point_3 in the Epeck kernel.
 *
 * Furthermore, conversions to double that would break constructive robustness are avoided.
 *
 * @section dependencies_sec Dependencies
 *
 * <ul>
 * 		<li>CGAL 4.1+ (http://www.cgal.org/)</li>
 * 		<li>Boost library (http://www.boost.org/)</li>
 * </ul>
 *
 * @section geometric_model_sec Geometric Model
 *
 * The Geometric Model is an extension of OGC Simple Feature Access providing solids (SFCGAL::Solid) defined
 * in the ISO 19107 and implemented in CityGML.
 *
 * Primitives
 *
 * <ul>
 * 		<li>SFCGAL::Point is a Point defined by SFCGAL::Coordinate</li>
 * 		<li>SFCGAL::LineString is a defined by an ordered list of SFCGAL::Point</li>
 * 		<li>SFCGAL::Polygon is a polygon with optional holes, defined by an exterior ring and interior rings</li>
 * 		<li>SFCGAL::Triangle is a Triangle defined by 3 points (*)</li>
 *		<li>SFCGAL::Solid is a solid with optional holes, defined by an exterior shell and interior shells</li>
 * </ul>
 *
 * Geometry collections
 *
 * <ul>
 * 		<li>SFCGAL::GeometryCollection is a list of geometries that might mix types</li>
 * 		<li>SFCGAL::MultiPoint is GeometryCollection restricted to SFCGAL::Point</li>
 * 		<li>SFCGAL::MultiLineString is GeometryCollection restricted to SFCGAL::LineString</li>
 * 		<li>SFCGAL::MultiPolygon is GeometryCollection restricted to SFCGAL::Polygon</li>
 * </ul>
 *
 * Geometries with shared coordinates
 *
 * <ul>
 * 		<li>SFCGAL::TriangulatedSurface is a triangle set with shared components</li>
 * 		<li>SFCGAL::PolyhedralSurface is a polygon set with shared components</li>
 * </ul>
 *
 *
 * (*) SFCGAL doesn't respect the standard on this point. SFCGAL::Triangle doesn't inherit SFCGAL::Polygon for different reasons
 * <ul>
 *		<li>Library is not query-only, geometries can be edited. We should expose triangle.addInteriorRing and throws exceptions on it.</li>
 * 		<li>Inherits Polygon would brought useless structure to handle exterior and interior rings</li>
 *		<li>CGAL model relies on 3 points</li>
 * </ul>
 * On the other hand, it leads to convert between Triangle and Polygon and to duplicate some codes (area for example) so that it could to change
 * that and conform.
 *
 * @defgroup public_api Public C++ API
 * @defgroup capi Public C API
 *
 * @defgroup detail Implementation details
 * 
 * @defgroup incomplete Incomplete or buggy functions
 *
 *
 */
