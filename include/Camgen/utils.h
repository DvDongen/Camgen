//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#ifndef CAMGEN_UTILS_H_
#define CAMGEN_UTILS_H_

#include <limits>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <complex>
#include <Camgen/num_config.h>


/* * * * * * * * * * * * * *
 * Utilities for Camgen...*
 *                         *
 * * * * * * * * * * * * * */

namespace Camgen
{
    /* Checking equality up to certain precision: */

    template<class T>bool equals(const T& a,const T& b)
    {
	if(std::abs(a-b)<numeric_configuration<T>::epsilon_abs)
	{
	    return true;
	}
	return std::abs(a-b)<=numeric_configuration<T>::epsilon_rel*std::max(std::abs(a),std::abs(b));
    }
    template<class T>bool equals(const std::complex<T>& a,const std::complex<T>& b)
    {
	if(std::abs(a-b)<numeric_configuration<T>::epsilon_abs)
	{
	    return true;
	}
	return std::abs(a-b)<=numeric_configuration<T>::epsilon_rel*std::max(std::abs(a),std::abs(b));
    }

    /* Comparisons up to PRECISION: */

    template<class T>bool smaller(const T& a,const T& b)
    {
	return (!equals(a,b) and a<b);
    }
    template<class T>bool larger(const T& a,const T& b)
    {
	return (!equals(a,b) and a>b);
    }


    /* Checking equality of two sequences up to PRECISION: */

    template<class T>bool equal_sequences(const T& first,const T& second)
    {
	if(first.size()==second.size())
	{
	    typename T::const_iterator it1=first.begin();
	    for(typename T::const_iterator it2=second.begin();it2 != second.end();++it2)
	    {
		if(!equals(*it1,*it2))
		{
		    return false;
		}	
		++it1;
	    }
	    return true;
	}
	else
	{
	    return false;
	}
    }

    /* Optimised multiplication by the imaginary unit: */

    template<class T>std::complex<T>times_i(const std::complex<T>& z)
    {
	return std::complex<T>(-z.imag(),z.real());
    }

    /* Compute z1+i*z2: */

    template<class T>std::complex<T>make_z(const T& z1,const T& z2)
    {
	return std::complex<T>(z1,z2);
    }

    /* Compute z1+i*z2, where z1 and z2 are complex numbers: */

    template<class T>std::complex<T>make_z(const std::complex<T>& z1,const std::complex<T>& z2)
    {
	return std::complex<T>(z1.real()-z2.imag(),z1.imag()+z2.real());
    }

    /* Compute z1-i*z2: */

    template<class T>std::complex<T>make_zbar(const T& z1,const T& z2)
    {
	return std::complex<T>(z1,-z2);
    }
    /* Compute z1-i*z2, where z1 and z2 are complex numbers: */

    template<class T>std::complex<T>make_zbar(const std::complex<T>& z1,const std::complex<T>& z2)
    {
	return std::complex<T>(z1.real()+z2.imag(),z1.imag()-z2.real());
    }

    /* Evaluates the signed root of a real number: */

    template<class T>T sgn_sqrt(const T& x)
    {
	return (x<(T)0)?(-std::sqrt(-x)):(std::sqrt(x));
    }

    /* Evaluates the signed square of a real number: */

    template<class T>T sgn_sq(const T& x)
    {
	return (x<(T)0)?(-x*x):(x*x);
    }

    /* Output utilities for simple complex numbers appearing in algebraic
     * expressions: */

    template<class T>std::ostream& shortprint(std::ostream& os,const std::complex<T>& z)
    {
	if(z.imag()==(T)0)
	{
	    os<<z.real();
	}
	else if(z.real()==(T)0)
	{
	    if(z.imag()==(T)1)
	    {
		os<<"i";
	    }
	    else if(z.imag()==(T)-1)
	    {
		os<<"-i";
	    }
	    else
	    {
		os<<z.imag()<<"i";
	    }
	}
	else
	{
	    if(z.imag()==(T)1)
	    {
		os<<z.real()<<"+i";
	    }
	    else if(z.imag()==(T)-1)
	    {
		os<<z.real()<<"-i";
	    }
	    else if(z.imag()<(T)0)
	    {
		os<<z.real()<<"-"<<-z.imag()<<"i";
	    }
	    else
	    {
		os<<z.real()<<"+"<<z.imag()<<"i";
	    }
	}
	return os;
    }

    template<class T>std::ostream& shortprint_prefactor(std::ostream& os,const std::complex<T>& z)
    {
	if(z.imag()==(T)0)
	{
	    if(z.real()==(T)1)
	    {
		os<<" + ";
		return os;
	    }
	    else if(z.real()==(T)-1)
	    {
		os<<" - ";
		return os;
	    }
	    else if(z.real()<0)
	    {
		os<<" - "<<-z.real();
		return os;
	    }
	    else
	    {
		os<<" + "<<z.real();
		return os;
	    }
	}
	else if(z.real()==(T)0)
	{
	    if(z.imag()==(T)1)
	    {
		os<<" + i";
		return os;
	    }
	    else if(z.imag()==(T)-1)
	    {
		os<<" - i";
		return os;
	    }
	    else if(z.imag()<0)
	    {
		os<<" - "<<-z.imag()<<"i";
		return os;
	    }
	    else
	    {
		os<<" + "<<z.imag()<<"i";
	    }
	}
	else
	{
	    os<<" + (";
	    shortprint(os,z);
	    os<<")";
	    return os;
	}
    }

    template<class T>std::ostream& shortprint_first_prefactor(std::ostream& os,const std::complex<T>& z)
    {
	if(z.imag()==(T)0)
	{
	    if(z.real()==(T)1)
	    {
		return os;
	    }
	    else if(z.real()==(T)-1)
	    {
		os<<"-";
		return os;
	    }
	    else if(z.real()<0)
	    {
		os<<"-"<<-z.real();
		return os;
	    }
	    else
	    {
		os<<z.real();
		return os;
	    }
	}
	else if(z.real()==(T)0)
	{
	    if(z.imag()==(T)1)
	    {
		os<<"i";
		return os;
	    }
	    else if(z.imag()==(T)-1)
	    {
		os<<"-i";
		return os;
	    }
	    else if(z.imag()<0)
	    {
		os<<"-"<<-z.imag()<<"i";
		return os;
	    }
	    else
	    {
		os<<z.imag()<<"i";
	    }
	}
	else
	{
	    os<<"(";
	    shortprint(os,z);
	    os<<")";
	    return os;
	}
    }

    /* String-producing function for object types for which the streaming
     * operator is overloaded: */

    template<class T>std::string make_string(const T& object)
    {
	std::stringstream ss;
	ss<<object;
	return ss.str();
    }

    /* Safe serialization of floating-point numbers, printing "M" for minus
     * infinity, "P" for plus infinity and "N" for nan: */

    template<class T>std::ostream& safe_write(std::ostream& os,const T& x)
    {
	if(x!=x)
	{
	    return (os<<'N');
	}
	else if(x==-std::numeric_limits<T>::infinity())
	{
	    return (os<<'M');
	}
	else if(x==std::numeric_limits<T>::infinity())
	{
	    return (os<<'P');
	}
	else
	{
	    return (os<<x);
	}
    }

    /* Safe deserialization of floating-point numbers, reading minus infinity
     * for "M", plus infinity for "P" and not a number for "N" */

    template<class T>std::istream& safe_read(std::istream& is,T& x)
    {
	is>>std::ws;
	char c=is.peek();
	switch(c)
	{
	    case 'N':
		x=(x-x)/(x-x);
		is.ignore(1);
		break;
	    case 'M':
		x=-std::numeric_limits<T>::infinity();
		is.ignore(1);
		break;
	    case 'P':
		x=std::numeric_limits<T>::infinity();
		is.ignore(1);
		break;
	    default:
		is>>x;
	}
	return is;
    }
}

#endif /*CAMGEN_UTILS_H_*/

