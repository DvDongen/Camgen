//
// This file is part of the CAMGEN library.
// Copyright (C) 2013 Gijs van den Oord.
// CAMGEN is licensed under the GNU GPL, version 2,
// see COPYING for details.
//

#ifndef QCDPBCHABDC_H_
#define QCDPBCHABDC_H_

#include <Camgen/QCD_base.h>
#include <Camgen/Minkowski.h>
#include <Camgen/Pauli_basis.h>
#include <Camgen/hel_type.h>
#include <Camgen/adjoint.h>

namespace Camgen
{
    class QCDPbchabdc: public QCD_base<QCDPbchabdc,double>
    {
	public:
	    typedef double value_type;
	    typedef Minkowski_type spacetime_type;
	    typedef Pauli_basis Dirac_algebra_type;
	    typedef helicity_type spin_vector_type;
	    typedef adjoint colour_treatment;
	    
	    static const std::size_t dimension=4;
	    static const bool coloured=true;
	    static const bool continuous_helicities=true;
	    static const bool continuous_colours=false;
	    static const int beam_direction=3;
	    static const std::size_t N_c=3;

	    QCDPbchabdc();
    };
}

#endif /*QCDPBCHABDC_H_*/

