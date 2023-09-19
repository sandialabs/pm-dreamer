/***************************************************************************
                                base_pair.cpp
                             -------------------
                               W. Michael Brown

  Base class for evaluating pair-potential functions

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

#include "base_pair.h"
#include "misc.h"

using namespace Beagle;

void BasePairOp::calculate_variables(const ParticleConfigs &pdata,
                                     const bool verbose) {
  data.mR.clear();
  data.mRF.clear();
  data.dmRF_x.clear();
  data.dmRF_y.clear();
  data.dmRF_z.clear();
  data.ghost_mRF.clear();
  data.num_configs=pdata.energies.size();

  data.num_types=pdata.num_types;
  data.stypes.clear();
  for (int i=0; i<data.num_types; i++)
    for (int j=0; j<data.num_types; j++)
      if (j<=i)
        data.stypes.push_back(a::itoa(i+1)+"-"+a::itoa(j+1));
  data.tR.clear();
  data.tRF.clear();
  std::vector<int> counts(data.stypes.size(),0);

  for (int ii=0; ii<pdata.num_configs; ii++) {
    data.mR.push_back(Double_vector());
    data.mRF.push_back(Double_vector());
    data.dmRF_x.push_back(Double_vector());
    data.dmRF_y.push_back(Double_vector());
    data.dmRF_z.push_back(Double_vector());
    data.tR.push_back(int_vector());
    data.tRF.push_back(int_vector());
    double cutsq=pdata.cutoff*pdata.cutoff;
    if (pdata.cutoff<0)
      cutsq=pdata.cutoff;
    for (int i=0; i<pdata.mX[ii].size(); i++)
      for (int j=i+1; j<pdata.mX[ii].size(); j++)
        calculate_distance(cutsq,i,j,pdata.mX[ii][i],pdata.mX[ii][j],
                           pdata.forcei[ii],pdata.particle_types[ii][i],
                           pdata.particle_types[ii][j],counts);
    for (int i=0; i<pdata.mX[ii].size(); i++) {
      for (int j=0; j<pdata.force_border[ii]; j++)
        calculate_distance(cutsq,i,-2,pdata.mX[ii][i],
                           pdata.ghost_atoms[ii][j],pdata.forcei[ii],
                           pdata.particle_types[ii][i],
                           pdata.ghost_atom_types[ii][j],counts);
    }
    data.ghost_mRF.push_back(data.mRF.back().size());
    if (_force_flag)
      for (int j=pdata.force_border[ii]; j<pdata.ghost_atoms[ii].size(); j++)
        calculate_distance(cutsq,pdata.forcei[ii],-2,
                           pdata.mX[ii][pdata.forcei[ii]],
                           pdata.ghost_atoms[ii][j],pdata.forcei[ii],
                           pdata.particle_types[ii][pdata.forcei[ii]],
                           pdata.ghost_atom_types[ii][j],counts);
    if (data.mR.back().empty() && data.mRF.back().empty())
      error->generate_error(0,"base_pair",
          "Lines in data file must contain an energy and at least 1 distance.");
    if (my_rank==0 && verbose)
      cout << "Loaded configuration with " 
           << data.mR.back().size()+data.ghost_mRF.back()
           << " energy terms and "
           << data.mRF.back().size() << " force terms...\n";
  }
  if (my_rank == 0) {
    cout << "\n\n"
         << "Pair_Particle_Type Count\n"
         << "------------------------\n";
    for (int i=0; i<data.stypes.size(); i++)
      cout << data.stypes[i] << " " << counts[i] << " " << endl;
  }
  _num_terms=data.stypes.size();
}

void BasePairOp::calculate_distance(const double cutsq, const int i,
                                    const int j, const double_array &ipt,
                                    const double_array &jpt, const int force_i,
                                    const int type1, const int type2, 
                                    int_vector &counts){
  double xx=pow(jpt[0]-ipt[0],2.0);
  double yy=pow(jpt[1]-ipt[1],2.0);
  double zz=pow(jpt[2]-ipt[2],2.0);
  double rsq=xx+yy+zz;
  if (rsq<=cutsq || cutsq<0.0) {
    int ui=ui_pair(type1,type2);
    if (i==force_i) {
      data.mRF.back().push_back(sqrt(rsq));
      data.dmRF_x.back().push_back(sqrt(rsq-xx+pow(jpt[0]-(ipt[0]+epsilon_p),2.0)));
      data.dmRF_y.back().push_back(sqrt(rsq-yy+pow(jpt[1]-(ipt[1]+epsilon_p),2.0)));
      data.dmRF_z.back().push_back(sqrt(rsq-zz+pow(jpt[2]-(ipt[2]+epsilon_p),2.0)));
      data.tRF.back().push_back(ui);
    } else if (j==force_i) {
      data.mRF.back().push_back(sqrt(rsq));
      data.dmRF_x.back().push_back(sqrt(rsq-xx+pow(jpt[0]+epsilon_p-ipt[0],2.0)));
      data.dmRF_y.back().push_back(sqrt(rsq-yy+pow(jpt[1]+epsilon_p-ipt[1],2.0)));
      data.dmRF_z.back().push_back(sqrt(rsq-zz+pow(jpt[2]+epsilon_p-ipt[2],2.0)));
      data.tRF.back().push_back(ui);
    } else {
      data.mR.back().push_back(sqrt(rsq));
      data.tR.back().push_back(ui);
    }
    counts[ui]++;
  }
}

void BasePairOp::init_fit_stat() {
  std::valarray<int> mY_sizes(eval_mY.size());
  std::valarray<int> dm_sizes(eval_dm.size());
  for (int vi=0; vi<data.num_configs; vi++) {
    mY_sizes[vi]=data.mR[vi].size()+data.ghost_mRF[vi];
    if (_force_flag==1) {
      dm_sizes[vi*3]=data.mRF[vi].size();
      dm_sizes[vi*3+1]=data.mRF[vi].size();
      dm_sizes[vi*3+2]=data.mRF[vi].size();
    } else if (_force_flag==2) {
      dm_sizes[vi]=data.mRF[vi].size();
    }
  }
  if (_energy_flag)
    fitstat_energy->init(eval_mY,mY_sizes);
  if (_force_flag)
    fitstat_force->init(eval_dm,dm_sizes);
}

void BasePairOp::init_fit_stat(const ParticleConfigs &pdata) {
  eval_mY.resize(pdata.num_configs);
  if (_force_flag==1)
    eval_dm.resize(pdata.num_configs*3);
  else if (_force_flag==2)
    eval_dm.resize(pdata.num_configs);
  for (int vi=0; vi<pdata.num_configs; vi++) {
    eval_mY[vi]=pdata.energies[vi];
    if (_force_flag==1) {
      eval_dm[vi*3]=pdata.forces[vi][0];
      eval_dm[vi*3+1]=pdata.forces[vi][1];
      eval_dm[vi*3+2]=pdata.forces[vi][2];
    } else if (_force_flag==2) {
      eval_dm[vi]=pdata.forces[vi][0];
    }
  }
  init_fit_stat();
}

void BasePairOp::read_data(const std::string &filename, const bool verbose){
  ifstream in;
  a::fileopen(in,filename,*error);
  std::string file_type=get_file_type(in,*error);
  
  if (_force_flag && file_type=="pair")
    error->generate_error(0,"pair_potential","File type '"+file_type+
        "' doesn't support forces. Choose another template.");
  
  if (file_type=="efxyz")
    read_xyz_data(in,verbose);
  else if (file_type=="efixyz")
    read_xyz_data(in,verbose,true);
  else if (file_type=="pair")
    read_variable_data(in,verbose);
  else
    error->generate_error(0,"pair_potential","Invalid file type: "+file_type);
  a::fileclose(in,*error);
}

void BasePairOp::read_xyz_data(istream &in, const bool verbose, 
                               const bool types) {
  ParticleConfigs pdata;
  read_xyz_configurations(in,pdata,types);
  create_ghost_particles(pdata);
  calculate_variables(pdata,verbose);
  init_fit_stat(pdata);
  pack_data();
  _valid_data=true;
}

void BasePairOp::read_variable_data(istream &in, const bool verbose) {
  ParticleConfigs pdata;
  
  data.mR.clear();
  data.mRF.clear();
  data.dmRF_x.clear();
  data.dmRF_y.clear();
  data.dmRF_z.clear();
  data.ghost_mRF.clear();
  data.num_types=1;
  data.stypes.push_back("1-1");

  char line[200000];
  char *tok;
  while (!in.eof()) {
    in.getline(line,200000);
    if (in.eof() || !in)
      break;

    // Split into tokens, # is comment, first is energy, rest are distance
    tok = strtok(line," ,\t");
    if (tok==NULL)
      continue;
    
    // Energy
    if (strcmp(tok,"#")==0)
      continue;
    double energy = atof(tok);
    pdata.energies.push_back(energy);
    tok = strtok(NULL," ,\t");
    
    // Distances
    data.mR.push_back(Double_vector());
    data.mRF.push_back(Double_vector());
    while (tok != NULL) {
      if (strcmp(tok,"#")==0)
        break;
      data.mR.back().push_back(atof(tok));
      tok = strtok(NULL," ,\t");
    }
    
    data.ghost_mRF.push_back(data.mRF.back().size());
    
    if (data.mR.back().empty())
      error->generate_error(0,"pair_potential",
         "Lines in data file must contain an energy and at least 1 distance.");
    
    if (my_rank==0 && verbose)
      cout << "Loaded configuration with " << data.mR.back().size()
           << " distances...\n";
  }  

  if (data.mR.empty())
    error->generate_error(0,"pair_potential",
                            "No configurations in input file.");

  data.num_configs=pdata.energies.size();
  pdata.num_configs=pdata.energies.size();

  for (int i=0; i<data.mR.size(); i++) {
    data.tR.push_back(int_vector());
    data.tR.back().assign(data.mR[i].size(),0);
    if (i<data.mRF.size()) {
      data.tRF.push_back(int_vector());
      data.tRF.back().assign(data.mRF[i].size(),0);
    }
  }

  init_fit_stat(pdata);
  pack_data();
  _valid_data=true;
}

Beagle::Fitness::Handle BasePairOp::fit_stat(const int totaln) {
  double fitness;
  if (_energy_flag) {
    fitness=fitstat_energy->parsimony_fitness(eval_mY,totaln);
    if (_force_flag) {
      if (ols_capable()) {
        double a,b;
        fitstat_energy->ols_correct(a,b);
        fitness+=fitstat_force->parsimony_fitness(eval_dm,totaln,a,b);
      } else
        fitness+=fitstat_force->parsimony_fitness(eval_dm,totaln);
      fitness/=2.0;
    }
  } else
    fitness=fitstat_force->parsimony_fitness(eval_dm,totaln);
  
  return new FitnessSimple(fitness);
}
