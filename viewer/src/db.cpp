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

#include "db.h"

namespace Db
{
    template <>
    bool Value::as<bool>()
    {
	return std::string(value_, len_ ) == "t" ? true : false;
    }

    template <>
    std::string Value::as<std::string>()
    {
	return std::string( value_, len_ );
    }

    template <>
    long long Value::as<long long>()
    {
	long long v;
 	sscanf( value_, "%Ld", &v );
	return v;
    }
    template <>
    int Value::as<int>()
    {
	int v;
 	sscanf( value_, "%d", &v );
	return v;
    }
    template <>
    float Value::as<float>()
    {
	float v;
 	sscanf( value_, "%f", &v );
	return v;
    }
    template <>
    double Value::as<double>()
    {
	double v;
 	sscanf( value_, "%lf", &v );
	return v;
    }

}
