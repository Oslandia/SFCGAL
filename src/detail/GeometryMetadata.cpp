#include <SFCGAL/detail/GeometryMetadata.h>

namespace SFCGAL {
namespace detail {

///
///
///
GeometryMetadata::GeometryMetadata(bool validityFlag):
    _validityFlag(validityFlag)
{
    
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
