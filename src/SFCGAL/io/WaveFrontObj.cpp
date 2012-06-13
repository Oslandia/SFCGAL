#include <SFCGAL/io/WaveFrontObj.h>

#include <sstream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>


#include <SFCGAL/algorithm/triangulate.h>


#undef SFCGAL_IO_WAVEFRONTOBJ_VERBOSE



namespace SFCGAL {
namespace io {

///
///
///
WaveFrontFaceItem::WaveFrontFaceItem( int v_, int vt_, int vn_ ):
	v(v_),
	vt(vt_),
	vn(vn_)
{

}

///
///
///
WaveFrontFaceItem::WaveFrontFaceItem( const std::string & str )
{
	std::vector< std::string > parts ;
	boost::algorithm::split( parts, str, boost::is_any_of("/") );

	v  = ( str.size() > 0 ? boost::lexical_cast< int >( str[0] ) : -1 ) ;
	vt = ( str.size() > 1 ? boost::lexical_cast< int >( str[1] ) : -1 ) ;
	vn = ( str.size() > 2 ? boost::lexical_cast< int >( str[2] ) : -1 ) ;
}

///
///
///
std::string WaveFrontFaceItem::toString() const
{
	std::ostringstream oss;
	oss << ( v + 1 ) ;

	if ( vt >= 0 || vn >= 0 ){
		oss << "/" ;
		if ( vt >= 0 ){
			oss << ( vt + 1 ) ;
		}
		oss << "/" ;
		if ( vn >= 0 ){
			oss << ( vn + 1 ) ;
		}
	}
	return oss.str() ;
}


///
///
///
WaveFrontObj::WaveFrontObj()
{

}

///
///
///
void WaveFrontObj::clear()
{
	_vertices.clear();
	_faces.clear();
}


///
///
///
void WaveFrontObj::load( const std::string & filename )
{
	clear();

	std::ifstream ifs( filename.c_str() );
	if ( ! ifs.good() ){
		BOOST_THROW_EXCEPTION( Exception( ( boost::format("can't open file '%1%'")%filename ).str() ) );
	}

	std::string line ;
	while ( std::getline( ifs, line ) ){
		_loadLine( line );
	}
	ifs.close();
}

///
///
///
size_t WaveFrontObj::addVertex( const Point & p )
{
	size_t id = _vertices.size();
	_vertices.push_back( p );
	return id ;
}

///
///
///
size_t WaveFrontObj::addFace( const WaveFrontFace & face )
{
	size_t id = _faces.size();
	_faces.push_back( face );
	return id ;
}


///
///
///
void   WaveFrontObj::addGeometry( const Polygon & polygon )
{
	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( polygon, triangulatedSurface );
	addGeometry( triangulatedSurface );
}

///
///
///
void   WaveFrontObj::addGeometry( const MultiPolygon & polygon )
{
	for ( size_t i = 0; i < polygon.numGeometries(); i++ ){
		addGeometry( polygon.geometryN(i).as< Polygon >() );
	}
}

///
///
///
void WaveFrontObj::addGeometry( const TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < triangulatedSurface.numTriangles(); i++ ){
		const Triangle & triangle = triangulatedSurface.triangleN(i) ;

		WaveFrontFace face ;
		for ( size_t j = 0; j < 3; j++ ){
			size_t v = addVertex( triangle.vertex(j) ) ;
			face.push_back( WaveFrontFaceItem( v ) );
		}
		addFace( face );
	}
}

///
///
///
void WaveFrontObj::save( const std::string & filename ) const
{
	std::ofstream ofs( filename.c_str() );
	if ( ! ofs.good() ){
		std::string message = ( boost::format( "can't write file : '%1%'" ) % filename ).str() ;
		BOOST_THROW_EXCEPTION( Exception( message ) );
	}

	/*
	 * write vertices
	 */
	for ( size_t i = 0; i < _vertices.size(); i++ ){
		const Point & vertex = _vertices[i] ;
		ofs << "v " << vertex.x() << " " << vertex.y() << " " << ( vertex.is3D() ? vertex.z() : 0.0 ) << std::endl ;
	}

	/*
	 * write faces
	 */
	for ( size_t i = 0; i < _faces.size(); i++ ){
		const WaveFrontFace & face = _faces[i] ;
		ofs << "f" ;
		for ( size_t j = 0; j < face.size(); j++ ){
			// note : 1-indexed
			ofs << " " << face[j].toString() ;
		}
		ofs << std::endl;
	}
}

///
///
///
void WaveFrontObj::_loadLine( const std::string & line )
{
	std::string trimed = boost::algorithm::trim_copy( line ) ;
	if ( trimed.empty() ){
		return ;
	}
	std::istringstream iss( trimed ) ;

	std::string lineType ;
	if ( ! ( iss >> lineType ) ){
		std::string message = ( boost::format("fail to read line type in : %1%") % line ).str() ;
		BOOST_THROW_EXCEPTION( Exception( message ) );
	}

	if ( lineType == "v" ){
		Point p ;
		iss >> p.x() >> p.y() >> p.z() ;
		_vertices.push_back( p ) ;

#ifdef SFCGAL_IO_WAVEFRONTOBJ_VERBOSE
		std::cout << "load point : " << p.asText() << std::endl;
#endif

	}else if ( lineType == "f" ){
		WaveFrontFace face ;
		size_t idx ;

		while ( ( iss >> idx ) ){
			BOOST_ASSERT( idx - 1 < _vertices.size() );
			face.push_back( WaveFrontFaceItem(idx - 1) );
		}

#ifdef SFCGAL_IO_WAVEFRONTOBJ_VERBOSE
		std::cout << "load face : " ;
		for ( size_t i = 0; i < face.size(); i++ ){
			std::cout << face[i] << " ";
		}
		std::cout << std::endl;
#endif

		_faces.push_back( face );
	}else{
#ifdef SFCGAL_IO_WAVEFRONTOBJ_VERBOSE
		std::cout << "unknown line type : " << lineType << std::endl;
#endif
	}
}





}//io
}//SFCGAL



