#ifndef _SFCGAL_SERIALIZATION_
#define _SFCGAL_SERIALIZATION_

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

#include <boost/assert.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace SFCGAL {
namespace io {

	class BinarySerializer : public boost::archive::binary_oarchive
	{
	public:
		BinarySerializer( std::ostream& ostr );
	};

	class BinaryUnserializer : public boost::archive::binary_iarchive
	{
	public:
		BinaryUnserializer( std::istream& istr );
	};

	/**
	 * Convert a Geometry to its binary representation
	 * @warning resulting string may contain 0s
	 */
	std::string writeBinary( const SFCGAL::Geometry& );

	/**
	 * Read a Geometry from a binary representation
	 */
	std::auto_ptr<SFCGAL::Geometry> readBinary( const std::string& );
}
}

namespace boost {
namespace serialization {

	/**
	 * Serialization of Gmpz for text archives
	 */
	void save( boost::archive::text_oarchive & ar, const CGAL::Gmpz & z, const unsigned int version);

	/**
	 * Serialization of Gmpz for binary archives
	 */
	void save ( boost::archive::binary_oarchive & ar, const CGAL::Gmpz & z, const unsigned int version);

	/**
	 * Unserialization of Gmpz for text archives
	 */
	void load( boost::archive::text_iarchive & ar, CGAL::Gmpz & z, const unsigned int version);

	/**
	 * Unserialization of Gmpz for binary archives
	 */
	void load( boost::archive::binary_iarchive & ar, CGAL::Gmpz & z, const unsigned int version);

	template<class Archive>
	void serialize(Archive & ar, CGAL::Gmpz & z, const unsigned int version)
	{
		split_free( ar, z, version );
	}
	
	/**
	 * Serializer of Gmpq
	 */
	template<class Archive>
	void save(Archive & ar, const CGAL::Gmpq & q, const unsigned int version)
	{
		CGAL::Gmpz n = q.numerator();
		CGAL::Gmpz d = q.denominator();
		ar & n;
		ar & d;
	}
	
	/**
	 * Unserializer of Gmpq
	 */
	template<class Archive>
	void load(Archive & ar, CGAL::Gmpq & q, const unsigned int version)
	{
		CGAL::Gmpz n;
		CGAL::Gmpz d;
		ar & n;
		ar & d;
		q = CGAL::Gmpq( n, d );
	}
	template<class Archive>
	void serialize(Archive & ar, CGAL::Gmpq & q, const unsigned int version)
	{
		split_free( ar, q, version );
	}
	

	/**
	 * Serializer of Kernel::FT
	 */
	template<class Archive>
	void save(Archive & ar, const SFCGAL::Kernel::FT & q, const unsigned int version)
	{
		SFCGAL::Kernel::Exact_kernel::FT eq = CGAL::exact( q );
		ar << eq;
	}
	
	/**
	 * Unserializer of Kernel::FT
	 */
	template<class Archive>
	void load(Archive & ar, SFCGAL::Kernel::FT & q, const unsigned int version)
	{
		SFCGAL::Kernel::Exact_kernel::FT eq;
		ar >> eq;
		q = eq;
	}
	template<class Archive>
	void serialize(Archive & ar, SFCGAL::Kernel::FT & q, const unsigned int version)
	{
		split_free( ar, q, version );
	}

}
}

#endif
