//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#include <Camgen/phi3.h>
#include <Camgen/CM_algo.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Program checking the Camgen process tree by counting the number of Feynman *
 * diagrams for a phi^3-theory hidden in the tree and comparing with the       *
 * explicit formula (3.4) in P. Dragiottis PhD thesis "Exploding QCD".         *
 *                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace Camgen
{
    namespace test_utils
    {

	/* Checking class, computing recursively the number of diagrams in 2->n
	 * processes in phi^3-theory, where the template parameter denotes the maximum
	 * number of external particles: */

	template<std::size_t N>class n_diagrams3
	{
	    public:

		/* Initialisation phase, computing all multiplicities up to N external
		 * particles: */

		static void initialise()
		{
		    if(!initialised)
		    {
			factorials[0]=1;
			factorials[1]=1;
			data[1]=1;
			for(std::size_t n=2;n<N;++n)
			{
			    factorials[n]=n*factorials[n-1];
			    for(std::size_t i=1;i<n;++i)
			    {
				data[n]+=(data[i]*data[n-i]*factorials[n]/(factorials[i]*factorials[n-i]));
			    }
			    data[n]/=2;
			}
			initialised=true;
		    }
		}

		/* Access the number of diagrams for i external particles: */

		static long long unsigned get_multiplicity(std::size_t i)
		{
		    if(i==0)
		    {
			return 0;
		    }
		    if(i<N)
		    {
			return data[i-1];
		    }
		    else
		    {
			std::cout<<"Number of requested legs out of range..."<<std::endl;
			return 0;
		    }
		}

		/* Printing facility: */

		static std::ostream& print(std::ostream& os)
		{
		    for(std::size_t n=0;n<N-1;++n)
		    {
			os<<data[n]<<",";
		    }
		    os<<data[N-1];
		    return os;
		}

	    private:

		/* Storage of the diagram counts: */

		static long long unsigned data[N];

		/* Utility storage of factorials: */

		static long long unsigned factorials[N];

		/* Initialisation tag: */

		static bool initialised;
	};

	template<std::size_t N>long long unsigned n_diagrams3<N>::data[]={0};
	template<std::size_t N>long long unsigned n_diagrams3<N>::factorials[]={0};
	template<std::size_t N>bool n_diagrams3<N>::initialised=false;

	template<int N,int M>bool check_phi3_graphs(std::ostream& os=std::cerr)
	{
	    if(N>=M-2)
	    {
		os<<"Skipped check of "<<N<<" final state particles with diagram counter class of max. multiplicity "<<M-2<<std::endl;
		return false;
	    }
	    os<<"Checking the number of 2->"<<N<<" diagrams..........";
	    n_diagrams3<M>::initialise();
	    os.flush();
	    std::string process="phi,phi > ";
	    for(int i=0;i<N;++i)
	    {
		process.append("phi,");
	    }
	    process.resize(process.size()-1);
	    CM_algorithm<phi3,2,N>algo(process);
	    algo.load();
	    algo.construct();
	    if(algo.count_diagrams()!=n_diagrams3<M>::get_multiplicity(2+N))
	    {
		os<<"mismatch encountered--camgen result: "<<algo.count_diagrams();
		os<<", formula-based result: "<<n_diagrams3<M>::get_multiplicity(2+N)<<std::endl;
		return false;
	    }
	    os<<"..........done, counted correctly "<<n_diagrams3<M>::get_multiplicity(2+N)<<" diagrams"<<std::endl;
	    return true;
	}
    }
}

using namespace Camgen;

int main()
{
    license_print::disable();
    std::cout<<"----------------------------------------------------------"<<std::endl;
    std::cout<<"testing tree construction by phi^3 diagram counting......."<<std::endl;
    std::cout<<"----------------------------------------------------------"<<std::endl;

    if(!test_utils::check_phi3_graphs<2,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<3,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<4,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<5,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<6,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<7,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<8,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<9,13>(std::cerr))
    {
	return 1;
    }
    if(!test_utils::check_phi3_graphs<10,13>(std::cerr))
    {
	return 1;
    }
}

