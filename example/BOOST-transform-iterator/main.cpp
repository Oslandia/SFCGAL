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
#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/iterator/transform_iterator.hpp>

typedef boost::shared_ptr< int > int_ptr ;

template < typename Iter >
struct GetValue : std::unary_function<
	typename Iter::value_type,
	typename Iter::value_type::value_type & //shared_ptr has a reference parameter
>
{
		typename Iter::value_type::value_type& operator()
				(typename Iter::value_type& p) const
		{
			return * p ;
		}
};

template < typename Iter >
struct GetConstValue : std::unary_function<
	typename Iter::value_type,
	const typename Iter::value_type::value_type & //shared_ptr has a reference parameter
>
{
		const typename Iter::value_type::value_type& operator()
				(const typename Iter::value_type& p) const
		{
			return * p ;
		}
};




int main( int argc, char* argv[] ){
	std::vector< int_ptr > values ;
	typedef std::vector< int_ptr >::iterator       input_iterator ;
	typedef std::vector< int_ptr >::const_iterator const_input_iterator ;

	values.push_back( int_ptr( new int(5) ) );
	values.push_back( int_ptr( new int(8) ) );
	values.push_back( int_ptr( new int(10) ) );


	typedef boost::transform_iterator< GetValue< input_iterator >, input_iterator >                  iterator ;
	typedef boost::transform_iterator< GetConstValue< const_input_iterator >, const_input_iterator > const_iterator ;

	//test iterator
	std::cout << "iterator" << std::endl;
	{
		iterator it = boost::make_transform_iterator(
			values.begin(),
			GetValue< input_iterator >()
		);
		iterator end = boost::make_transform_iterator(
			values.end(),
			GetValue< input_iterator >()
		);

		for ( ; it != end; ++it ){
			std::cout << (*it) << std::endl ;
			(*it) += 10 ;
		}
	}
	//test const_iterator
	std::cout << "const_iterator" << std::endl;
	{
		const_iterator it = boost::make_transform_iterator(
			values.begin(),
			GetConstValue< const_input_iterator >()
		);
		const_iterator end = boost::make_transform_iterator(
			values.end(),
			GetConstValue< const_input_iterator >()
		);

		for ( ; it != end; ++it ){
			std::cout << (*it) << std::endl ;
			//doesn't build
			//(*it) += 10 ;
		}
	}

	return 0 ;
}
