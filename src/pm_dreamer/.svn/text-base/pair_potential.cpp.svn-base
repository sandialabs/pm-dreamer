/***************************************************************************
                              pair_potential.cpp
                             -------------------
                               W. Michael Brown

  Fitness evaluation for pair potentials

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

#include "pair_potential.h"

using namespace Beagle;

PairPotentialOp::PairPotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                 int energy_flag, int force_flag) :
  BasePairOp(errin,fse,fsf,energy_flag,force_flag)
{ }

PairPotentialOp::PairPotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                 int energy_flag, int force_flag,
                                 const PairPotentialOp &in) :
  BasePairOp(errin,fse,fsf,energy_flag,force_flag) {
  epsilon_p=in.epsilon_p;
  pm_copy(eval_dm,in.eval_dm);
  pm_copy(eval_mY,in.eval_mY);
  _valid_data=in._valid_data;
  data=in.data;
  
  _num_types=in._num_types;
  _num_terms=in._num_terms;
  _num_terms3=in._num_terms3;
  
  if (_valid_data)
    init_fit_stat();
}

Fitness::Handle PairPotentialOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
  for (int i=0; i<data.num_configs; i++) {
    double energy = 0.0;
    double energyf=0;
    double energyfx=0, energyfy=0, energyfz=0;
    int nR;
    if (_energy_flag) {
      nR=data.mR[i].size();
      for (int j=0; j<nR; j++) {
        setValue("X", data.mR[i][j], ioContext);
        Double lResult;
        inIndividual.run(lResult, ioContext);
        energy+=lResult;
      }
    }
    if (_force_flag) {
      nR=data.mRF[i].size();
      for (int j=0; j<nR; j++) {
        setValue("X", data.mRF[i][j], ioContext);
        Double lResult;
        inIndividual.run(lResult, ioContext);
        energyf+=lResult;
        if (j==data.ghost_mRF[i]-1)
          energy+=energyf;
        setValue("X", data.dmRF_x[i][j], ioContext);
        inIndividual.run(lResult, ioContext);
        energyfx+=lResult;
        if (_force_flag==1) {
          setValue("X", data.dmRF_y[i][j], ioContext);
          inIndividual.run(lResult, ioContext);
          energyfy+=lResult;
          setValue("X", data.dmRF_z[i][j], ioContext);
          inIndividual.run(lResult, ioContext);
          energyfz+=lResult;
        }
      }
    }
    eval_mY[i]=energy;
    if (_force_flag==1) {
      eval_dm[i*3]=(energyf-energyfx)/epsilon_p;
      eval_dm[i*3+1]=(energyf-energyfy)/epsilon_p;
      eval_dm[i*3+2]=(energyf-energyfz)/epsilon_p;
    } else if (_force_flag==2)
      eval_dm[i]=(energyf-energyfx)/epsilon_p;
  }
  unsigned totaln=inIndividual.getTotalNodes();
  return fit_stat(totaln);
}
