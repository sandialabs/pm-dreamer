/***************************************************************************
                             vpair_potential.cpp
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

#include "vpair_potential.h"

using namespace Beagle;

VPairPotentialOp::VPairPotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                     int energy_flag, int force_flag) :
  BasePairOp(errin,fse,fsf,energy_flag,force_flag)
{ }

VPairPotentialOp::VPairPotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                   int energy_flag, int force_flag,
                                   const VPairPotentialOp &in) :
  BasePairOp(errin,fse,fsf,energy_flag,force_flag) {
  epsilon_p=in.epsilon_p;
  pm_copy(eval_dm,in.eval_dm);
  pm_copy(eval_mY,in.eval_mY);
  _valid_data=in._valid_data;
  data=in.data;
  vector_x=in.vector_x;
  vstart_e=in.vstart_e;
  vstart_f=in.vstart_f;
  vstart_fx=in.vstart_fx;
  vstart_fy=in.vstart_fy;
  vstart_fz=in.vstart_fz;
  
  _num_types=in._num_types;
  _num_terms=in._num_terms;
  _num_terms3=in._num_terms3;
  vector_t=in.vector_t;
  vector_t3=in.vector_t3;
  
  if (_valid_data)
    init_fit_stat();
}

Fitness::Handle VPairPotentialOp::evaluate(GP::Individual& inIndividual, 
                                           GP::Context& ioContext)
{
  VDouble lResult;
  setValue("X", vector_x, ioContext);
  inIndividual.run(lResult, ioContext);
  unsigned totaln=inIndividual.getTotalNodes();

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
  }

  return fit_stat(totaln);
}

void VPairPotentialOp::pack_data() {
  vector_t.clear();
  vstart_e.clear();
  vstart_f.clear();
  vstart_fx.clear();
  vstart_fy.clear();
  vstart_fz.clear();

  int pos=0;
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag) {
      vstart_e.push_back(pos);
      pos+=data.mR[i].size();
    }
    if (_force_flag) {
      vstart_f.push_back(pos);
      pos+=data.mRF[i].size();
      vstart_fx.push_back(pos);
      pos+=data.dmRF_x[i].size();
    }
    if (_force_flag==1) {
      vstart_fy.push_back(pos);
      pos+=data.dmRF_y[i].size();
      vstart_fz.push_back(pos);
      pos+=data.dmRF_z[i].size();
    }
  }

  std::valarray<double> &wv=vector_x.getWrappedValue().dvec;
  wv.resize(pos);
  
  for (int i=0; i<data.num_configs; i++) {
    int nE=data.mR[i].size();
    int nF=data.mRF[i].size();
    if (_energy_flag) {
      wv[std::slice(vstart_e[i],nE,1)]=vec_to_array(data.mR[i]);
      vector_t.insert(vector_t.end(),data.tR[i].begin(),data.tR[i].end());
    }
    if (_force_flag) {
      wv[std::slice(vstart_f[i],nF,1)]=vec_to_array(data.mRF[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
      wv[std::slice(vstart_fx[i],nF,1)]=vec_to_array(data.dmRF_x[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
    }
    if (_force_flag==1) {
      wv[std::slice(vstart_fy[i],nF,1)]=vec_to_array(data.dmRF_y[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
      wv[std::slice(vstart_fz[i],nF,1)]=vec_to_array(data.dmRF_z[i]);
      vector_t.insert(vector_t.end(),data.tRF[i].begin(),data.tRF[i].end());
    }
  }
}

void VPairPotentialOp::pack_simplify(unsigned num_evals, 
                                     Double_matrix &two_body_input,
                                     primitive_vector &two_body_vars,
                                     Double_matrix &three_body_input,
                                     primitive_vector &three_body_vars,
                                     Beagle::GP::System& ioSystem) {
  two_body_input.clear();
  two_body_vars.clear();
  three_body_input.clear();
  three_body_vars.clear();

  // Find the "X" primitive in the superset
  GP::PrimitiveSuperSet& lSuperSet = ioSystem.getPrimitiveSuperSet();
  GP::Primitive::Handle xPrim=lSuperSet[0]->getPrimitiveByName("X");
  if (xPrim==NULL)
    error->generate_error(0,"VPairPotentialOp",
      "Cannot use pair potential without X in the primitive set");
  two_body_vars.push_back(xPrim);

  if (num_terms()>1) {
    two_body_input.push_back(vector_x);
    return;
  }
  
  // Calculate the size of the simplification vector
  int total_size=0;
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag)
      total_size+=data.mR[i].size();
    if (_force_flag)
      total_size+=data.mRF[i].size();
  }
  double_array vector_s(total_size);
  int start_s=0;
  
  for (int i=0; i<data.num_configs; i++) {
    if (_energy_flag) {
      vector_s[std::slice(start_s,data.mR[i].size(),1)]=vec_to_array(data.mR[i]);
      start_s+=data.mR[i].size();
    }
    if (_force_flag) {
      vector_s[std::slice(start_s,data.mRF[i].size(),1)]=vec_to_array(data.mRF[i]);
      start_s+=data.mRF[i].size();
    }
  }
  
  if (num_evals<=1 || num_evals>=total_size)
    two_body_input.push_back(vector_s);
  else
    two_body_input.push_back(vector_s[std::slice(0,num_evals,1)]);
}
