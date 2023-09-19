/***************************************************************************
                              TermMaxTimeOp.cpp
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

#include "beagle/Beagle.hpp"
#include "TermMaxTimeOp.hpp"

#ifdef GPMPI
#include "mpi.h"
#endif

using namespace Beagle;

TermMaxTimeOp::TermMaxTimeOp(Beagle::string inName) :
  TerminationOp(inName), my_rank(0), num_islands(1)
{ 
  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_islands);
  #endif
}

void TermMaxTimeOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.term.maxtime")) {
    mMaxTime =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.term.maxtime"));
  } else {
    mMaxTime = new UInt(60);
    Register::Description lDescription(
      "Maximum time in minutes term criterion",
      "UInt",
      "60",
      "Maximum number of minutes for the evolution."
    );
    ioSystem.getRegister().addEntry("ec.term.maxtime", mMaxTime, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }
  #ifdef GPMPI
  timestart=MPI_Wtime();
  #else
  timestart = clock();
  #endif
  
  Beagle_StackTraceEndM("void TermMaxTimeOp::initialize(System& ioSystem)");
}


bool TermMaxTimeOp::terminate(const Deme& inDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if (ioContext.getDemeIndex() == (mPopSize->size()-1)) {
    if (mMaxTime->getWrappedValue()==0)
      return false;
    float sec_passed;
    unsigned min_passed;
    #ifdef GPMPI
    sec_passed=MPI_Wtime()-timestart;
    #else
    clock_t timeend=clock();
    sec_passed=(timeend-timestart)/static_cast<double>(CLOCKS_PER_SEC);
    #endif
    min_passed=static_cast<unsigned>(sec_passed/60.0);
    if (my_rank==0)
      std::cout << "  Time: " << sec_passed << std::endl;
  
    if(min_passed > mMaxTime->getWrappedValue()) {
      Beagle_LogInfoM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::TermMaxTimeOp",
        std::string("Maximum number of minutes (") +
        uint2str(mMaxTime->getWrappedValue()) +
        std::string(") termination criterion overpassed")
      );
      Beagle_LogDetailedM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::TermMaxTimeOp",
        std::string("Actual minutes is: ")+uint2str(ioContext.getGeneration())
      );
      return true;
    }
    if((min_passed == mMaxTime->getWrappedValue()) &&
       (ioContext.getDemeIndex() == (ioContext.getVivarium().size()-1))) {
      Beagle_LogInfoM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::TermMaxTimeOp",
        std::string("Maximum number of minutes (") +
        uint2str(mMaxTime->getWrappedValue()) +
        std::string(") termination criterion reached")
      );
      return true;
    }
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "termination", "Beagle::TermMaxTimeOp",
      std::string("Maximum number of minutes (") +
      uint2str(mMaxTime->getWrappedValue()) +
      std::string(") termination criterion not reached ")
    );
  }
  return false;
  Beagle_StackTraceEndM("bool TermMaxTimeOp::terminate(const Deme& inDeme, Context& ioContext)");
}
