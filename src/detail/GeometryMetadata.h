#ifndef _SFCGAL_DETAIL_GEOMETRYMETADATA_H_
#define _SFCGAL_DETAIL_GEOMETRYMETADATA_H_

namespace SFCGAL {
namespace detail {
    
    /**
     * Stores information about a geometry (isValid, is3D, etc.)
     */
    class GeometryMetadata {
    public:
        GeometryMetadata( bool is3d = false, bool validityFlag = false);
        
        /**
         * Indicite if geometry is 3D
         */
        bool is3D() const ;
        /**
         * set 3d flag
         */
        void set3D( bool is3d ) ;
        
        /**
         * Force the state of the validity flag. The validity flag allows to bypass validity checks
         * If the flag is true, it means the geometry is considered valid
         * If the flag is false, it means the validity state of the geometry is unknown
         * The flag is only changed for this geometry and not the internal geometries.
         * @see propagateValidityFlag
         */
        void setValidityFlag( bool validityFlag );
        /** Returns the validity flag */
        bool validityFlag() const;
        
    private:
        // TODO optimize storage?
        bool _is3d ;
        bool _validityFlag ;
    } ;
    
    
} // namespace detail
} // namespace SFCGAL

#endif
