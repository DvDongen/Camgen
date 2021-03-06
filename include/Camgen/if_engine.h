//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

/*! \file if_engine.h
  \brief Abstract base class for output interface engines.
  */

#ifndef CAMGEN_IF_ENGINE_H_
#define CAMGEN_IF_ENGINE_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Abstract for base class for output interface engines, which should implement  *
 * the fill() method to evaluate the variables that need to be written to disk.  *
 *                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Camgen/if_output.h>

namespace Camgen
{
    /// Interface engine base class:

    template<class model_t>class interface_engine: public ps_generator_viewer<model_t>
    {
	public:

	    /* Type definitions: */

	    typedef model_t model_type;
	    typedef ps_generator_viewer<model_t> base_type;
	    typedef typename base_type::momentum_type momentum_type;
	    typedef typename base_type::value_type value_type;
	    typedef typename base_type::size_type size_type;
	    typedef typename base_type::spacetime_type spacetime_type;

	    /* Public constructors: */
	    /*----------------------*/

	    /// Default constructor.

	    interface_engine():output(NULL),evt_size(0){}

	    /// Constructor with generator instance argument.

	    interface_engine(const ps_generator_base<model_t>* gen_):base_type(gen_),output(NULL),evt_size(0){}

	    /// Constructor with generator and output instance arguments.

	    interface_engine(const ps_generator_base<model_t>* gen_,interface_output<model_t>* output_):base_type(gen_),output(output_),evt_size(0){}

	    /* Public destructors: */
	    /*---------------------*/

	    virtual ~interface_engine(){}

	    /* Public modifiers: */
	    /*-------------------*/

	    /// Copies the output variable addresses to the output object

	    void add_branches(interface_output<model_t>* output_)
	    {
		output=output_;
		add_variables();
	    }

	    /// Abstract clone method.

	    virtual interface_engine<model_t>* clone() const=0;

	    /// Abstract method filling the output variables.

	    virtual void fill()=0;

	    /* Public readout functions: */
	    /*---------------------------*/

	    size_type event_size() const
	    {
		return evt_size;
	    }

	protected:

	    /// Abstract mathod adding variables to the interface. Derived classes should
	    /// implement this function with add_variable() statements to stream the
	    /// variable addresses to the output interface.

	    virtual void add_variables()=0;

	    /// Adds an momentum-valued output variable to the interface engine.

	    bool add_variable(const momentum_type& q,const std::string& name)
	    {
		if(output!=NULL)
		{
		    evt_size+=sizeof(momentum_type);
		    return output->branch(&q,name);
		}
		return false;
	    }

	    /// Adds a floating-point-valued output variable to the interface engine.

	    bool add_variable(const value_type& q,const std::string& name)
	    {
		if(output!=NULL)
		{
		    evt_size+=sizeof(value_type);
		    return output->branch(&q,name);
		}
		return false;
	    }

	    /// Adds an integer-valued output variable to the interface engine.

	    bool add_variable(const int& q,const std::string& name)
	    {
		if(output!=NULL)
		{
		    evt_size+=sizeof(int);
		    return output->branch(&q,name);
		}
		return false;
	    }

	    /// Adds an boolean-valued output variable to the interface engine.

	    bool add_variable(const bool& q,const std::string& name)
	    {
		if(output!=NULL)
		{
		    evt_size+=sizeof(bool);
		    return output->branch(&q,name);
		}
		return false;
	    }

	private:

	    /* Output file object: */

	    interface_output<model_t>* output;

	    /* event size: */

	    size_type evt_size;
    };
}

#endif /*CAMGEN_IF_ENGINE_H_*/

