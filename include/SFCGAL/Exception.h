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
		 * returns diagnostic informations (file, line, etc.)
		 */
		std::string diagnostic() const throw();
	protected:
		std::string _message;
	};
	
} // namespace SFCGAL

#endif


