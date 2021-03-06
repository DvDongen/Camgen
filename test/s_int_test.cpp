//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Testing facility for s-branching phase space integrals. *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Camgen/plt_config.h>
#include <Camgen/stdrand.h>
#include <Camgen/rn_strm.h>
#include <s_int_tester.h>

using namespace Camgen;

int main()
{
    std::cout<<"-------------------------------------------------------------------------"<<std::endl;
    std::cout<<"testing constrained density integrals...................................."<<std::endl;
    std::cout<<"-------------------------------------------------------------------------"<<std::endl;
    
    /* Some useful type definitions: */

    typedef double value_type;
    typedef std::random rng_type;

    typedef BW_s_generator<value_type,std::random> Breit_Wigner_type;
    typedef pl_s_generator<value_type,std::random> power_law_type;
    typedef uni_s_generator<value_type,std::random> uniform_type;
    typedef Dd_s_generator<value_type,std::random> Dirac_delta_type;
    
    //////////////////////////////////////////////////////////////////////////////

    /* Number of evaluated integrals: */
    
    std::size_t N_samples = 100;

    /* Monte Carlo points per batch: */

    std::size_t N_MC = 10000;
    
    /* Some useful constants: */
    
    //////////////////////////////////////////////////////////////////////////////

    value_type M_max	 = 500;
    value_type M1	 = 81.5;
    value_type W1	 = 2.5;
    value_type nu1	 = 1.05;
    value_type M_min1    = 5;
    value_type M2	 = 120;
    value_type W2	 = 5;
    value_type nu2	 = 1.75;
    value_type M_min2    = 10;
    
    //////////////////////////////////////////////////////////////////////////////

    {
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * Breit-Wigner("<<M2<<","<<W2<<")............";
	std::cerr.flush();
	std::string file("plots/BWBW1_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(M_min1);
	Breit_Wigner_type* channel2=new Breit_Wigner_type(&M2,&W2);
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }
    
    //////////////////////////////////////////////////////////////////////////////

    M1	 	= 15.;
    W1	 	= 200.;
    M2	 	= 100;
    W2	 	= 5;
    
    /////////////////////////////////////////////////////////////////////////////
    
    {
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * Breit-Wigner("<<M2<<","<<W2<<")............";
	std::cerr.flush();
	std::string file("plots/BWBW2_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(M_min1);
	Breit_Wigner_type* channel2=new Breit_Wigner_type(&M2,&W2);
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }
    
    //////////////////////////////////////////////////////////////////////////////

    M1	 	= 91.;
    W1	 	= 1.5;
    M2	 	= 91.;
    W2	 	= 1.5;
    
    /////////////////////////////////////////////////////////////////////////////
    
    {
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * Breit-Wigner("<<M2<<","<<W2<<")............";
	std::cerr.flush();
	std::string file("plots/BWBW3_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(0.5*M1);
	Breit_Wigner_type* channel2=new Breit_Wigner_type(&M2,&W2);
	channel2->set_m_min(0.5*M1);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    M1=81.5;
    M2=3.;
    
    //////////////////////////////////////////////////////////////////////////////

    {
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * power-law("<<M2<<","<<nu2<<")............";
	std::cerr.flush();
	std::string file("plots/BWpl_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(M_min1);
	power_law_type* channel2=new power_law_type(&M2,&nu2);
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }

    {
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * uniform()............";
	std::cerr.flush();
	std::string file("plots/BWuni_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(M_min1);
	uniform_type* channel2=new uniform_type();
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }
    
    {
	
	std::cerr<<"Checking integral Breit-Wigner("<<M1<<","<<W1<<") * Dirac-delta("<<M2<<")............";
	std::cerr.flush();
	std::string file("plots/BWDd_int");
	Breit_Wigner_type* channel1=new Breit_Wigner_type(&M1,&W1);
	channel1->set_m_min(M_min1);
	Dirac_delta_type* channel2=new Dirac_delta_type(&M2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    M1=5;
    M_min1=6;
    M2=2.5;
    M_min2=4;
    
    //////////////////////////////////////////////////////////////////////////////

    {
	std::cerr<<"Checking integral power-law("<<M1<<","<<nu1<<") * power-law("<<M2<<","<<nu2<<")............";
	std::cerr.flush();
	std::string file("plots/plpl_int");
	power_law_type* channel1=new power_law_type(&M1,&nu1);
	channel1->set_m_min(M_min1);
	power_law_type* channel2=new power_law_type(&M2,&nu2);
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }
    
    //////////////////////////////////////////////////////////////////////////////

    M2=120;
    
    //////////////////////////////////////////////////////////////////////////////

    {
	std::cerr<<"Checking integral power-law("<<M1<<","<<nu1<<") * uniform()............";
	std::cerr.flush();
	std::string file("plots/pluni_int");
	power_law_type* channel1=new power_law_type(&M1,&nu1);
	channel1->set_m_min(M_min1);
	uniform_type* channel2=new uniform_type();
	channel2->set_m_min(M_min2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M_min2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }

    {
	std::cerr<<"Checking integral power-law("<<M1<<","<<nu1<<") * Dirac_delta("<<M2<<")............";
	std::cerr.flush();
	std::string file("plots/plDd_int");
	power_law_type* channel1=new power_law_type(&M1,&nu1);
	channel1->set_m_min(M_min1);
	Dirac_delta_type* channel2=new Dirac_delta_type(&M2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1+M2,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }

    {
	std::cerr<<"Checking integral uniform() * Dirac_delta("<<M2<<")............";
	std::cerr.flush();
	std::string file("plots/uDd_int");
	uniform_type* channel1=new uniform_type();
	channel1->set_m_min(M_min1);
	Dirac_delta_type* channel2=new Dirac_delta_type(&M2);
	s_int_tester<value_type,std::random>tester(channel1,channel2);
	tester.run(M_min1,M_max,N_samples,N_MC,file,"postscript color");
	delete channel1;
	delete channel2;
	std::string output=plot_config::gnuplot_path==NULL?file+".dat/.gp":file+".eps";
	std::cerr<<"...........done, file "<<output<<" written."<<std::endl;
    }
}

