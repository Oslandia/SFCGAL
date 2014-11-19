/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _SFCGAL_VALIDITY_H_
#define _SFCGAL_VALIDITY_H_

namespace SFCGAL {

/**
 * @brief the class, convertible to bool, that stores the reason why a geom is invalid
 */
struct Validity {
    /**
     * @note the class has private ctor to force the use of functions valid() and invalid(reason) that are clearer in the code than to remember that "Valid constructed with a reason is invalid"
     */
    static const Validity valid() {
        return Validity();
    }
    static const Validity invalid( const std::string& reason ) {
        return Validity( reason );
    }
    operator bool() const {
        return _valid;
    }
    const std::string& reason() const {
        return _reason;
    }
private:
    bool _valid; // not const to allow default copy
    std::string _reason;
    /**
     * @brief default ctor for valid
     */
    Validity():_valid( true ) {}
    /**
     * @brief if we construct with a reason, the class is invalid
     */
    Validity( const std::string& reason ):
        _valid( false ),
        _reason( reason ) {
    }
};

}
#endif
