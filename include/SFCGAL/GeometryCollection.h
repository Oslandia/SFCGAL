#ifndef _SFCGAL_GEOMETRYCOLLECTION_H_
#define _SFCGAL_GEOMETRYCOLLECTION_H_

#include <vector>
#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

	/**
	 * A GeometryCollection in SFA.
	 */
	class GeometryCollection : public Geometry {
	public:
		typedef boost::ptr_vector< Geometry >::iterator       iterator ;
		typedef boost::ptr_vector< Geometry >::const_iterator const_iterator ;

		/**
		 * Empty GeometryCollection constructor
		 */
		GeometryCollection() ;
		/**
		 * Copy constructor
		 */
		GeometryCollection( GeometryCollection const& other ) ;
		/**
		 * assign operator
		 */
		GeometryCollection& operator = ( const GeometryCollection & other ) ;
		/**
		 * destructor
		 */
		virtual ~GeometryCollection() ;

		//-- SFCGAL::Geometry
		virtual GeometryCollection *   clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string    geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType   geometryTypeId() const ;
		//-- SFCGAL::Geometry
		virtual int            dimension() const ;
		//-- SFCGAL::Geometry
		virtual int            coordinateDimension() const ;
		//-- SFCGAL::Geometry
		virtual bool           isEmpty() const ;
		//-- SFCGAL::Geometry
		virtual bool           is3D() const ;


		/**
		 * [SFA/OGC]Returns the number of geometries
		 */
		inline size_t             numGeometries() const { return _geometries.size(); }
		/**
		 * [SFA/OGC]Returns the n-th geometry
		 */
		inline const Geometry  &  geometryN( size_t const& n ) const { return _geometries[n]; }
		/**
		 * [SFA/OGC]Returns the n-th geometry
		 */
		inline Geometry &         geometryN( size_t const& n ) { return _geometries[n]; }

		/**
		 * [SFA/OGC]add a geometry to the collection (takes ownership)
		 */
		void                      addGeometry( Geometry * geometry ) ;
		/**
		 * [SFA/OGC]add a geometry to the collection (clone instance)
		 */
		void                      addGeometry( Geometry const& geometry ) ;

		//-- iterators

		inline iterator       begin() {
			return _geometries.begin() ;
		}
		inline const_iterator begin() const {
			return _geometries.begin() ;
		}

		inline iterator       end() {
			return _geometries.end() ;
		}
		inline const_iterator end() const {
			return _geometries.end() ;
		}



		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

	private:
		boost::ptr_vector< Geometry > _geometries ;

	protected:
		/**
		 * Test if a geometry in the collection
		 */
		virtual bool           isAllowed( Geometry const& g ) ;


		/**
		 * Assign other collection
		 */
		void  assign( const GeometryCollection & other ) ;
	};


}

#endif
