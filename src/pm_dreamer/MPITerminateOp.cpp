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

#include "beagle/Beagle.hpp"
#include "MPITerminateOp.hpp"

#ifdef GPMPI
#include "mpi.h"
#endif

using namespace Beagle;

MPITerminateOp::MPITerminateOp(Beagle::string inName) :
  Operator(inName), terminate_flag(false)
{ }

void MPITerminateOp::initialize(System& ioSystem) {
  Beagle_StackTraceBeginM();
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
  Beagle_StackTraceEndM("void MPITerminateOp::initialize(System& ioSystem)");
}

void MPITerminateOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "termination", "Beagle::MPITerminateOp",
    string("Checking if \"")+getName()+
    string("\" termination criterion reached")
  );
  
  if (ioContext.getDemeIndex()==mPopSize->size()-1) {
    if (terminate_flag)
      ioContext.setContinueFlag(false);
    #ifdef GPMPI
    int terminate;
    if (ioContext.getContinueFlag())
      terminate=0;
    else
      terminate=1;
    int answer;
    MPI_Allreduce(&terminate,&answer,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    if(answer>0) {
      Beagle_LogInfoM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::MPITerminateOp",
        "Termination criterion reached"
      );
      ioContext.setContinueFlag(false);
    }
    #endif
  } else
    if (ioContext.getContinueFlag()==false) {
      terminate_flag=true;
      ioContext.setContinueFlag(true);
    }
  
  Beagle_StackTraceEndM("void MPITerminateOp::operate(Deme& ioDeme, Context& ioContext)");
}
