/***************************************************************************
                                 base_pair.h
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

#ifndef BASE_PAIR_H
#define BASE_PAIR_H

#include "base_potential.h"

struct PairVariableData {
  // Configuration distances not involved in Force
  std::vector<Double_vector> mR;
  // Configuration distances that influence force
  std::vector<Double_vector> mRF;
  // Starting index of ghost atoms used only for force calc in mRF
  std::vector<int> ghost_mRF;

  // Configuration distances that influence force adjusted by epsilon
  std::vector<Double_vector> dmRF_x;
  std::vector<Double_vector> dmRF_y;
  std::vector<Double_vector> dmRF_z;
  int num_configs;
  
  // Number of particle types
  int num_types;
  // String representing particle types interacting (1-1,1-2,etc)
  std::vector<std::string> stypes;
  // Index into stypes of the pair interactions in mR
  int_set tR;
  // Index into stypes of the pair interactions in mRF;
  int_set tRF;
};

/// Base class for evaluating pair-potential functions
class BasePairOp : public BasePotentialOp {
 public:
  typedef Beagle::AllocatorT<BasePairOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<BasePairOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<BasePairOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  BasePairOp(Error *error, FitStat *fse, FitStat *fsf, int energy_flag, 
             int force_flag) : 
    BasePotentialOp(error,fse,fsf,energy_flag,force_flag) {}
 
  /// Read in a data file and perform any initialization from the data
  void read_data(const std::string &filename, const bool verbose);

  /// Read in a data file from xyz formats and initialize
  void read_xyz_data(istream &in, const bool verbose, 
                     const bool types=false);
  /// Read in a data file storing variables instead of configuration data
  void read_variable_data(istream &in, const bool verbose);
  /// Calculate variables from data
  void calculate_variables(const ParticleConfigs &pdata, const bool verbose);
  /// Initialize the fitness statistic
  void init_fit_stat(const ParticleConfigs &pdata);
  /// Post-process any data for vectorization
  virtual void pack_data() = 0;
      
  /// Calculate the fitness statistic and return
  Beagle::Fitness::Handle fit_stat(const int tree_size);

  /// Get the unnormalized RMSD for energies from the last evaluation
  inline double energy_rmsd() { return fitstat_energy->rmsd(eval_mY); }
  /// Get the unnormalized RMSD for forces from the last evaluation
  inline double force_rmsd() { return fitstat_force->rmsd(eval_dm); }
  void fix_dual_equation(Beagle::GP::Context& ioContext,Beagle::GP::Tree&) {}
protected:
  BasePairOp() {} 
  PairVariableData data;
    
  void calculate_distance(const double, const int,const int, 
                          const double_array&, const double_array&, const int,
                          const int, const int, int_vector &);
  void init_fit_stat();
};

#endif
