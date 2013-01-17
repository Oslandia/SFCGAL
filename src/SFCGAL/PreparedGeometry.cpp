#include <SFCGAL/PreparedGeometry.h>

#include <SFCGAL/io/WktWriter.h>

namespace SFCGAL
{
	PreparedGeometry::PreparedGeometry() :
		_srid( 0 )
	{
	}

	PreparedGeometry::PreparedGeometry( std::auto_ptr<Geometry> geometry, srid_t srid ) :
		_geometry( geometry ),
		_srid( srid )
	{
	}

	PreparedGeometry::PreparedGeometry( Geometry* geometry, srid_t srid ) :
		_geometry( geometry ),
		_srid( srid )
	{
	}

	PreparedGeometry::~PreparedGeometry()
	{
	}

	const Geometry& PreparedGeometry::geometry() const {
		BOOST_ASSERT( _geometry.get() );
		return *_geometry;
	}

	Geometry& PreparedGeometry::geometry() {
		BOOST_ASSERT( _geometry.get() );
		return *_geometry;
	}

	void PreparedGeometry::resetGeometry( Geometry* geom ) {
		_geometry.reset( geom );
		invalidateCache();
	}

	const Envelope& PreparedGeometry::envelope() const
	{
		if ( ! _envelope ) {
			_envelope.reset( _geometry->envelope() );
		}
		return *_envelope;
	}

	void PreparedGeometry::invalidateCache()
	{
		_envelope.reset();
	}

	std::string PreparedGeometry::asEWKT( const int& numDecimals ) const
	{
		std::ostringstream oss;
		if ( numDecimals >= 0 ){
			oss << std::fixed ;
			oss.precision( numDecimals );
		}

		if ( _srid != 0 ) {
			oss << "SRID=" << _srid << ";";
		}

		io::WktWriter writer( oss );
		bool exactWrite = false;
		if ( numDecimals == -1 ) {
			exactWrite = true;
		}
		writer.write( *_geometry, exactWrite );
		return oss.str();		
	}
}
