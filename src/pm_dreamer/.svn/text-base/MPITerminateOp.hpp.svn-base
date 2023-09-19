/***************************************************************************
                             MPITerminateOp.hpp
                             -------------------
                               W. Michael Brown

  Termination on all procs if criteria met on 1

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun 12 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef MPITerminateOp_hpp
#define MPITerminateOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"


namespace Beagle {

/// Termination on all procs if criteria met on 1
class MPITerminateOp : public Operator {
 public:
  typedef AllocatorT<MPITerminateOp,Operator::Alloc>
          Alloc;
  typedef PointerT<MPITerminateOp,Operator::Handle>
          Handle;
  typedef ContainerT<MPITerminateOp,Operator::Bag>
          Bag;

  explicit MPITerminateOp(string inName="MPITerminateOp");
  virtual ~MPITerminateOp() { }

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);

 protected:
  bool terminate_flag;
  UIntArray::Handle mPopSize;            //!< Population size for the evolution.
};

}

#endif
