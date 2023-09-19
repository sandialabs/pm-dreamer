/***************************************************************************
                           two_three_potential.cpp
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

#include "two_three_potential.h"

using namespace Beagle;

TwoThreePotentialOp::TwoThreePotentialOp(Error *errin,FitStat *fse,FitStat *fsf, 
                                         int energy_flag, int force_flag) :
  BaseTwoThreeOp(errin,fse,fsf,energy_flag,force_flag)
{ }

TwoThreePotentialOp::TwoThreePotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                         int energy_flag, int force_flag,
                                         const TwoThreePotentialOp &in) :
  BaseTwoThreeOp(errin,fse,fsf,energy_flag,force_flag) {
  epsilon_p=in.epsilon_p;
  pm_copy(eval_dm,in.eval_dm);
  pm_copy(eval_mY,in.eval_mY);
  _valid_data=in._valid_data;
  data=in.data;
  pm_copy(eval_mY3,in.eval_mY3);
  pm_copy(eval_dm3,in.eval_dm3);
  
  _num_types=in._num_types;
  _num_terms=in._num_terms;
  _num_terms3=in._num_terms3;
  
  if (_valid_data)
    init_fit_stat();
}

Fitness::Handle TwoThreePotentialOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
  GP::Tree& lTree = *inIndividual[0];
  GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
  unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
  ioContext.setGenotypeHandle(inIndividual[0]);
  ioContext.setGenotypeIndex(0);

  // Replace the root node with the LeftRight Primitive
  // Assert that the left tree only has x and the right tree only has r1, r2, a
  GP::Primitive::Handle old_root=lTree[0].mPrimitive;
  lTree[0].mPrimitive=new GP::LeftRight("TwoThree",lTree[0].mPrimitive->getNumberArguments());
  fix_dual_equation(ioContext,lTree);
  
  for (int i=0; i<data.num_configs; i++) {
    // Set left-right to evaluate first subtree
    lTree[0].mPrimitive->setValue(UInt(0));
    double energy=0, energyf=0, energyfx=0, energyfy=0, energyfz=0;
    int nR;
    if (_energy_flag) {
      nR=data.mR[i].size();
      for (int j=0; j<nR; j++) {
        setValue("X",data.mR[i][j],ioContext);
        Double lResult;
        inIndividual.run(lResult, ioContext);
        energy+=lResult;
      }
    }
    if (_force_flag>0) {
      nR=data.mRF[i].size();
      for (int j=0; j<nR; j++) {
        setValue("X",data.mRF[i][j],ioContext);
        Double lResult;
        inIndividual.run(lResult, ioContext);
        energyf+=lResult;
        if (j<data.ghost_mRF[i])
          energy+=lResult;
        setValue("X",data.dmRF_x[i][j],ioContext);
        inIndividual.run(lResult, ioContext);
        energyfx+=lResult;
        if (_force_flag==1) {
          setValue("X",data.dmRF_y[i][j],ioContext);
          inIndividual.run(lResult, ioContext);
          energyfy+=lResult;
          setValue("X",data.dmRF_z[i][j],ioContext);
          inIndividual.run(lResult, ioContext);
          energyfz+=lResult;
        }
      }
    }
    if (_energy_flag)
      eval_mY[i]=energy;
    if (_force_flag==1) {
      eval_dm[i*3]=(energyf-energyfx)/epsilon_p;
      eval_dm[i*3+1]=(energyf-energyfy)/epsilon_p;
      eval_dm[i*3+2]=(energyf-energyfz)/epsilon_p;
    } else if (_force_flag==2)
      eval_dm[i]=(energyf-energyfx)/epsilon_p;
    energy = energyf = energyfx = energyfy = energyfz = 0.0;
    // 3-body portion
    if (lTree[0].mPrimitive->getNumberArguments()>1) {
      // Set left-right to evaluate second subtree
      lTree[0].mPrimitive->setValue(UInt(1));
      nR=data.mR1[i].size();
      if (_energy_flag) {
        for (int j=0; j<nR; j++) {
          setValue("R1",data.mR1[i][j],ioContext);
          setValue("R2",data.mR2[i][j],ioContext);
          setValue("A",data.mA[i][j],ioContext);
          Double lResult;
          inIndividual.run(lResult, ioContext);
          energy+=lResult;
        }
      }
      if (_force_flag>0) {
        nR=data.mR1F[i].size();
        for (int j=0; j<nR; j++) {
          setValue("R1",data.mR1F[i][j],ioContext);
          setValue("R2",data.mR2F[i][j],ioContext);
          setValue("A",data.mAF[i][j],ioContext);
          Double lResult;
          inIndividual.run(lResult, ioContext);
          energyf+=lResult;
          if (j<data.ghost_3bF[i])
            energy+=lResult;
          setValue("R1",data.dmR1F_x[i][j],ioContext);
          setValue("R2",data.dmR2F_x[i][j],ioContext);
          setValue("A",data.dmAF_x[i][j],ioContext);
          inIndividual.run(lResult, ioContext);
          energyfx+=lResult;

          if (_force_flag==1) {
            setValue("R1",data.dmR1F_y[i][j],ioContext);
            setValue("R2",data.dmR2F_y[i][j],ioContext);
            setValue("A",data.dmAF_y[i][j],ioContext);
            inIndividual.run(lResult, ioContext);
            energyfy+=lResult;

            setValue("R1",data.dmR1F_z[i][j],ioContext);
            setValue("R2",data.dmR2F_z[i][j],ioContext);
            setValue("A",data.dmAF_z[i][j],ioContext);
            inIndividual.run(lResult, ioContext);
            energyfz+=lResult;
          }
        }
      }
    }
    if (_energy_flag)
      eval_mY3[i]=energy;
    if (_force_flag==1) {
      eval_dm3[i*3]=(energyf-energyfx)/epsilon_p;
      eval_dm3[i*3+1]=(energyf-energyfy)/epsilon_p;
      eval_dm3[i*3+2]=(energyf-energyfz)/epsilon_p;
    } else if (_force_flag==2)
      eval_dm3[i]=(energyf-energyfx)/epsilon_p;
  }
  
  unsigned totaln=inIndividual.getTotalNodes();

  // Replace the old root primitive
  lTree[0].mPrimitive=old_root;
  ioContext.setGenotypeHandle(lOldTreeHandle);
  ioContext.setGenotypeIndex(lOldTreeIndex);

  return fit_stat(totaln);
}
