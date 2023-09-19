/***************************************************************************
                              vpair_potential.h
                             -------------------
                               W. Michael Brown

  Fitness evaluation for pair potentials with force calculation

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun 6 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef VPAIR_POTENTIAL_H
#define VPAIR_POTENTIAL_H

#include "base_pair.h"

/// Fitness Function for Pair Potentials
class VPairPotentialOp : public BasePairOp {
 public:
  typedef Beagle::AllocatorT<VPairPotentialOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<VPairPotentialOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<VPairPotentialOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  VPairPotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                   int energy_flag, int force_flag);
  VPairPotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                   int energy_flag,int force_flag,const VPairPotentialOp &in);
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext);
  void pack_simplify(unsigned, Double_matrix &, primitive_vector &, 
                     Double_matrix &, primitive_vector &, Beagle::GP::System&);
protected:
  VPairPotentialOp() {}

  // Vector for total processing
  Beagle::VDouble vector_x;
  
  // Indices into vector x to calculate the energies and forces for each config
  int_vector vstart_e;
  int_vector vstart_f;
  int_vector vstart_fx;
  int_vector vstart_fy;
  int_vector vstart_fz;
 
  void pack_data();
};

#endif
