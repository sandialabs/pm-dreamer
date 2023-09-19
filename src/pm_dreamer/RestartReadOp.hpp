/***************************************************************************
                                RestartReadOp
                             -------------------
                               W. Michael Brown

  Only read the vivarium from a milestone file

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : July 31 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef RESTART_READ_OP_hpp
#define RESTART_READ_OP_hpp

#include "beagle/MilestoneReadOp.hpp"

namespace Beagle {

class RestartReadOp : public MilestoneReadOp {
 public:
  typedef AllocatorT<RestartReadOp,Operator::Alloc>
          Alloc;
  typedef PointerT<RestartReadOp,Operator::Handle>
          Handle;
  typedef ContainerT<RestartReadOp,Operator::Bag>
          Bag;

  enum ParseCommands { 
    eParseMilestone = 1,
    eParseSystem    = 2,
    eParseEvolver   = 4,
    eParseRegister  = 8,
    eParseVivarium  = 16,
    eParseAll = 255 
  };

  explicit inline RestartReadOp(EvaluationOp::Handle fitguy,string inName="RestartReadOp") : 
    MilestoneReadOp(inName), fit_guy(fitguy) {}
  virtual ~RestartReadOp() { }

  inline void readMilestone(string inFilename, Context& ioContext, 
                            int inCommands) {
    Beagle_StackTraceBeginM();
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "readmilestone", "Beagle::ReadMilestoneOp",
      string("Reading milestone file and resetting fitness."));
    MilestoneReadOp::readMilestone(inFilename,ioContext,eParseVivarium);
    
    // Re-evaluate the fitness for all HOF members
    for (int i=0; i<ioContext.getVivarium().getHallOfFame().size(); i++) {
      ioContext.setIndividualHandle(ioContext.getVivarium().getHallOfFame()[i].mIndividual);
      ioContext.getVivarium().getHallOfFame()[i].mIndividual->
        setFitness(fit_guy->evaluate(*ioContext.getVivarium().getHallOfFame()[i].mIndividual,ioContext));
    }
    ioContext.getVivarium().getHallOfFame().sort();
    
    // Re-evaluate the fitness for all individuals
    for (int i=0; i<ioContext.getVivarium().size(); i++) {
      ioContext.setDemeIndex(i);
      for (int j=0; j<ioContext.getVivarium()[i]->size(); j++) {
        ioContext.setIndividualIndex(j);
        ioContext.setIndividualHandle((*ioContext.getVivarium()[i])[j]);
        (*ioContext.getVivarium()[i])[j]->
               setFitness(fit_guy->evaluate(*(*ioContext.getVivarium()[i])[j],
                                            ioContext));
        (*ioContext.getVivarium()[i])[j]->getFitness()->setValid();
      }
      ioContext.getVivarium().getHallOfFame().updateWithDeme(
                 ioContext.getVivarium().getHallOfFame().size(),
                 *ioContext.getVivarium()[i],ioContext);
      ioContext.getVivarium()[i]->getStats()->setInvalid();
    }
    ioContext.getVivarium().getStats()->setInvalid();
    ioContext.setDemeIndex(0);
    ioContext.setIndividualIndex(0);
    ioContext.setIndividualHandle((*ioContext.getVivarium()[0])[0]);

    Beagle_StackTraceEndM("void RestartReadOp::operate(Deme&,Context&)");
  }

private:
  String::Handle mMilestoneName;  //!< Name of the milestone to read.
  EvaluationOp::Handle fit_guy;
};

}

#endif // RestartReadOp
