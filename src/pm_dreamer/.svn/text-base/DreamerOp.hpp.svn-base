/***************************************************************************
                               DreamerOp.hpp
                             -------------------
                               W. Michael Brown

  Combines MPITerminateOp, StatsCalcFitSimpleMPIOp and performs end
  of optimization simplification and optimization using HybridOptOp and
  SimplifyOp

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Nov 08 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef DreamerOp_hpp
#define DreamerOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/GP.hpp"

namespace Beagle {

/// Termination on all procs if criteria met on 1
class DreamerOp : public Operator {
 public:
  typedef AllocatorT<DreamerOp,Operator::Alloc>
          Alloc;
  typedef PointerT<DreamerOp,Operator::Handle>
          Handle;
  typedef ContainerT<DreamerOp,Operator::Bag>
          Bag;

  explicit DreamerOp(Operator::Handle terminate_op, 
                     Operator::Handle statistics_op, 
                     MutationOp::Handle hybrid_op, 
                     MutationOp::Handle ols_op, MutationOp::Handle rmsd_op,
                     EvaluationOp::Handle ifit_op,
                     EvaluationOp::Handle irmsd_fit_op, int eflag, int fflag,
                     string inName="DreamerOp");
  virtual ~DreamerOp() { }

  virtual void initialize(System& ioSystem);
  virtual void postInit(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);

 protected:
  int my_rank,num_islands;  
  Operator::Handle term_op, stats_op;
  MutationOp::Handle hyb_op, ols_op, rmsd_hyb_op;
  EvaluationOp::Handle fit_op, rmsd_fit_op;
  int energy_flag, force_flag;
};

}

#endif
