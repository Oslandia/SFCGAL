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

#ifndef _IGN_EXCEPTION_H_
#define _IGN_EXCEPTION_H_

#include <SFCGAL/config.h>

#include <string>

#include <boost/exception/all.hpp>
#include <boost/format.hpp>

namespace SFCGAL {

/**
 * Base SFCGAL Exception
 *
 * \code
 * BOOST_THROW_EXCEPTION( Exception("invalid geometry") );
 * \endcode
 */
class SFCGAL_API Exception : public virtual boost::exception, public virtual  std::exception {
public:
    Exception() throw();
    Exception( std::string const& message ) throw();
    virtual ~Exception() throw();

    /**
     * returns the exception message
     */
    virtual const char* what() const throw();
    /**
     * returns diagnostic information (file, line, etc.)
     */
    std::string diagnostic() const throw();
protected:
    std::string _message;
};

/**
 * SFCGAL Exception thrown when invalid geometries are found before entering an algo
 */
class SFCGAL_API GeometryInvalidityException : public Exception {
public:
    GeometryInvalidityException( std::string const& message ):
        Exception( message ) {
    }

};

/**
 * SFCGAL Exception thrown when a function is not implemented
 */
class SFCGAL_API NotImplementedException : public Exception {
public:
    NotImplementedException( std::string const& message ):
        Exception( message ) {
    }

};

/**
 * SFCGAL Exception thrown when geometry is inapropriate for a function
 */
class SFCGAL_API InappropriateGeometryException : public Exception {
public:
    InappropriateGeometryException( std::string const& message ):
        Exception( message ) {
    }

};

/**
 * SFCGAL Exception thrown when non finite value is found
 */
class SFCGAL_API NonFiniteValueException : public Exception {
public:
    NonFiniteValueException( std::string const& message ):
        Exception( message ) {
    }

};

/**
 * SFCGAL Exception thrown when parsing WKT
 */
class SFCGAL_API WktParseException : public Exception {
public:
    WktParseException( std::string const& message ):
        Exception( message ) {
    }

};

} // namespace SFCGAL

#endif


