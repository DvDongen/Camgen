//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#ifndef CAMGEN_SYMTVV_H_
#define CAMGEN_SYMTVV_H_

#include <Camgen/def_args.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Symmetric tensor-vector-vector interaction Feynman rule class decaration and  *
 * definition. Implemented are the recursive relations corresponding to the      *
 * Feynman rule                                                                  *
 *                                                                               *
 * 		g^{mu,rho}g^{nu,sigma} + g^{mu,sigma}g(nu,rho}                   *
 *                                                                               *
 * where mu and nu are the Lorentz indices of the symmetric tensor field and     *
 * sigma and rho those of the vector particles.                                  *
 *                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace Camgen
{
    /* Implementation of the Feynman rule class: */

    template<class model_t>class symtvv
    {
	public:

	    /* Reference type definition to the model type: */

	    typedef model_t model_type;

	    /* Vertex rank definition: */

	    static const std::size_t rank=3;
	    
	    /* Number of couplings used by the vertex: */
	    
	    static const std::size_t params=1;

	    /* Size of the vertex tensor: */

	    static const std::size_t tensor_size=model_t::dimension*model_t::dimension*model_t::dimension*model_t::dimension;
	    
	    /* Boolean denoting whether the vertex tensor depends on momenta: */
	    
	    static const bool p_dependent=false;

	    /* Boolean denoting whether the vertex interacts fermions: */

	    static const bool fermionic=false;
	    
	    /* Tensor sizes of interacting amplitudes: */

	    static const std::size_t sizes[4]

	    /* Feynman rule formula: */

	    static const std::string formula;
    };
    template<class model_t>const std::size_t symtvv<model_t>::rank;
    template<class model_t>const std::size_t symtvv<model_t>::params;
    template<class model_t>const std::size_t symtvv<model_t>::vector_size;
    template<class model_t>const std::size_t symtvv<model_t>::tensor_size;
    template<class model_t>const bool symtvv<model_t>::p_dependent;
    template<class model_t>const bool symtvv<model_t>::fermionic;
    template<class model_t>const std::size_t symtvv<model_t>::sizes[4]={model_t::dimension*model_t::dimension,model_t::dimension,model_t::dimension,0};
    template<class model_t>const std::string symtvv<model_t>::formula="(g(mu1,mu3)g(mu2,mu4)+g(mu1,mu4)g(mu2,mu3))";

    /* Specialisation of the evaluate class template: */

    template<class model_t>class evaluate< symtvv<model_t> >
    {
	public:

	    /* Reference type definition of the vertex type: */

	    typedef symtvv<model_t> vertex_type;

	    /* The usual type definitions: */

	    DEFINE_BASIC_TYPES(model_t);

	private:

	    /* Spacetime type definition: */

	    DEFINE_SPACETIME_TYPE(model_t);

	    /* Vector size definition: */
	    
	    static const std::size_t vector_size=model_t::dimension;

	public:

	    /* Initialisation phase: */

	    static void initialise()
	    {
		spacetime_type::initialise();
	    }

	    /* function returning the index ranges of the interacting
	     * subamplitudes: */

	    static std::vector<size_type>& get_index_ranges(size_type n,std::vector<size_type>& v)
	    {
		v.clear();
		if(n==0)
		{
		    v.push_back(vector_size);
		    v.push_back(vector_size);
		}
		else if(n==1 or n==2)
		{
		    v.push_back(vector_size);
		}
		return v;
	    }

	    /* Recursion relation evaluating the tensor subamplitude: */

	    static void first(ARG_LIST)
	    {
		value_type c;
		for(size_type mu=0;mu<vector_size;++mu)
		{
		    for(size_type nu=0;nu<=mu;++nu)
		    {
			c=factor*C_0*(A_1[mu]*A_2[nu]+A_2[mu]*A_1[nu]);
			A_0[mu*vector_size+nu]+=c;
			A_0[nu*vector_size+mu]+=c;
		    }
		}
	    }

	    /* Recursion relation evaluating the first vector subamplitude: */

	    static void second(ARG_LIST)
	    {
		for(size_type mu=0;mu<vector_size;++mu)
		{
		    A_1[mu]+=(r_value_type)2*factor*C_0*spacetime_type::dot(A_0,A_2);
		    A_0+=vector_size;
		}
		A_0-=(vector_size*vector_size);
	    }

	    /* Recursion relation evaluating the second vector subamplitude: */

	    static void third(ARG_LIST)
	    {
		for(size_type mu=0;mu<vector_size;++mu)
		{
		    A_2[mu]+=(r_value_type)2*factor*C_0*spacetime_type::dot(A_0,A_1);
		    A_0+=vector_size;
		}
		A_0-=(vector_size*vector_size);
	    }
	    static void fourth(ARG_LIST){}
    };
    template<class model_t>const std::size_t evaluate< symtvv<model_t> >::vector_size;
}

#include <Camgen/undef_args.h>

#endif /*CAMGEN_SYMTVV_H_*/

