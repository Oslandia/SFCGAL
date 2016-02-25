#ifndef _SFCGAL_DETAIL_GEOMETRYMETADATA_H_
#define _SFCGAL_DETAIL_GEOMETRYMETADATA_H_

namespace SFCGAL {
namespace detail {
    
    /**
     * Coordinate properties allows to store extra information on
     * on a geometry points.
     * 
     * (for now, it only stores M, it could be refactored to a property map
     *  in order to store texturation information for example)
     */
    class CoordinateProperties {
    public:
        CoordinateProperties() ;

        /*
         * Indicates if M is defined (not NaN)
         */
        bool hasM() const ;

        /**
         * Returns the m value (NaN is not defined)
         */
        inline double    m() const {
            return _m ;
        }
        /**
         * Sets the m value
         */
        inline void      setM( const double& m ) {
            _m = m ;
        }
    private:
        double _m ;
    } ;
    
    
} // namespace detail
} // namespace SFCGAL

#endif
