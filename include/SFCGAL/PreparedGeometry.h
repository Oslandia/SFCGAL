#ifndef _SFCGAL_PREPARED_GEOMETRY_H_
#define _SFCGAL_PREPARED_GEOMETRY_H_

#include <SFCGAL/Envelope.h>

#include <boost/noncopyable.hpp>

#include <stdint.h> // uint32_t

namespace SFCGAL {

class Geometry;

typedef uint32_t srid_t;

/**
 * A PreparedGeometry is a shell around a SFCGAL::Geometry.
 * It is used to store annex data, like SRID or cached computations
 *
 * It is noncopyable since it stores a std::auto_ptr<SFCGAL::Geometry>
 */
class PreparedGeometry : public boost::noncopyable
{
 public:
	/**
	 * Cosntructor
	 * @geometry pointer to the underlying SFCGAL::Geometry. Takes ownership
	 */
	PreparedGeometry( std::auto_ptr<Geometry> geometry, srid_t srid = 0 );

	/**
	 * Cosntructor
	 * @geometry pointer to the underlying SFCGAL::Geometry. Takes ownership
	 */
	PreparedGeometry( Geometry* geometry, srid_t srid = 0 );

	virtual ~PreparedGeometry();

	/**
	 * Geometry accessor
	 */
	const Geometry& geometry() const { return *_geometry; }
	Geometry& geometry() { return *_geometry; }

	/**
	 * SRID read only accessor
	 */
	const srid_t& SRID() const { return _srid; }

	/**
	 * SRID accessor
	 */
	srid_t& SRID() { return _srid; }

	/**
	 * Envelope accessor (using cache)
	 */
	const Envelope& envelope() const;

	/**
	 * Is the envelope valid
	 */
	bool isEnvelopeValid() const { return _is_envelope_valid; }

	/**
	 * Resets the envelope cache
	 */
	void invalidateEnvelope() { _is_envelope_valid = false; }

	/**
	 * Convert to an extended WKT (with SRID)
	 * @numDecimals: number of decimals, -1 for keeping the exact rational representation, if possible
	 */
	std::string asEWKT( const int& numDecimals = - 1) const;


protected:
	// Pointer to underlying Geometry
	std::auto_ptr<Geometry> _geometry;

	// SRID of the geometry
	srid_t _srid;

	// bbox of the geometry
	mutable Envelope _envelope;
	// Envelope dirty flag. When accessing to the envelope, it is not recomputed if this flag is set to true
	mutable bool _is_envelope_valid;
};

}

#endif
