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

};
