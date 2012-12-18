#ifndef _SFCGAL_SERIALIZATION_
#define _SFCGAL_SERIALIZATION_

#include <SFCGAL/Kernel.h>

#include <boost/serialization/split_free.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// TODO : deduce point type from Kernel
// The ExactPoint type is the exact type, but without any 'lazy' part
typedef CGAL::Point_2<CGAL::Simple_cartesian<CGAL::Gmpq> > ExactPoint2;
typedef CGAL::Point_3<CGAL::Simple_cartesian<CGAL::Gmpq> > ExactPoint3;

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
	 * Serializer of Point_2
	 */
	template<class Archive>
		void save(Archive & ar, const SFCGAL::Kernel::Point_2 & pt, const unsigned int version)
	{
		// tree evaluation here
		ExactPoint2 exactp = CGAL::exact( pt );
		ar << exactp.x();
		ar << exactp.y();
	}
	/**
	 * Unserializer of Point_2
	 */
	template<class Archive>
		void load(Archive & ar, SFCGAL::Kernel::Point_2 & pt, const unsigned int version)
	{
		CGAL::Gmpq x, y;
		ar >> x >> y;
		ExactPoint2 exactp( x, y );
		pt = SFCGAL::Kernel::Point_2( exactp );
	}
	template<class Archive>
		void serialize(Archive & ar, SFCGAL::Kernel::Point_2 & pt, const unsigned int version)
	{
		split_free( ar, pt, version );
	}

	/**
	 * Serializer of Point_3
	 */
	template<class Archive>
		void save(Archive & ar, const SFCGAL::Kernel::Point_3 & pt, const unsigned int version)
	{
		// tree evaluation here
		ExactPoint3 exactp = CGAL::exact( pt );
		ar << exactp.x();
		ar << exactp.y();
	}

	/**
	 * Unserializer of Point_3
	 */
	template<class Archive>
		void load(Archive & ar, SFCGAL::Kernel::Point_3 & pt, const unsigned int version)
	{
		CGAL::Gmpq x, y;
		ar >> x >> y;
		ExactPoint3 exactp( x, y );
		pt = SFCGAL::Kernel::Point_2( exactp );
	}
	template<class Archive>
		void serialize(Archive & ar, SFCGAL::Kernel::Point_3 & pt, const unsigned int version)
	{
		split_free( ar, pt, version );
	}
}
}

#endif
