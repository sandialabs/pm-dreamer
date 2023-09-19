/***************************************************************************
                              TermMaxTimeOp.hpp
                             -------------------
                               W. Michael Brown

  Termination operator based on a maximum MPI Wall Time

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jul 8 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef TermMaxTimeOp_hpp
#define TermMaxTimeOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Context.hpp"
#include "beagle/System.hpp"

namespace Beagle {

class TermMaxTimeOp : public TerminationOp {
 public:
  typedef AllocatorT<TermMaxTimeOp,TerminationOp::Alloc>
          Alloc;
  typedef PointerT<TermMaxTimeOp,TerminationOp::Handle>
          Handle;
  typedef ContainerT<TermMaxTimeOp,TerminationOp::Bag>
          Bag;

  explicit TermMaxTimeOp(string inName="TermMaxTimeOp");
  virtual ~TermMaxTimeOp() { }

  virtual void initialize(System& ioSystem);
  virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:
  UInt::Handle mMaxTime;   //!< Maximum number of minutes of the evolution.
  #ifdef GPMPI
  double timestart;
  #else
  clock_t timestart;
  #endif
  int my_rank,num_islands;
  UIntArray::Handle mPopSize;            //!< Population size for the evolution.
};

}

#endif
