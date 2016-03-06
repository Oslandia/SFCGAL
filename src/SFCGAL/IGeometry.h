#ifndef _SGCGAL_IGEOMETRY_H_
#define _SFCGAL_IGEOMETRY_H_

#include <SFCGAL/CoordinateType.h>
#include <SFCGAL/GeometryType.h>

namespace SFCGAL {

    /**
     * Base class for geometry with a dynamic kernel
     */
    class IGeometry {
    public:
        virtual ~IGeometry(){}
        
        /**
         * Test if geometry is instance of T
         */
        template < typename T >
        bool is() const { 
            return dynamic_cast<const T*>(this) != nullptr; 
        }

        /**
         * Downcast to T&
         */
        template < typename T >
        T & as() { 
            T* result = dynamic_cast<T*>(this) ;
            BOOST_ASSERT(result != nullptr);
            return *result ;  
        }
        /**
         * Downcast to const T&
         */
        template < typename T >
        const T & as() const { 
            const T* result = dynamic_cast<const T*>(this) ;
            BOOST_ASSERT(result != nullptr);
            return *result ;  
        }
        
        /**
         * Get GeometryType identifier
         */
        virtual GeometryType geometryTypeId() const = 0 ;
        /**
         * Get GeometryType name
         */
        virtual std::string geometryType() = 0 ;
        
        /**
         * Indicates if the geometry is empty
         */
        virtual bool isEmpty() const = 0 ;
        /**
         * Indicates if the geometry is 3D
         */
        virtual bool is3D() const = 0 ;
        /**
         * Indicates if the geometry has M coordinates
         */
        virtual bool isMeasured() const = 0 ;
        
    protected:
        IGeometry() = default;
        IGeometry( const IGeometry& ) = default;
    } ;

    
    
} // namespace SFCGAL

#endif
