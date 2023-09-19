/***************************************************************************
                              pair_potential.h
                             -------------------
                               W. Michael Brown

  Fitness evaluation for pair potentials without vectorization

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

#ifndef PAIR_POTENTIAL_H
#define PAIR_POTENTIAL_H

#include "base_pair.h"

/// Fitness Function for Pair Potentials
class PairPotentialOp : public BasePairOp {
 public:
  typedef Beagle::AllocatorT<PairPotentialOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<PairPotentialOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<PairPotentialOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  PairPotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                  int energy_flag, int force_flag);
  PairPotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                  int energy_flag, int force_flag, const PairPotentialOp &in);
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext);
  void pack_simplify(unsigned, Double_matrix &, primitive_vector &, 
                     Double_matrix &, primitive_vector &, Beagle::GP::System&){}

protected:
  PairPotentialOp() {}
    
  void pack_data() {}
};

#endif
