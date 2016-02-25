#include <SFCGAL/detail/GeometryMetadata.h>

namespace SFCGAL {
namespace detail {

///
///
///
GeometryMetadata::GeometryMetadata( bool is3d, bool validityFlag):
    _is3d(is3d),
    _validityFlag(validityFlag)
{
    
}

///
///
///
bool GeometryMetadata::is3D() const 
{
    return _is3d ;
}

///
///
///
void GeometryMetadata::set3D( bool is3d ) 
{
    _is3d = is3d ;
}

///
///
///
void GeometryMetadata::setValidityFlag( bool validityFlag )
{
    _validityFlag = validityFlag ;
}

///
///
///
bool GeometryMetadata::validityFlag() const
{
    return _validityFlag ;
}

    
} // namespace detail
} // namespace SFCGAL
