//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#ifndef CAMGEN_COLGEN_FAC_H_
#define CAMGEN_COLGEN_FAC_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Colour generator factory specialisation for the istream class, creating *
 * a colour generator instance from a filestream.                          *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Camgen/static_comp.h>
#include <Camgen/MC_config.h>
#include <Camgen/uni_cols.h>
#include <Camgen/qcd_cols.h>

namespace Camgen
{
    /// Colour generator factory declaration.

    template<class model_t,std::size_t N_in,std::size_t N_out,class rng_t,bool q=model_t::coloured>class colour_generator_factory;

    /// Colour generator factory specialization for uncoloured models: always
    /// returns zero pointers.

    template<class model_t,std::size_t N_in,std::size_t N_out,class rng_t>class colour_generator_factory<model_t,N_in,N_out,rng_t,false>
    {
	public:

	    typedef colour_generator<typename model_t::value_type,N_in,N_out,false> generator_type;
	    typedef colour_generators::type generator_tag;

	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it)
	    {
		return NULL;
	    }
	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it,generator_tag tag)
	    {
		return NULL;
	    }
	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it,std::istream& is)
	    {
		std::string initline,endline;
		while(initline!="</colgen>" and !is.eof())
		{
		    std::getline(is,initline);
		}
		if(is.eof())
		{
		    log(log_level::warning)<<CAMGEN_STREAMLOC<<"end of file reached before final tag detected"<<endlog;
		    return NULL;
		}
		return NULL;
	    }
    };

    /// Colour generator factory for coloured models.

    template<class model_t,std::size_t N_in,std::size_t N_out,class rng_t>class colour_generator_factory<model_t,N_in,N_out,rng_t,true>
    {
	public:

	    typedef typename model_t::value_type value_type;
	    typedef colour_generator<value_type,N_in,N_out,model_t::continuous_colours> generator_type;
	    typedef colour_generators::type generator_tag;

	    static const bool cc=model_t::continuous_colours;

	    /// Factory method returning a colour generator from a tree
	    /// iterator.

	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it)
	    {
		return create_instance(it,colour_generator_type());
	    }

	    /// Factory method returning a colour generator from a tree iterator
	    /// and a generator tag.

	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it,generator_tag tag)
	    {
		switch(tag)
		{
		    case colour_generators::uniform:
			return uniform_colours<value_type,N_in,N_out,rng_t,cc>::template create_instance<model_t>(it);
		    case colour_generators::summation:
			return colour_summer<value_type,N_in,N_out,cc>::template create_instance<model_t>(it);
		    case colour_generators::flow_sampling:
			if(static_eq<typename model_t::colour_treatment,adjoint>::value)
			{
			    return adjoint_QCD<value_type,N_in,N_out,model_t::N_c,rng_t,cc>::template create_instance<model_t>(it);
			}
			else if(static_eq<typename model_t::colour_treatment,colour_flow>::value)
			{
			    return colour_flow_QCD<value_type,N_in,N_out,model_t::N_c,rng_t,cc>::template create_instance<model_t>(it);
			}
			else
			{
			    return NULL;
			}
		    default:
			return NULL;
		}
	    }

	    /// Factory method returning a colour generator from a tree iterator
	    /// and an input stream.

	    static generator_type* create_instance(typename CM_algorithm<model_t,N_in,N_out>::tree_iterator it,std::istream& is)
	    {
		std::string initline,endline;
		while(initline!="<colgen>" and !is.eof())
		{
		    std::getline(is,initline);
		}
		if(is.eof())
		{
		    log(log_level::warning)<<CAMGEN_STREAMLOC<<"end of file reached before initial data are read"<<endlog;
		    return NULL;
		}
		generator_type* result;
		std::string type;
		is>>type;
		if(type=="sum")
		{
		    result=colour_summer<value_type,N_in,N_out,cc>::template create_instance<model_t>(it);
		}
		else if(type=="uniform")
		{
		    result=uniform_colours<value_type,N_in,N_out,rng_t,cc>::template create_instance<model_t>(it);
		}
		else if(type=="qcd")
		{
		    if(static_eq<typename model_t::colour_treatment,adjoint>::value)
		    {
			result=adjoint_QCD<value_type,N_in,N_out,model_t::N_c,rng_t,cc>::template create_instance<model_t>(it);
		    }
		    else if(static_eq<typename model_t::colour_treatment,colour_flow>::value)
		    {
			result=colour_flow_QCD<value_type,N_in,N_out,model_t::N_c,rng_t,cc>::template create_instance<model_t>(it);
		    }
		    else
		    {
			result=NULL;
		    }
		}
		else
		{
		    log(log_level::warning)<<CAMGEN_STREAMLOC<<"colour generator type "<<type<<" not recognised"<<endlog;
		    result=NULL;
		}
		if(result!=NULL)
		{
		    result->load(is);
		}
		do
		{
		    std::getline(is,endline);
		}
		while(endline!="</colgen>" and !is.eof());
		return result;
	    }
    };
    template<class model_t,std::size_t N_in,std::size_t N_out,class rng_t>const bool colour_generator_factory<model_t,N_in,N_out,rng_t,true>::cc;
}

#endif /*COLGEN_FAC_H_*/

