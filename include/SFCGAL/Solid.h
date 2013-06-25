/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_SOLID_H_
#define _SFCGAL_SOLID_H_

#include <vector>
#include <boost/assert.hpp>

#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

#include <SFCGAL/PolyhedralSurface.h>

#include <CGAL/Nef_polyhedron_3.h>

namespace SFCGAL {

	/**
	 * A Solid modeled with an exteriorShell and interiorShells materialized by PolyhedralSurface.
	 * @ingroup public_api
	 * @note A shell is supposed to be close.
	 * @warning GM_Solid, from ISO 19107 is defined in CityGML, but not in SFA. Without Solid concept,
	 * @note Volum concept is missing.
	 * @todo template < size_t N >?
	 */
	class SFCGAL_API Solid : public Geometry {
	public:
		typedef boost::ptr_vector< PolyhedralSurface >::iterator       iterator ;
		typedef boost::ptr_vector< PolyhedralSurface >::const_iterator const_iterator ;

		/**
		 * Empty Solid constructor
		 */
		Solid() ;
		/**
		 * Constructor with an exterior shell
		 */
		Solid( const PolyhedralSurface & exteriorShell ) ;
		/**
		 * Constructor with an exterior shell (takes ownership)
		 */
		Solid( PolyhedralSurface * exteriorShell ) ;
		/**
		 * Constructor with a vector of shells (PolyhedralSurface)
		 */
		Solid( const std::vector< PolyhedralSurface > & shells ) ;
		/**
		 * Copy constructor
		 */
		Solid( const Solid& other ) ;
		/**
		 * assign operator
		 */
		Solid& operator = ( Solid other ) ;
		/**
		 * destructor
		 */
		~Solid() ;

		//-- SFCGAL::Geometry
		virtual Solid *        clone() const ;

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
		//-- SFCGAL::Geometry
		virtual bool           isMeasured() const ;

		/**
		 * Returns the exterior shell
		 */
		inline const PolyhedralSurface &    exteriorShell() const { return _shells[0] ; }
		/**
		 * Returns the exterior shell
		 */
		inline PolyhedralSurface &          exteriorShell() { return _shells[0] ; }

		/**
		 * Returns the number of interior shells
		 */
		inline size_t                       numInteriorShells() const { return _shells.size() - 1 ; }
		/**
		 * Returns the n-th interior shell
		 */
		inline const PolyhedralSurface  &   interiorShellN( size_t const& n ) const { return _shells[n+1]; }
		/**
		 * Returns the n-th interior shell
		 */
		inline PolyhedralSurface &          interiorShellN( size_t const& n ) { return _shells[n+1]; }
		/**
		 * add a polygon to the PolyhedralSurface
		 */
		inline void                         addInteriorShell( const PolyhedralSurface & shell )
		{
			_shells.push_back( shell.clone() );
		}
		/**
		 * add a polygon to the PolyhedralSurface
		 */
		inline void                         addInteriorShell( PolyhedralSurface * shell )
		{
			BOOST_ASSERT( shell != NULL );
			_shells.push_back( shell );
		}

		/**
		 * Returns the number of shells
		 */
		inline size_t  numShells() const {
			return _shells.size() ;
		}
		/**
		 * Returns the n-th shell, 0 is exteriorShell
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline const PolyhedralSurface &  shellN( const size_t & n ) const {
			BOOST_ASSERT( n < numShells() );
			return _shells[n];
		}
		/**
		 * Returns the n-th shell, 0 is exteriorShell
		 * @warning not standard, avoid conditionnal to access rings
		 */
		inline PolyhedralSurface &        shellN( const size_t & n ) {
			BOOST_ASSERT( n < numShells() );
			return _shells[n];
		}

		//-- iterators

		inline iterator       begin() { return _shells.begin() ; }
		inline const_iterator begin() const { return _shells.begin() ; }

		inline iterator       end() { return _shells.end() ; }
		inline const_iterator end() const { return _shells.end() ; }


		//-- helpers

		/**
		 * Convert to Nef_polyhedron_3
		 */
		template < typename K >
		CGAL::Nef_polyhedron_3<K> toNef_polyhedron_3() const
		{
			typedef CGAL::Polyhedron_3<K> Polyhedron;
			CGAL::Nef_polyhedron_3<K> nef;
			// Convert each shell of the solid to a polyhedron_3
			// Then build a Nef_polyhedron by substraction of interior shells
			TriangulatedSurface ext_tri;
			triangulate::triangulatePolygon3D( this->exteriorShell(), ext_tri );
			
			std::auto_ptr<Polyhedron> poly( ext_tri.toPolyhedron_3<K, Polyhedron>());
			nef = CGAL::Nef_polyhedron_3<K>( *poly );
			
			for ( size_t i = 0; i < this->numInteriorShells(); i++ ) {
				TriangulatedSurface tri;
				triangulate::triangulatePolygon3D( this->interiorShellN(i), tri );
				
				std::auto_ptr<Polyhedron> poly( tri.toPolyhedron_3<K, Polyhedron>());
				CGAL::Nef_polyhedron_3<K> lnef( *poly );
				
				// substract the hole from the global nef
				// WARNING: interior shells are supposed to have an opposite orientation
				// to the exterior shell's one. In this case, we should use the intersection operator
				nef = nef - lnef;
			}
			return nef;
		}


		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

		/**
		 * Serializer
		 */
		template <class Archive>
		void serialize( Archive& ar, const unsigned int /*version*/ )
		{
			ar & boost::serialization::base_object<Geometry>(*this);
			ar & _shells;
		}
	private:
		boost::ptr_vector< PolyhedralSurface > _shells ;

        void swap( Solid & other )
        {
            _shells.swap( other._shells );
        }
	};


}

#endif
