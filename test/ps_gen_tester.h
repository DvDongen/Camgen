//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#ifndef PS_GEN_TESTER_H_
#define PS_GEN_TESTER_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Testing class template for phase space Monte-Carlo generators. The 'run'  *
 * function performs a check on every event and creates a plot comparing     *
 * respectively the phase space volume and the cross section with uniformly  *
 * generated points.                                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Camgen/plt_config.h>
#include <Camgen/type_holders.h>
#include <Camgen/isgen_fac.h>
#include <Camgen/psgen_fac.h>
#include <Camgen/multiplot.h>

namespace Camgen
{
    /* Monte-Carlo phase space generator class template declaration: */

    template<class model_t,std::size_t N_in,std::size_t N_out,class rng_t>class ps_generator_tester
    {
	public:

	    /* Type definitions: */

	    typedef model_t model_type;
	    typedef typename model_t::value_type value_type;
	    typedef typename get_spacetime_type<model_t>::type spacetime_type;
	    typedef vector<value_type,spacetime_type::dimension> momentum_type;
	    typedef typename momentum_type::size_type size_type;
	    typedef ps_generator_factory<model_t,N_in,N_out,rng_t> gen_factory;
	    typedef typename CM_algorithm<model_t,N_in,N_out>::tree_iterator CM_tree_iterator;

	    /* Output file name: */

	    const std::string filename;
	    
	    /* Process symmetry factor: */
	    
	    const value_type sym_factor;
	    
	    /* Amplitude process tree iterator: */
	    
	    CM_tree_iterator amplitude;

	    /* Phase space generator: */

	    ps_generator<model_t,N_in,N_out>* ps_gen;
	    
	    /* Uniform phase space generator for comparison purposes: */
	    
	    ps_generator<model_t,N_in,N_out>* uni_gen;

	    /* Constructor from a CM-tree iterator, an initial-state generator instance
	     * and a file name: */

	    ps_generator_tester(CM_tree_iterator amplitude_,const std::string& filename_,initial_states::type isgen_type=Camgen::initial_state_type(),phase_space_generators::type psgen_type=Camgen::phase_space_generator_type()):filename(filename_),sym_factor(amplitude_->symmetry_factor()),amplitude(amplitude_)
	    {
		ps_gen=gen_factory::create_instance(amplitude,isgen_type,psgen_type);
		uni_gen=gen_factory::create_instance(amplitude,isgen_type,phase_space_generators::uniform);

	    }

	    /* Sets the i-th beam energy: */

	    bool set_beam_energy(int i,const value_type& E)
	    {
		return (ps_gen->set_beam_energy(i,E) and uni_gen->set_beam_energy(i,E));
	    }

	    /* Run method, performing N_events calls to the MC generator and RAMBO, and
	     * recording N_points cross section along the run: */

	    void run(size_type N_events,size_type N_points)
	    {
		size_type N_batch=N_events/N_points;
		value_type n,psvol1,psvol1err,psvol2,psvol2err,xsec1,xsec1err,xsec2,xsec2err;
		bool have_gp=plot_config::gnuplot_path!=NULL;
		data_wrapper* data=have_gp?new data_wrapper(&n,&psvol1,&psvol1err):new data_wrapper(filename,&n,&psvol1,&psvol1err);
		data->add_leaf(&psvol2);
		data->add_leaf(&psvol2err);
		data->add_leaf(&xsec1);
		data->add_leaf(&xsec1err);
		data->add_leaf(&xsec2);
		data->add_leaf(&xsec2err);

		value_type w1sum=0;
		value_type w1sqsum=0;
		value_type w2sum=0;
		value_type w2sqsum=0;

		ps_gen->refresh_Ecm();
		uni_gen->refresh_Ecm();
		ps_gen->refresh_m_min();
		uni_gen->refresh_m_min();

		for(size_type i=0;i<N_events;++i)
		{
		    if(ps_gen->generate())
		    {
			ps_gen->check();
			value_type w1=(ps_gen->pass())?ps_gen->weight():(value_type)0;
			w1sum+=w1;
			w1sqsum+=(w1*w1);
			amplitude->reset();
			ps_gen->integrand()*=std::norm(amplitude->evaluate())*sym_factor;
			
			if(ps_gen->weight()!=ps_gen->weight())
			{
			    Camgen::log(log_level::warning)<<"Invalid phase space weight encountered:";
			    ps_gen->print(Camgen::log);
			    Camgen::log<<endlog;
			}
			if(ps_gen->integrand()!=ps_gen->integrand())
			{
			    Camgen::log(log_level::warning)<<"Invalid phase space integrand encountered:";
			    amplitude->print(Camgen::log);
			    ps_gen->print(Camgen::log);
			    Camgen::log<<endlog;
			}
		    }
		    else
		    {
			ps_gen->integrand()=(value_type)0;
		    }
		    ps_gen->refresh_cross_section();
		    ps_gen->update();
		    if(uni_gen->generate())
		    {
			uni_gen->check();
			value_type w2=(uni_gen->pass())?uni_gen->weight():(value_type)0;
			w2sum+=w2;
			w2sqsum+=(w2*w2);
			amplitude->reset();
			uni_gen->integrand()*=std::norm(amplitude->evaluate())*sym_factor;
		    }
		    uni_gen->refresh_cross_section();
		    uni_gen->update();
		    if(i!=0 and i%N_batch==0)
		    {
			n=(value_type)i;
			value_type n2=n*(n-(value_type)1);
			psvol1=w1sum/n;
			psvol1err=std::sqrt(std::abs((w1sqsum-w1sum*w1sum/n)/n2));
			psvol2=w2sum/n;
			psvol2err=std::sqrt(std::abs((w2sqsum-w2sum*w2sum/n)/n2));
			xsec1=(ps_gen->cross_section()).value;
			xsec1err=(ps_gen->cross_section()).error;
			xsec2=(uni_gen->cross_section()).value;
			xsec2err=(uni_gen->cross_section()).error;
			data->fill();
		    }
		}
		data->write();
		data_stream* datstr1=new data_stream(data,"1","2","3");
		datstr1->title="MC";
		datstr1->style="yerrorbars";
		data_stream* datstr2=new data_stream(data,"1","4","5");
		datstr2->title="RAMBO";
		datstr2->style="yerrorbars";
		plot_script* psvolplot=new plot_script("Phase space volume");
		psvolplot->add_plot(datstr1);
		psvolplot->add_plot(datstr2);
		double dx=N_events/5;
		for(size_type i=0;i<5;++i)
		{
		    psvolplot->add_x_tic(i*dx);
		}

		data_stream* datstr3=new data_stream(data,"1","6","7");
		datstr3->title="MC";
		datstr3->style="yerrorbars";
		data_stream* datstr4=new data_stream(data,"1","8","9");
		datstr4->title="RAMBO";
		datstr4->style="yerrorbars";
		plot_script* xsecplot=new plot_script("Cross section");
		xsecplot->add_plot(datstr3);
		xsecplot->add_plot(datstr4);
		for(size_type i=0;i<5;++i)
		{
		    xsecplot->add_x_tic(i*dx);
		}

		multi_plot<1,2>plotall(filename,"postscript enhanced color");
		plotall.add_plot(psvolplot,0,0);
		plotall.add_plot(xsecplot,0,1);
		plotall.plot();
	    }

	    /* Destructor: */

	    ~ps_generator_tester()
	    {
		delete ps_gen;
		delete uni_gen;
	    }

	    /* Sets a minimal dimass: */

	    bool set_m_min(size_type i,size_type j,const value_type& x)
	    {
		bool q=true;
		if(i!=0 and j!=0 and i<=N_out and j<=N_out)
		{
		    q&=ps_gen->set_m_min(i,j,x);
		    q&=uni_gen->set_m_min(i,j,x);
		}
		return false;
	    }

	    /* Sets the minimal pT for the generators */

	    void set_pT_min(int i,const value_type& pTmin)
	    {
		ps_gen->set_pT_min(i,pTmin);
		uni_gen->set_pT_min(i,pTmin);
	    }

	    /* Sets the maximal angle for the generators */

	    void set_ct_max(int i,const value_type& ctmax)
	    {
		ps_gen->set_ct_max(i,ctmax);
		uni_gen->set_ct_max(i,ctmax);
	    }

	    /* Prints the amplitude: */

	    std::ostream& print_amplitude(std::ostream& os) const
	    {
		amplitude->print(os);
		return os;
	    }

	    /* Prints the phase space generator: */

	    std::ostream& print_generator(std::ostream& os) const
	    {
		ps_gen->print(os);
		return os;
	    }
    };
}

#endif /*PS_GEN_TESTER_H_*/

