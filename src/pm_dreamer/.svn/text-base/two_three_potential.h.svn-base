/***************************************************************************
                            two_three_potential.h
                             -------------------
                               W. Michael Brown

  Fitness evaluation for 3-body potentials

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

#ifndef TWO_THREE_POTENTIAL_H
#define TWO_THREE_POTENTIAL_H

#include "base_twothree.h"

/// Fitness Function for Pair Potentials
class TwoThreePotentialOp : public BaseTwoThreeOp {
 public:
  typedef Beagle::AllocatorT<TwoThreePotentialOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<TwoThreePotentialOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<TwoThreePotentialOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  TwoThreePotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                      int energy_flag, int force_flag);
  TwoThreePotentialOp(Error *error, FitStat *fse, FitStat *fsf, 
                      int energy_flag, int force_flag,
                      const TwoThreePotentialOp &in);
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext);
  void pack_simplify(unsigned, Double_matrix &, primitive_vector &, 
                     Double_matrix &, primitive_vector &, Beagle::GP::System&){}

 protected:
  TwoThreePotentialOp() {} // Filename required
  void pack_data() {}
};

#endif
