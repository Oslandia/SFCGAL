/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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

#ifndef _SFCGAL_DETAIL_GEOMETRY_SET_H_
#define _SFCGAL_DETAIL_GEOMETRY_SET_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/variant.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/TypeForDimension.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Box_intersection_d/Box_with_handle_d.h>

namespace SFCGAL {
	class Geometry;
namespace detail {

	enum PrimitiveType
	{
		PrimitivePoint,
		PrimitiveSegment,
		PrimitiveSurface,
		PrimitiveVolume
	};

	template <int Dim>
	struct PrimitiveHandle
	{
		typedef boost::variant< const typename TypeForDimension<Dim>::Point*,
					const typename TypeForDimension<Dim>::Segment*,
					const typename TypeForDimension<Dim>::Surface*,
					const typename TypeForDimension<Dim>::Volume* > Type;
		Type handle;

		template <class T>
		PrimitiveHandle( const T* p ) : handle( p ) {}

		template <class T>
		inline const T* as() const { return boost::get<const T*>(handle); }
	};

	template <int Dim>
	struct PrimitiveBox
	{
		typedef CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, PrimitiveHandle<Dim>*> Type;
	};


	template <int Dim>
	struct BoxCollection
	{
		typedef std::vector<typename PrimitiveBox<Dim>::Type> Type;
	};
	template <int Dim>
	struct HandleCollection
	{
		typedef std::list<PrimitiveHandle<Dim> > Type;
	};

	template <int Dim>
	class GeometrySet
	{
	public:
		typedef std::list<typename TypeForDimension<Dim>::Point>   PointCollection;
		typedef std::list<typename TypeForDimension<Dim>::Segment> SegmentCollection;
		typedef std::list<typename TypeForDimension<Dim>::Surface> SurfaceCollection;
		typedef std::list<typename TypeForDimension<Dim>::Volume>  VolumeCollection;

		GeometrySet();

		/**
		 * Construct a GeometrySet from a SFCGAL::Geometry
		 */
		GeometrySet( const Geometry& g );

		/**
		 * Add a geometry by decomposing it into CGAL primitives
		 */
		void addGeometry( const Geometry& g );

		/**
		 * add a primitive from a PrimitiveHandle  to the set
		 */
		void addPrimitive( const PrimitiveHandle<Dim>& p );

		/**
		 * add a primitive from a CGAL::Object to the set
		 */
		void addPrimitive( const CGAL::Object& o );

		/**
		 * add a point to the set
		 */
		void addPrimitive( const typename TypeForDimension<Dim>::Point& g );
		template <class IT>
		void addPoints( IT ibegin, IT iend )
		{
			std::copy( ibegin, iend, std::back_inserter(_points) );
		}

		/**
		 * add a segment to the set
		 */
		void addPrimitive( const typename TypeForDimension<Dim>::Segment& g );
		template <class IT>
		void addSegments( IT ibegin, IT iend )
		{
			std::copy( ibegin, iend, std::back_inserter(_segments) );
		}

		/**
		 * add a surface to the set
		 */
		void addPrimitive( const typename TypeForDimension<Dim>::Surface& g );
		template <class IT>
		void addSurfaces( IT ibegin, IT iend )
		{
			std::copy( ibegin, iend, std::back_inserter(_surfaces) );
		}

		/**
		 * add a volume to the set
		 */
		void addPrimitive( const typename TypeForDimension<Dim>::Volume& g );
		template <class IT>
		void addVolumes( IT ibegin, IT iend )
		{
			std::copy( ibegin, iend, std::back_inserter(_volumes) );
		}

		/**
		 * Compute all bounding boxes and handles of the set
		 */
		void compute_bboxes( typename HandleCollection<Dim>::Type& handles, typename BoxCollection<Dim>::Type& boxes ) const;

		inline PointCollection& points() { return _points; }
		inline const PointCollection& points() const { return _points; }

		inline SegmentCollection& segments() { return _segments; }
		inline const SegmentCollection& segments() const { return _segments; }

		inline SurfaceCollection& surfaces() { return _surfaces; }
		inline const SurfaceCollection& surfaces() const { return _surfaces; }

		inline VolumeCollection& volumes() { return _volumes; }
		inline const VolumeCollection& volumes() const { return _volumes; }

		/**
		 * convert the set to a SFCGAL::Geometry
		 */
		std::auto_ptr<Geometry> recompose() const;
	private:
		///
		/// Given an input SFCGAL::Geometry, decompose it into CGAL primitives
		void _decompose( const Geometry& g );


		PointCollection _points;
		SegmentCollection _segments;
		SurfaceCollection _surfaces;
		VolumeCollection _volumes;
	};
}
}

#endif
