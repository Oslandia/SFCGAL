#include <SFCGAL/all.h>
#include <SFCGAL/io/Serialization.h>

namespace SFCGAL {
namespace io {

	BinarySerializer::BinarySerializer( std::ostream& ostr ) : boost::archive::binary_oarchive( ostr )
	{
		using namespace SFCGAL;
		register_type< Point >();
		register_type< LineString >();
		register_type< Triangle >();
		register_type< Polygon >();
		register_type< TriangulatedSurface >();
		register_type< PolyhedralSurface >();
		register_type< Solid >();
		register_type< GeometryCollection >();
		register_type< MultiPoint >();
		register_type< MultiLineString >();
		register_type< MultiPolygon >();
		register_type< MultiSolid >();
	}

	BinaryUnserializer::BinaryUnserializer( std::istream& ostr ) : boost::archive::binary_iarchive( ostr )
	{
		using namespace SFCGAL;
		register_type< Point >();
		register_type< LineString >();
		register_type< Triangle >();
		register_type< Polygon >();
		register_type< TriangulatedSurface >();
		register_type< PolyhedralSurface >();
		register_type< Solid >();
		register_type< GeometryCollection >();
		register_type< MultiPoint >();
		register_type< MultiLineString >();
		register_type< MultiPolygon >();
		register_type< MultiSolid >();
	}

	///
	///
	///
	std::string writeBinaryGeometry( const Geometry& g )
	{
		std::ostringstream ostr;
		BinarySerializer arc( ostr );
		// use the pointer version to force dynamic type writing
		const Geometry* pg = &g;
		arc << pg;
		return ostr.str();
	}

	///
	///
	///
	std::string writeBinaryPrepared( const PreparedGeometry& g )
	{
		std::ostringstream ostr;
		BinarySerializer arc( ostr );
		const PreparedGeometry* pg = &g;
		arc << pg;
		return ostr.str();
	}

	///
	///
	///
	std::auto_ptr<Geometry> readBinaryGeometry( const std::string& str )
	{
		std::istringstream istr( str );
		BinaryUnserializer iarc( istr );
		Geometry* g;
		iarc >> g;
		return std::auto_ptr<Geometry>( g );
	}

	///
	///
	///
	std::auto_ptr<PreparedGeometry> readBinaryPrepared( const std::string& str )
	{
		std::istringstream istr( str );
		BinaryUnserializer iarc( istr );
		PreparedGeometry* pg;
		iarc >> pg;
		return std::auto_ptr<PreparedGeometry>( pg );
	}
}
}
namespace boost {
namespace serialization {

	void save( boost::archive::text_oarchive & ar, const CGAL::Gmpz & z, const unsigned int version)
	{
		std::ostringstream ostr;
		ostr << z;
		std::string str = ostr.str();
		ar << str;
	}

	// specialization for binary archives
	void save ( boost::archive::binary_oarchive & ar, const CGAL::Gmpz & z, const unsigned int version)
	{
		const mpz_t& mpz = z.mpz();
		int32_t size = mpz->_mp_size;
		ar & size;
		uint32_t rsize = size >= 0 ? size : -size;
		for ( uint32_t i = 0; i < rsize; ++i ) {
			ar & mpz->_mp_d[i];
		}
	}


	void load( boost::archive::text_iarchive & ar, CGAL::Gmpz & z, const unsigned int version)
	{
		std::string line;
		ar >> line;
		std::istringstream istr( line );
		istr >> z;
	}

	void load( boost::archive::binary_iarchive & ar, CGAL::Gmpz & z, const unsigned int version)
	{
		int32_t size;
		uint32_t rsize;
		mpz_t& mpz = z.mpz();
		ar & size;
		rsize = size >= 0 ? size : -size;
		mpz->_mp_size = size;
		_mpz_realloc( mpz, rsize );
		uint32_t i;
		for ( i = 0; i < rsize; ++i ) {
			ar & mpz->_mp_d[i];
		}
	}
		
}
}
