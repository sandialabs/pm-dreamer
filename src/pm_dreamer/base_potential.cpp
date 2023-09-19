/***************************************************************************
                              base_potential.cpp
                             -------------------
                               W. Michael Brown

  Base class for evaluating potential functions

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

#ifdef GPMPI
#include "mpi.h"
#endif

#include "beagle/GP.hpp"
#include "base_potential.h"
#include "misc.h"

#include <cmath>

using namespace Beagle;

BasePotentialOp::BasePotentialOp(Error *errin, FitStat *fse, FitStat *fsf, 
                                 int energy_flag, int force_flag) :
  GP::EvaluationOp("EnergyOp"),
  error(errin),
  fitstat_energy(fse),
  fitstat_force(fsf),
  _energy_flag(energy_flag),
  _force_flag(force_flag),
  _valid_data(false),  
  my_rank(0),
  epsilon_p(1e-8),
  _num_types(1),
  _num_terms(1),
  _num_terms3(0)
{ 
  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  #endif
}

char* BasePotentialOp::check_tokens(char *tok) {
  tok = strtok(NULL," ,\t");
  if (tok==NULL)
    error->generate_error(0,"BasePotentialOp",
                  "Not enough tokens on data file input. See documentation.\n");
  return tok;
}

// Read the filetype string from an input data file and return
std::string BasePotentialOp::get_file_type(istream &in, Error &error) {
  std::string filetype;
  char line[200000];
  char *tok;

  while (!in.eof()) {
    in.getline(line,200000);
    if (in.eof() || !in)
      error.generate_error(0,"Base Potential",
                           "'filetype' specifier not found in input file.");

    // Split into tokens, # is comment, first is energy, rest are distance
    tok = strtok(line," ,\t");
    if (tok==NULL)
      continue;

    // Comment
    if (strcmp(tok,"#")==0)
      continue;

    // Cutoff
    if (strcmp(tok,"filetype")==0) {
      tok=check_tokens(tok);
      filetype=tok;
      break;
    }

    error.generate_error(0,"Base Potential",
                         "'filetype' specifier should be first in data file.");
  }
  return filetype;
}

void BasePotentialOp::read_xyz_configurations(istream &in,
                                              ParticleConfigs &pdata,
                                              const bool type_data) {
  pdata.cutoff_inf=true;
  pdata.cutoff=std::numeric_limits<double>::max();
  pdata.periodic=false;
  pdata.energies.clear();
  pdata.forces.clear();
  pdata.forcei.clear();
  pdata.mX.clear();
  pdata.ghost_atoms.clear();
  pdata.num_configs=0;

  bool cutoff_set, types_set=true;
  char line[200000];
  char *tok;
  double_array cPt(3);
  double_array force(3);

  pdata.num_types=1;
  if (type_data)
    types_set=false;

  while (!in.eof()) {
    in.getline(line,200000);
    if (in.eof() || !in)
      break;

    // Split into tokens, # is comment, first is energy, rest are distance
    tok = strtok(line," ,\t");
    if (tok==NULL)
      continue;
    
    // Comment
    if (strcmp(tok,"#")==0)
      continue;
    
    // Cutoff
    if (strcmp(tok,"cutoff")==0) {
      tok=check_tokens(tok);
      pdata.cutoff=atof(tok);
      pdata.cutoff_inf=false;
      cutoff_set=true;
      continue;
    }
    
    if (strcmp(tok,"periodic")==0) {
      pdata.periodic=true;
      tok=check_tokens(tok);
      pdata.periodic_x=atof(tok);
      tok=check_tokens(tok);
      pdata.periodic_y=atof(tok);
      tok=check_tokens(tok);
      pdata.periodic_z=atof(tok);
      continue;
    }

    if (strcmp(tok,"types")==0) {
      tok=check_tokens(tok);
      pdata.num_types=atoi(tok);
      _num_types=pdata.num_types;

      if (pdata.num_types<1)
        error->generate_error(0,"BasePotential",
                     "The number of particle types must be greater than 0.");
      
      if (type_data==false)
        error->generate_error(0,"BasePotential",
          "This input data file type doesn't allow mulitple particle types.");
      
      continue;
    } 

    if (cutoff_set==false)
      error->generate_error(0,"BasePotential",
                     "Error reading from data file. No cutoff has been read.");
    
    // Energy
    double energy = atof(tok);
    pdata.energies.push_back(energy);
    tok=check_tokens(tok);
    
    // Force index
    unsigned force_i=atoi(tok);
    if (_force_flag) {
      pdata.forcei.push_back(force_i-1);
      if (force_i==0) 
        error->generate_error(0,"BasePotential",
                           "Force index must be at least 1. See documentation.");
    } else
      pdata.forcei.push_back(-1);
    tok=check_tokens(tok);
    
    // Force
    force[0]=atof(tok);
    tok=check_tokens(tok);
    force[1]=atof(tok);
    tok=check_tokens(tok);
    force[2]=atof(tok);
    tok=check_tokens(tok);
    pdata.forces.push_back(force);
    
    // Positions
    pdata.mX.push_back(double_matrix());
    pdata.particle_types.push_back(int_vector());
    while (tok != NULL) {
      if (type_data) {
        int ptype=atoi(tok);
        if (ptype<1)
          error->generate_error(0,"BasePotential","Particle type must be >=1.");
        if (ptype>pdata.num_types)
          error->generate_error(0,"BasePotential",
            "Particle type greater than 'types'. Make sure types is specified");
        pdata.particle_types.back().push_back(ptype-1);
        tok=check_tokens(tok);
      } else
        pdata.particle_types.back().push_back(0);
      cPt[0]=atof(tok);
      tok=check_tokens(tok);
      cPt[1]=atof(tok);
      tok=check_tokens(tok);
      cPt[2]=atof(tok);
      tok = strtok(NULL," ,\t");
      pdata.mX.back().push_back(cPt);
    }

    if (_force_flag && force_i>pdata.mX.back().size()) 
      error->generate_error(0,"BasePotential",
                        "Force index is bigger than the number of positions.");
  }
  if (pdata.energies.empty())
    error->generate_error(0,"BasePotential",
                            "No configurations in input file.");
  pdata.num_configs=pdata.energies.size();
}

void BasePotentialOp::create_ghost_particles(ParticleConfigs &pdata) {
  pdata.ghost_atoms.clear();
  pdata.force_border.clear();

  for (int i=0; i<pdata.energies.size(); i++) {
    pdata.ghost_atoms.push_back(double_matrix());
    pdata.ghost_atom_types.push_back(int_vector());
    pdata.force_border.push_back(0);
  }
  if (pdata.periodic==false)
    return;

  if (pdata.cutoff>pdata.periodic_x || pdata.cutoff>pdata.periodic_y ||
      pdata.cutoff>pdata.periodic_z)
    error->generate_error(0,"BasePotential",
      "Currently, cutoff cannot be larger than the box size.");
  
  const double pbasis[26][3]={{-1,-1,-1},{-1,-1,0},{-1,-1,1},{-1,0,-1},{-1,0,0},
                              {-1,0,1},{-1,1,-1},{-1,1,0},{-1,1,1},{0,-1,-1},
                              {0,-1,0},{0,-1,1},{0,0,-1},
                              {1,-1,-1},{1,-1,0},{1,-1,1},{0,0,1},{0,1,-1},
                              {0,1,0},{0,1,1},{1,0,-1},{1,0,0},{1,0,1},{1,1,-1},
                              {1,1,0},{1,1,1}};
  
  double_array cPt(3);
  for (int i=0; i<pdata.energies.size(); i++) {
    for (int j=0; j<26; j++) {
      cPt[0]=pbasis[j][0]*pdata.periodic_x;
      cPt[1]=pbasis[j][1]*pdata.periodic_x;
      cPt[2]=pbasis[j][2]*pdata.periodic_x;
      for (int k=0; k<pdata.mX[i].size(); k++) {
        pdata.ghost_atoms[i].push_back(pdata.mX[i][k]+cPt);
        pdata.ghost_atom_types[i].push_back(pdata.particle_types[i][k]);
      }
      if (j==12)
        pdata.force_border[i]=pdata.ghost_atoms[i].size();
    }
  }
}

void BasePotentialOp::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem);
  assert(valid_data());
}
