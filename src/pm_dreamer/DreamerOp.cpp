/***************************************************************************
                               DreamerOp.cpp
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

#include "base_potential.h"
#include "beagle/Beagle.hpp"
#include "DreamerOp.hpp"

#ifdef GPMPI
#include "mpi.h"
#endif

using namespace Beagle;

DreamerOp::DreamerOp(Operator::Handle terminate_op, Operator::Handle s_op,
                     MutationOp::Handle hybrid_op, MutationOp::Handle olsop,
                     MutationOp::Handle rmsd_op, EvaluationOp::Handle ifit_op,
                     EvaluationOp::Handle irmsd_fit_op, int eflag, int fflag,
                     Beagle::string inName) :
  Operator(inName), term_op(terminate_op), stats_op(s_op), hyb_op(hybrid_op),
  ols_op(olsop), rmsd_hyb_op(rmsd_op), fit_op(ifit_op), 
  rmsd_fit_op(irmsd_fit_op), energy_flag(eflag), force_flag(fflag)  {

  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_islands);
  #endif
}

void DreamerOp::initialize(System& ioSystem) {
  Beagle_StackTraceBeginM();
  Operator::initialize(ioSystem);
  term_op->initialize(ioSystem);
  stats_op->initialize(ioSystem);
  hyb_op->initialize(ioSystem);
  ols_op->initialize(ioSystem);
  rmsd_hyb_op->initialize(ioSystem);
  fit_op->initialize(ioSystem);
  rmsd_fit_op->initialize(ioSystem);
  Beagle_StackTraceEndM("void DreamerOp::initialize(System& ioSystem)");
}

void DreamerOp::postInit(System& ioSystem) {
  Beagle_StackTraceBeginM();
  Operator::postInit(ioSystem);
  term_op->postInit(ioSystem);
  stats_op->postInit(ioSystem);
  hyb_op->postInit(ioSystem);
  ols_op->postInit(ioSystem);
  rmsd_hyb_op->postInit(ioSystem);
  fit_op->postInit(ioSystem);
  rmsd_fit_op->postInit(ioSystem);
  Beagle_StackTraceEndM("void DreamerOp::postInit(System& ioSystem)");
}

void DreamerOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  term_op->operate(ioDeme,ioContext);
  if (ioContext.getContinueFlag()==false) {
    if (my_rank==0)
      std::cout << "---------------------------------------------------------\n"
                << "Termination criterion met.\n";

    Individual::Handle ind=ioContext.getVivarium().getHallOfFame()[0].mIndividual;
    ioContext.setIndividualHandle(ind);
    ioContext.setIndividualIndex(0);

    // Adjust registers
    UInt::Handle simplify = castHandleT<UInt>(ioContext.getSystem().getRegister().getEntry("gp.hybopt.simplify"));
    UInt::Handle hybtype = castHandleT<UInt>(ioContext.getSystem().getRegister().getEntry("gp.hybopt.type"));
    UInt::Handle maxi = castHandleT<UInt>(ioContext.getSystem().getRegister().getEntry("gp.hybopt.maxi"));
    UInt::Handle s_inter=castHandleT<UInt>(ioContext.getSystem().getRegister().getEntry("gp.simplify.interval"));
    UInt::Handle t_inter=castHandleT<UInt>(ioContext.getSystem().getRegister().getEntry("gp.olscorrect.interval"));
    assert(hybtype!=NULL && simplify!=NULL && maxi!=NULL && s_inter!=NULL &&
           t_inter!=NULL);

    UInt old_inter=*t_inter;
    UInt old_simplify=*simplify;
    UInt old_hybtype=*hybtype;
    UInt old_maxi=*maxi;
    UInt old_sinter=*s_inter;
    *t_inter=1;
    *simplify=1;
    *hybtype=0;
    *maxi=100;
    *s_inter=1;

    Fitness::Handle fitf;
    FitnessSimple::Handle fitp; 
    
    // Simplification and optimization
    if (my_rank==0) {
      std::cout << " Performing simplification and optimization...";
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << "-->";
    }
    hyb_op->mutate(*ind,ioContext);
    ind=ioContext.getVivarium().getHallOfFame()[0].mIndividual;
    ioContext.setIndividualHandle(ind);
    ioContext.setIndividualIndex(0);
    if (my_rank==0) {
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << std::endl;
    }

    // OLS Correction
    fitf=rmsd_fit_op->evaluate(*ind,ioContext);
    ind->setFitness(fitf);
    ind->getFitness()->setValid();
    if (my_rank==0) {
      //std::cout << "Before: " << ind->serialize() << std::endl;
      std::cout << " Performing OLS correction...";
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << "-->";
    }
    ols_op->mutate(*ind,ioContext);
    fitf=rmsd_fit_op->evaluate(*ind,ioContext);
    ind->setFitness(fitf);
    ind->getFitness()->setValid();
    if (my_rank==0) {
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << std::endl;
      //std::cout << "After: " << ind->serialize() << std::endl;
    }
    
    // Simplification and optimization
    fitf=rmsd_fit_op->evaluate(*ind,ioContext);
    ind->setFitness(fitf);
    ind->getFitness()->setValid();
    if (my_rank==0) {
      std::cout << " Performing simplification and optimization (RMSD)...";
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << "-->";
    }
    rmsd_hyb_op->mutate(*ind,ioContext);
    ind=ioContext.getVivarium().getHallOfFame()[0].mIndividual;
    ioContext.setIndividualHandle(ind);
    ioContext.setIndividualIndex(0);
    if (my_rank==0) {
      fitp = castHandleT<FitnessSimple>(ind->getFitness());
      std::cout << fitp->getValue() << std::endl << std::endl;
    }
        
    // Replace Fitness
    fitf=fit_op->evaluate(*ind,ioContext);
    ind->setFitness(fitf);
    ind->getFitness()->setValid();

    // Sync processors
    stats_op->operate(ioDeme,ioContext);
    ind=ioContext.getVivarium().getHallOfFame()[0].mIndividual;
    ioContext.setIndividualHandle(ind);
    ioContext.setIndividualIndex(0);

    // RMSD
    if (my_rank==0) {
      rmsd_fit_op->evaluate(*ind,ioContext);
      if (energy_flag) {
        double rmsd=castHandleT<BasePotentialOp>(rmsd_fit_op)->energy_rmsd();
        double range=castHandleT<BasePotentialOp>(rmsd_fit_op)->energy_range();
        std::cout << "  Energy RMSD: " << rmsd << " (%" << rmsd/range*100.0 
                  << ")\n";
      }
      if (force_flag) {
        double rmsd=castHandleT<BasePotentialOp>(rmsd_fit_op)->force_rmsd();
        double range=castHandleT<BasePotentialOp>(rmsd_fit_op)->force_range();
        std::cout << "  Force RMSD: " << rmsd << " (%" << rmsd/range*100.0 
                  << ")\n";
      }
    }
    
    // Replace register values
    *t_inter=old_inter;
    *simplify=old_simplify;
    *hybtype=old_hybtype;
    *maxi=old_maxi;
    *s_inter=old_sinter;
  } else
    stats_op->operate(ioDeme,ioContext);
  if (ioContext.getContinueFlag()==false && my_rank==0)
    std::cout << "---------------------------------------------------------\n";
  Beagle_StackTraceEndM("void DreamerOp::operate(Deme& ioDeme, Context& ioContext)");
}
