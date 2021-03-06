//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

/*! \file root_if.h
    \brief Root TTree output interface for phase space generators.
 */

#ifndef CAMGEN_ROOT_IF_H_
#define CAMGEN_ROOT_IF_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Root TTree output interface class implementation. It creates a root file and  *
 * implements the branching members as creators of branchings in the TTree.      *
 *                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <climits>
#include <Camgen/if_output.h>

/* External component, should be in the c-flags when compiling this code: */

#include <TFile.h>
#include <TTree.h>

namespace Camgen
{
    /* Numerical type conversion to root type identifyer utility class template: */

    template<class value_type,std::size_t bits=sizeof(value_type)*CHAR_BIT>class root_type_id;
    template<std::size_t N>class root_type_id<bool,N>
    {
	public:

	    static const char* id;
    };
    template<std::size_t N>const char* root_type_id<bool,N>::id="/O";
    template<>class root_type_id<int,16>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<int,16>::id="/S";
    template<>class root_type_id<int,32>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<int,32>::id="/I";
    template<>class root_type_id<int,64>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<int,64>::id="/L";
    template<>class root_type_id<float,32>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<float,32>::id="/F";
    template<>class root_type_id<float,64>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<float,64>::id="/D";
    template<>class root_type_id<double,64>
    {
	public:

	    static const char* id;
    };
    const char* root_type_id<double,64>::id="/D";
    
    /// ROOT TTree output interface class.
    
    template<class model_t>class root_tree: public interface_output<model_t>
    {
	typedef interface_output<model_t> base_type;

	public:

	    /* Type definitions: */

	    typedef model_t model_type;
	    typedef typename base_type::value_type value_type;
	    typedef typename base_type::momentum_type momentum_type;

	    /// ROOT tree name.

	    const std::string tree_name;

	    /// Constructor with file name, tree name and tree description arguments.

	    root_tree(const std::string& file_name_="output",
		      const std::string tree_name_="output_tree",
		      const std::string description_="no description available"):base_type(file_name_,description_),tree_name(tree_name_),rootfile(NULL),roottree(NULL){}

	    /* Destructor. */

	    ~root_tree()
	    {
		if(rootfile!=NULL)
		{
		    std::cout<<"Root file was not written to disk...do you wish to write the output file? (y/n)";
		    char answer='n';
		    std::cin>>answer;
		    if(answer=='y')
		    {
			rootfile->Write();
			rootfile->Close();
		    }
		    delete rootfile;
		}
	    }

	    /* Factory method implementation: */

	    base_type* create(const std::string& file_name_) const
	    {
		return new root_tree<model_t>(file_name_,tree_name,this->description);
	    }

	    /* Opens the root file. */

	    bool open_file()
	    {
		if(rootfile==NULL)
		{
		    std::string fname=this->file_name+".root";
		    rootfile=new TFile(fname.c_str(),"RECREATE");
		}
		if(roottree==NULL)
		{
		    roottree=new TTree(tree_name.c_str(),(this->description).c_str());
		}
		return rootfile->IsOpen();
	    }

	    /* Closes the .root file. */

	    bool close_file()
	    {
		if(rootfile!=NULL)
		{
		    rootfile->Write();
		    rootfile->Close();
		    delete rootfile;
		    rootfile=NULL;
		}
		return true;
	    }

	    /* Inserts a branch holding a momentum: */

	    bool branch(const momentum_type* p,const std::string& name)
	    {
		if(rootfile==NULL)
		{
		    return false;
		}
		if(!rootfile->IsOpen() or roottree==NULL)
		{
		    return false;
		}
		std::stringstream ss;
		ss<<name<<'['<<p->size()<<']'<<(root_type_id<typename momentum_type::value_type>::id);
		std::string leafname=ss.str();
		roottree->Branch(name.c_str(),const_cast<value_type*>(p->data),leafname.c_str());
		return true;
	    }

	    /* Inserts a branch holding a floating-point number: */

	    bool branch(const value_type* x,const std::string& name)
	    {
		if(rootfile==NULL)
		{
		    return false;
		}
		if(!rootfile->IsOpen() or roottree==NULL)
		{
		    return false;
		}
		std::string leafname(name+root_type_id<value_type>::id);
		roottree->Branch(name.c_str(),const_cast<value_type*>(x),leafname.c_str());
		return true;
	    }

	    /* Inserts a branch holding an integer: */

	    bool branch(const int* n,const std::string& name)
	    {
		if(rootfile==NULL)
		{
		    return false;
		}
		if(!rootfile->IsOpen() or roottree==NULL)
		{
		    return false;
		}
		std::string leafname(name+root_type_id<int>::id);
		roottree->Branch(name.c_str(),const_cast<int*>(n),leafname.c_str());
		return true;
	    }

	    /* Inserts a branch holding a boolean: */

	    bool branch(const bool* n,const std::string& name)
	    {
		if(rootfile==NULL)
		{
		    return false;
		}
		if(!rootfile->IsOpen() or roottree==NULL)
		{
		    return false;
		}
		std::string leafname(name+root_type_id<bool>::id);
		roottree->Branch(name.c_str(),const_cast<bool*>(n),leafname.c_str());
		return true;
	    }

	    /* Writes the event to the ROOT tree. */

	    bool write_event()
	    {
		if(rootfile==NULL)
		{
		    return false;
		}
		if(!rootfile->IsOpen() or roottree==NULL)
		{
		    return false;
		}
		roottree->Fill();
		return true;
	    }

	private:

	    TFile* rootfile;
	    TTree* roottree;
    };
}

#endif /*CAMGEN_ROOT_TTREE*/

