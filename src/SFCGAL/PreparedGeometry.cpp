#include <SFCGAL/PreparedGeometry.h>

#include <SFCGAL/io/WktWriter.h>

namespace SFCGAL
{
	PreparedGeometry::PreparedGeometry( std::auto_ptr<Geometry> geometry, srid_t srid ) :
		_geometry( geometry ),
		_srid( srid ),
		_is_envelope_valid( false )
	{
	}

	PreparedGeometry::PreparedGeometry( Geometry* geometry, srid_t srid ) :
		_geometry( geometry ),
		_srid( srid ),
		_is_envelope_valid( false )
	{
	}

	PreparedGeometry::~PreparedGeometry()
	{
	}

	const Envelope& PreparedGeometry::envelope() const
	{
		if ( !_is_envelope_valid ) {
			_envelope = _geometry->envelope();
			_is_envelope_valid = true;
		}
		return _envelope;
	}

	std::string PreparedGeometry::asEWKT( const int& numDecimals ) const
	{
		std::ostringstream oss;
		if ( numDecimals >= 0 ){
			oss << std::fixed ;
			oss.precision( numDecimals );
		}

		oss << "SRID=" << _srid << ";";

		io::WktWriter writer( oss );
		bool exactWrite = false;
		if ( numDecimals == -1 ) {
			exactWrite = true;
		}
		writer.write( *_geometry, exactWrite );
		return oss.str();		
	}
}
