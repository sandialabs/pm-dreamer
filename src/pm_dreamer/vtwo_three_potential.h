/***************************************************************************
                            vtwo_three_potential.h
                             -------------------
                               W. Michael Brown

  Fitness evaluation for 3-body potentials
  
 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : July 28 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef VTWO_THREE_POTENTIAL_H
#define VTWO_THREE_POTENTIAL_H

#include "base_twothree.h"

/// Fitness Function for Pair Potentials
class VTwoThreePotentialOp : public BaseTwoThreeOp {
 public:
  typedef Beagle::AllocatorT<VTwoThreePotentialOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<VTwoThreePotentialOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<VTwoThreePotentialOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  VTwoThreePotentialOp(Error *error, FitStat *fse, FitStat *fsf,
                       int energy_flag, int force_flag);
  VTwoThreePotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                       int energy_flag, int force_flag,
                       const VTwoThreePotentialOp &in);
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext);
  void pack_simplify(unsigned, Double_matrix &, primitive_vector &, 
                     Double_matrix &, primitive_vector &, Beagle::GP::System&);

protected:
  VTwoThreePotentialOp() {} // Filename required

  // Vectors for total processing
  Beagle::VDouble vector_x,vector_r1,vector_r2,vector_a;
  
  // Indices into vector x to calculate the energies and forces for each config
  int_vector vstart_e, vstart_e3;
  int_vector vstart_f, vstart_f3;
  int_vector vstart_fx, vstart_fx3;
  int_vector vstart_fy, vstart_fy3;
  int_vector vstart_fz, vstart_fz3;
  
  void pack_data();
};

#endif
