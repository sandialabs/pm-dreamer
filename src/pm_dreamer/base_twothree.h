/***************************************************************************
                               base_twothree.h
                             -------------------
                               W. Michael Brown

  Base class for evaluating 3-body potential functions

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

#ifndef BASE_TWOTHREE_H
#define BASE_TWOTHREE_H

#include "beagle/GP.hpp"
#include "LeftRightT.hpp"
#include "base_potential.h"

struct TwoThreeVariableData {
  // 2-body Configuration distances not involved in Force
  std::vector<Double_vector> mR;

  // 3-body Configuration distances not involved in Force
  std::vector<Double_vector> mR1,mR2;
  // 3-body Configuration angles not involved in Force
  std::vector<Double_vector> mA;
  // 3-body parameters that influence force
  std::vector<Double_vector> mRF, mR1F, mR2F, mAF;
  // Configuration distances that influence force adjusted by epsilon
  std::vector<Double_vector> dmRF_x, dmRF_y, dmRF_z;
  std::vector<Double_vector> dmR1F_x, dmR1F_y, dmR1F_z;
  std::vector<Double_vector> dmR2F_x, dmR2F_y, dmR2F_z;
  std::vector<Double_vector> dmAF_x, dmAF_y, dmAF_z; 

  // Starting index for ghost atoms used only for force in 2-body
  std::vector<int> ghost_mRF;
  // Starting index for ghost atoms used only for force in 3-body
  std::vector<int> ghost_3bF;

  int num_configs;

  // Number of particle types
  int num_types;
  // String representing particle types interacting - pairs (1-1,1-2,etc)
  std::vector<std::string> stypes;
  // Index into stypes of the pair interactions in mR
  int_set tR;
  // Index into stypes of the pair interactions in mRF;
  int_set tRF;

  // String representing particle types interacting - (1-1-1,1-1-2,etc)
  std::vector<std::string> stypes3;
  // Index into stypes3 of the 3-body interactions in mA
  int_set tR3;
  // Index into stypes3 of the 3-body interactions in mAF;
  int_set tRF3;
};  

/// Base class for evaluating pair-potential functions
class BaseTwoThreeOp : public BasePotentialOp {
 public:
  typedef Beagle::AllocatorT<BaseTwoThreeOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<BaseTwoThreeOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<BaseTwoThreeOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  BaseTwoThreeOp(Error *error, FitStat *fse, FitStat *fsf, int energy_flag, 
                 int force_flag) : 
    BasePotentialOp(error,fse,fsf,energy_flag,force_flag) {}
  virtual void postInit(Beagle::System& ioSystem);

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
  inline double energy_rmsd() { return fitstat_energy->rmsd(eval_mY+eval_mY3); }
  /// Get the unnormalized RMSD for forces from the last evaluation
  inline double force_rmsd() { return fitstat_force->rmsd(eval_dm+eval_dm3); }
protected:
  BaseTwoThreeOp() {} 
  TwoThreeVariableData data;
  Beagle::GP::Primitive::Handle xPrim, aPrim, r1Prim, r2Prim, edPrim;
  std::valarray<double> eval_mY3; // Vector evaluated energies (3-body comp);
  std::valarray<double> eval_dm3; // Vector evaluated forces (3-body comp);
    
  void calculate_distance(const double, const int,const int, 
                          const double_array&, const double_array&, const int,
                          const int, const int, int_vector &);
  void calculate_3body(const double, const int,const int,const int, 
                       const double_array&, const double_array&, 
                       const double_array&, const int, const int, const int,
                       const int, int_vector &);
  void fix_dual_equation(Beagle::GP::Context& ioContext,Beagle::GP::Tree&);
    
  void clear_data();
  void add_empty_config();
  void init_fit_stat();
};

#endif
