#include <SFCGAL/io/Serialization.h>

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
		ar & mpz->_mp_size;
		for ( int32_t i = 0; i < mpz->_mp_size; ++i ) {
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
		mpz_t& mpz = z.mpz();
		ar & size;
		_mpz_realloc( mpz, size );
		mpz->_mp_size = size;
		int32_t i;
		for ( i = 0; i < size; ++i ) {
		       ar & mpz->_mp_d[i];
		}
	}
		
}
}
