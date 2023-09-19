/***************************************************************************
                           vtwo_three_potential.cpp
                             -------------------
                               W. Michael Brown

  Fitness evaluation for 3-body potentials with Force Calculation

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : July 28 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#include "vtwo_three_potential.h"

using namespace Beagle;

VTwoThreePotentialOp::VTwoThreePotentialOp(Error *errin, FitStat *fse, 
                                           FitStat *fsf, 
                                           int energy_flag, int force_flag) :
  BaseTwoThreeOp(errin,fse,fsf,energy_flag,force_flag)
{ }

VTwoThreePotentialOp::VTwoThreePotentialOp(Error *errin, FitStat *fse, 
                                           FitStat *fsf, 
                                           int energy_flag, int force_flag,
                                           const VTwoThreePotentialOp &in) :
  BaseTwoThreeOp(errin,fse,fsf,energy_flag,force_flag) {
  epsilon_p=in.epsilon_p;
  pm_copy(eval_dm,in.eval_dm);
  pm_copy(eval_mY,in.eval_mY);
  _valid_data=in._valid_data;
  data=in.data;
  pm_copy(eval_mY3,in.eval_mY3);
  pm_copy(eval_dm3,in.eval_dm3);
  
  vector_x=in.vector_x;
  vstart_e=in.vstart_e;
  vstart_f=in.vstart_f;
  vstart_fx=in.vstart_fx;
  vstart_fy=in.vstart_fy;
  vstart_fz=in.vstart_fz;
  
  vector_r1=in.vector_r1;
  vector_r2=in.vector_r2;
  vector_a=in.vector_a;
  vstart_e3=in.vstart_e3;
  vstart_f3=in.vstart_f3;
  vstart_fx3=in.vstart_fx3;
  vstart_fy3=in.vstart_fy3;
  vstart_fz3=in.vstart_fz3;
  
  _num_types=in._num_types;
  _num_terms=in._num_terms;
  _num_terms3=in._num_terms3;
  vector_t=in.vector_t;
  vector_t3=in.vector_t3;
  
  if (_valid_data)
    init_fit_stat();
}

Fitness::Handle VTwoThreePotentialOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
  GP::Tree& lTree = *inIndividual[0];
  GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();

  // Replace the root node with the LeftRight Primitive
  unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
  ioContext.setGenotypeHandle(inIndividual[0]);
  ioContext.setGenotypeIndex(0);

  GP::Primitive::Handle old_root=lTree[0].mPrimitive;
  lTree[0].mPrimitive=new GP::LeftRightT<VDouble>("TwoThree",lTree[0].mPrimitive->getNumberArguments());
  fix_dual_equation(ioContext,lTree);
  
  // Set left-right to evaluate first subtree
  lTree[0].mPrimitive->setValue(UInt(0));
  VDouble lResult;
  setValue("X",vector_x,ioContext);
  inIndividual.run(lResult, ioContext);
  
  VDouble lResult3;
  if (lTree[0].mPrimitive->getNumberArguments()>1 && vector_r1.size()>0) {
    // Set left-right to evaluate second subtree
    lTree[0].mPrimitive->setValue(UInt(1));
    setValue("R1",vector_r1,ioContext);
    setValue("R2",vector_r2,ioContext);
    setValue("A",vector_a,ioContext);
    inIndividual.run(lResult3, ioContext);
  }
  
  double energy, energyf, energyfx, energyfy, energyfz;
  for (int i=0; i<data.num_configs; i++) {
    // Without periodic boundary conditions, the energy is summed over
    // the distances in mR and mRF. With periodic boundaries, only
    // the first vnumel_ef distances are included in the energy calc.
    int vnumel_e=data.mR[i].size();
    if (_energy_flag==false)
      vnumel_e=0;
    int vnumel_ef=data.ghost_mRF[i];
    int vnumel_f=data.mRF[i].size();
    if (lResult.size()==1) {
      energy=vnumel_e*lResult[0]+vnumel_ef*lResult[0];
      energyfx=energyfy=energyfz=energyf=vnumel_f*lResult[0];
    } else {
      if (_energy_flag)
        energy=lResult.slice_sum(vstart_e[i],vnumel_e);
      if (_force_flag) {
        energyf=lResult.slice_sum(vstart_f[i],vnumel_ef);
        energy+=energyf;
        if (vnumel_ef!=vnumel_f)
          energyf+=lResult.slice_sum(vstart_f[i]+vnumel_ef,vnumel_f-vnumel_ef);
        energyfx=lResult.slice_sum(vstart_fx[i],vnumel_f);
        if (_force_flag==1) {
          energyfy=lResult.slice_sum(vstart_fy[i],vnumel_f);
          energyfz=lResult.slice_sum(vstart_fz[i],vnumel_f);
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
    
    // 3-body portion
    energy = energyf = energyfx = energyfy = energyfz = 0.0;
    if (lTree[0].mPrimitive->getNumberArguments()>1 && vector_r1.size()>0) {
      int vnumel_e3=data.mR1[i].size();
      if (_energy_flag==false)
        vnumel_e3=0;
      int vnumel_ef3=data.ghost_3bF[i];
      int vnumel_f3=data.mR1F[i].size();
      if (lResult3.size()==1) {
        energy=vnumel_e3*lResult3[0]+vnumel_ef3*lResult3[0];
        energyfx=energyfy=energyfz=energyf=vnumel_f3*lResult3[0];
      } else {
        if (_energy_flag)
          energy=lResult3.slice_sum(vstart_e3[i],vnumel_e3);
        if (_force_flag) {
          energyf=lResult3.slice_sum(vstart_f3[i],vnumel_ef3);
          energy+=energyf;
          if (vnumel_ef3!=vnumel_f3)
            energyf+=lResult3.slice_sum(vstart_f3[i]+vnumel_ef3,vnumel_f3-vnumel_ef3);
          energyfx=lResult3.slice_sum(vstart_fx3[i],vnumel_f3);
          if (_force_flag==1) {
            energyfy=lResult3.slice_sum(vstart_fy3[i],vnumel_f3);
            energyfz=lResult3.slice_sum(vstart_fz3[i],vnumel_f3);
          }
        }
      } 
    }
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

void VTwoThreePotentialOp::pack_data() {
  vector_t.clear();
  vstart_e.clear();
  vstart_f.clear();
  vstart_fx.clear();
  vstart_fy.clear();
  vstart_fz.clear();
  
  vector_t3.clear();
  vstart_e3.clear();
  vstart_f3.clear();
  vstart_fx3.clear();
  vstart_fy3.clear();
  vstart_fz3.clear();
  
  int pos=0, pos3=0;
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag) {
      vstart_e.push_back(pos);
      pos+=data.mR[i].size();
      
      vstart_e3.push_back(pos3);
      pos3+=data.mR1[i].size();
    }
    if (_force_flag) {
      vstart_f.push_back(pos);
      pos+=data.mRF[i].size();
      vstart_fx.push_back(pos);
      pos+=data.dmRF_x[i].size();
    
      vstart_f3.push_back(pos3);
      pos3+=data.mR1F[i].size();
      vstart_fx3.push_back(pos3);
      pos3+=data.dmR1F_x[i].size();
    }
    if (_force_flag==1) {
      vstart_fy.push_back(pos);
      pos+=data.dmRF_y[i].size();
      vstart_fz.push_back(pos);
      pos+=data.dmRF_z[i].size();
    
      vstart_fy3.push_back(pos3);
      pos3+=data.dmR1F_y[i].size();
      vstart_fz3.push_back(pos3);
      pos3+=data.dmR1F_z[i].size();
    }
  }

  std::valarray<double> &wv=vector_x.getWrappedValue().dvec;
  std::valarray<double> &wv_r1=vector_r1.getWrappedValue().dvec;
  std::valarray<double> &wv_r2=vector_r2.getWrappedValue().dvec;
  std::valarray<double> &wv_a=vector_a.getWrappedValue().dvec;
  wv.resize(pos);
  wv_r1.resize(pos3);
  wv_r2.resize(pos3);
  wv_a.resize(pos3);

  for (int i=0; i<data.num_configs; i++) {
    int nE=data.mR[i].size();
    int nF=data.mRF[i].size();
    int nE3=data.mR1[i].size();
    int nF3=data.mR1F[i].size();
    if (_energy_flag) {
      wv[std::slice(vstart_e[i],nE,1)]=vec_to_array(data.mR[i]);
      vector_t.insert(vector_t.end(),data.tR[i].begin(),data.tR[i].end());
    
      wv_r1[std::slice(vstart_e3[i],nE3,1)]=vec_to_array(data.mR1[i]);
      wv_r2[std::slice(vstart_e3[i],nE3,1)]=vec_to_array(data.mR2[i]);
      wv_a[std::slice(vstart_e3[i],nE3,1)]=vec_to_array(data.mA[i]);
      vector_t3.insert(vector_t3.end(),data.tR3[i].begin(),data.tR3[i].end());
    }
    if (_force_flag) {
      wv[std::slice(vstart_f[i],nF,1)]=vec_to_array(data.mRF[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
      wv[std::slice(vstart_fx[i],nF,1)]=vec_to_array(data.dmRF_x[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
    
      wv_r1[std::slice(vstart_f3[i],nF3,1)]=vec_to_array(data.mR1F[i]);
      wv_r2[std::slice(vstart_f3[i],nF3,1)]=vec_to_array(data.mR2F[i]);
      wv_a[std::slice(vstart_f3[i],nF3,1)]=vec_to_array(data.mAF[i]);
      vector_t3.insert(vector_t3.end(),data.tRF3[i].begin(),data.tRF3[i].end());
    
      wv_r1[std::slice(vstart_fx3[i],nF3,1)]=vec_to_array(data.dmR1F_x[i]);
      wv_r2[std::slice(vstart_fx3[i],nF3,1)]=vec_to_array(data.dmR2F_x[i]);
      wv_a[std::slice(vstart_fx3[i],nF3,1)]=vec_to_array(data.dmAF_x[i]);
      vector_t3.insert(vector_t3.end(),data.tRF3[i].begin(),data.tRF3[i].end());
    }
    if (_force_flag==1) {
      wv[std::slice(vstart_fy[i],nF,1)]=vec_to_array(data.dmRF_y[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
      wv[std::slice(vstart_fz[i],nF,1)]=vec_to_array(data.dmRF_z[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
    
      wv_r1[std::slice(vstart_fy3[i],nF3,1)]=vec_to_array(data.dmR1F_y[i]);
      wv_r2[std::slice(vstart_fy3[i],nF3,1)]=vec_to_array(data.dmR2F_y[i]);
      wv_a[std::slice(vstart_fy3[i],nF3,1)]=vec_to_array(data.dmAF_y[i]);
      vector_t3.insert(vector_t3.end(),data.tRF3[i].begin(),data.tRF3[i].end());
    
      wv_r1[std::slice(vstart_fz3[i],nF3,1)]=vec_to_array(data.dmR1F_z[i]);
      wv_r2[std::slice(vstart_fz3[i],nF3,1)]=vec_to_array(data.dmR2F_z[i]);
      wv_a[std::slice(vstart_fz3[i],nF3,1)]=vec_to_array(data.dmAF_z[i]);
      vector_t3.insert(vector_t3.end(),data.tRF3[i].begin(),data.tRF3[i].end());
    }
  }
}

void VTwoThreePotentialOp::pack_simplify(unsigned num_evals, 
                                         Double_matrix &two_body_input,
                                         primitive_vector &two_body_vars,
                                         Double_matrix &three_body_input,
                                         primitive_vector &three_body_vars,
                                         Beagle::GP::System& ioSystem) {
  two_body_input.clear();
  two_body_vars.clear();
  three_body_input.clear();
  three_body_vars.clear();

  GP::PrimitiveSuperSet& lSuperSet = ioSystem.getPrimitiveSuperSet();
  xPrim=lSuperSet[0]->getPrimitiveByName("X");
  aPrim=lSuperSet[0]->getPrimitiveByName("A");
  r1Prim=lSuperSet[0]->getPrimitiveByName("R1");
  r2Prim=lSuperSet[0]->getPrimitiveByName("R2");
  if (xPrim==NULL || aPrim==NULL || r1Prim==NULL || r2Prim==NULL)
    error->generate_error(0,"BaseTwoThreeOp",
     "Cannot use 3-body potential without X,A,R1, and R2 in the primitive set");
  two_body_vars.push_back(xPrim);
  three_body_vars.push_back(r1Prim);
  three_body_vars.push_back(r2Prim);
  three_body_vars.push_back(aPrim);

  if (num_terms3()>1) {
    two_body_input.push_back(vector_x);
    three_body_input.push_back(vector_r1);
    three_body_input.push_back(vector_r2);
    three_body_input.push_back(vector_a);
    return;
  }
  
  // Calculate the size of the simplification vector
  int total_size=0, total_3size=0;
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag) {
      total_size+=data.mR[i].size();
      total_3size+=data.mR1[i].size();
    }
    if (_force_flag) {
      total_size+=data.mRF[i].size();
      total_3size+=data.mR1F[i].size();
    }      
  }
  double_array vector_s(total_size);
  double_array vector_r1(total_3size),vector_r2(total_3size),
               vector_a(total_3size);
  int start_s=0, start_s3=0;
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag) {
      vector_s[std::slice(start_s,data.mR[i].size(),1)]=vec_to_array(data.mR[i]);
      start_s+=data.mR[i].size();
      vector_r1[std::slice(start_s3,data.mR1[i].size(),1)]=vec_to_array(data.mR1[i]);
      vector_r2[std::slice(start_s3,data.mR2[i].size(),1)]=vec_to_array(data.mR2[i]);
      vector_a[std::slice(start_s3,data.mA[i].size(),1)]=vec_to_array(data.mA[i]);
      start_s3+=data.mR1[i].size();
    }
    if (_force_flag) {
      vector_s[std::slice(start_s,data.mRF[i].size(),1)]=vec_to_array(data.mRF[i]);
      start_s+=data.mRF[i].size();
      vector_r1[std::slice(start_s3,data.mR1F[i].size(),1)]=vec_to_array(data.mR1F[i]);
      vector_r2[std::slice(start_s3,data.mR2F[i].size(),1)]=vec_to_array(data.mR2F[i]);
      vector_a[std::slice(start_s3,data.mAF[i].size(),1)]=vec_to_array(data.mAF[i]);
      start_s3+=data.mR1F[i].size();
    }
  }

  if (num_evals<=1 || num_evals>=total_size)
    two_body_input.push_back(vector_s);
  else
    two_body_input.push_back(vector_s[std::slice(0,num_evals,1)]);
  
  if (num_evals<=1 || num_evals>=total_3size) {
    three_body_input.push_back(vector_r1);
    three_body_input.push_back(vector_r2);
    three_body_input.push_back(vector_a);
  } else {
    three_body_input.push_back(vector_r1[std::slice(0,num_evals,1)]);
    three_body_input.push_back(vector_r2[std::slice(0,num_evals,1)]);
    three_body_input.push_back(vector_a[std::slice(0,num_evals,1)]);
  }
}
