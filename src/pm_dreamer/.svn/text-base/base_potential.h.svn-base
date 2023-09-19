/***************************************************************************
                              base_potential.h
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

#ifndef BASE_POTENTIAL_H
#define BASE_POTENTIAL_H

#include "beagle/GP.hpp"
#include <string>
#include <vector>
#include "error.h"
#include "VDouble.hpp"
#include "fit_stat.h"

typedef std::vector<double> double_vector;
typedef std::vector<Beagle::Double> Double_vector;
typedef std::vector<int> int_vector;
typedef std::vector<int_vector> int_set;
typedef std::vector<Beagle::VDouble> Double_matrix;
typedef std::vector<std::valarray<double> > double_matrix;
typedef std::vector<double_matrix> double_matrix_set;
typedef std::valarray<double> double_array;
typedef std::vector<Beagle::GP::Primitive::Handle> primitive_vector;

inline void pm_copy(double_array &target, const double_array &source) {
  target.resize(source.size());
  target=source;
}

inline void pm_copy(double_matrix &target, const double_matrix &source) {
  target.clear();
  for (int i=0; i<source.size(); i++) {
    target.push_back(double_array());
    pm_copy(target.back(),source[i]);
  }
}

inline void pm_copy(double_matrix_set &target,
                    const double_matrix_set &source) {
  target.clear();
  for (int i=0; i<source.size(); i++) {
    target.push_back(double_matrix());
    pm_copy(target.back(),source[i]);
  }
}

/// Data storing a set of configurations for training or test data
struct ParticleConfigs {
  bool cutoff_inf;
  double cutoff;
  double_vector energies;
  double_matrix forces;
  int_vector forcei;
  double_matrix_set mX;
  int_set particle_types;
  int num_configs;
  int num_types;
  
  bool periodic;
  double_matrix_set ghost_atoms;
  int_set ghost_atom_types;
  int_vector force_border;
  double periodic_x, periodic_y, periodic_z; // Box dimensions in x, y, and z
};

/// Base class for evaluating potential functions
class BasePotentialOp : public Beagle::GP::EvaluationOp {
 public:
  typedef Beagle::AllocatorT<BasePotentialOp,Beagle::GP::EvaluationOp::Alloc>
          Alloc;
  typedef Beagle::PointerT<BasePotentialOp,Beagle::GP::EvaluationOp::Handle>
          Handle;
  typedef Beagle::ContainerT<BasePotentialOp,Beagle::GP::EvaluationOp::Bag>
          Bag;

  /// Initialize with error object and objects for fitness statistics
  /** Valid pointers should be supplied for the fitness statistic for the
    * energy (fse) and the force (fsf) even if only one is to be used.
    * If energy_flag is nonzero the energy is included in the fitness 
    * calculation. If force_flag is 1, all three components of the force
    * are used in the fitness evaluation. If force_flag is 2, only the x-
    * component is used. If 0, the forces are not evaluated. **/
  BasePotentialOp(Error *error, FitStat *fse, FitStat *fsf, int energy_flag, 
                  int force_flag);
 
  virtual void postInit(Beagle::System& ioSystem);

  /// Calculate the fitness statistic and return
  virtual Beagle::Fitness::Handle fit_stat(const int tree_size)=0;
  
  /// Pack variables for function simplification
  virtual void pack_simplify(unsigned, Double_matrix &, primitive_vector &, 
                             Double_matrix &, primitive_vector &, 
                             Beagle::GP::System& ) = 0;
  /// If necessary, fix variables and primitives in equation
  virtual void fix_dual_equation(Beagle::GP::Context& ioContext,
                                 Beagle::GP::Tree&) = 0;
  
  /// Return the range of fitnesses
  double fit_range();

  /// True if everything is setup correctly for fitness evaluation
  inline bool valid_data() { return _valid_data; }
  /// Read in a data file and perform any initialization from the data
  virtual void read_data(const std::string &filename, const bool verbose) = 0;

  /// Read the filetype string from an input data file and return
  std::string get_file_type(istream &in, Error &error);
  /// Read in a set of configurations from a file and store in pdata
  /** \note Does not create ghost atoms in the data structure **/
  void read_xyz_configurations(istream &in, ParticleConfigs &pdata,
                               const bool type_data=false);
  /// Create ghost particles from the positions and cutoff in ParticleConfigs
  void create_ghost_particles(ParticleConfigs &pdata);
  
  /// Get the OLS correction (a*f+b) that optimizes the fitness
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext) = 0;
  inline void ols_correct(Beagle::GP::Individual& inIndividual,
                          Beagle::GP::Context& ioContext, double &a, double &b){
    Beagle::castHandleT<Beagle::FitnessSimple>(evaluate(inIndividual,ioContext));
    if (_energy_flag)
      fitstat_energy->ols_correct(a,b);
    else
      fitstat_force->ols_correct(a,b);
  }
  
  inline void ols_correct(Beagle::GP::Individual& inIndividual,
                          Beagle::GP::Context& ioContext, double &a, 
                          double &b, double &c, double &d){
    Beagle::castHandleT<Beagle::FitnessSimple>(evaluate(inIndividual,ioContext));
    if (_energy_flag)
      fitstat_energy->ols_correct(a,b,c,d);
    else
      fitstat_force->ols_correct(a,b,c,d);
  }

  /// Get the unnormalized RMSD for energies from the last evaluation
  virtual double energy_rmsd() = 0;
  /// Get the unnormalized RMSD for forces from the last evaluation
  virtual double force_rmsd() = 0;
  /// Get the range of input data
  inline double energy_range() { return fitstat_energy->range(); }
  /// Get the range of input data
  inline double force_range() { return fitstat_force->range(); }
  
  /// Return a unique index identifying the pair interaction by types
  /** This maps the types interacting in a given pair to a unique index.
    * The index for an i-j interaction is the same as that for a j-i
    * The indexes will be sequential starting at 0 **/
  inline int ui_pair(const int i, const int j)
    { if (i<j) return (j*j+j)/2+i; else return (i*i+i)/2+j; }
    
  /// Return a unique index identifying the 3-body interaction by types
  /** This maps the types interacting in a given 3-body to a unique index.
    * The index for an i-j-k interaction is the same as that for a i-k-j
    * The indexes will be sequential starting at 0 for N particle types **/
  inline int ui_three(const int i, const int j, const int k, const int N)
    { return i*(N*N+N)/2+ui_pair(j,k); }
  
  /// Return a pointer to the pair type vector
  inline const std::vector<int> * pair_types() const { return &vector_t; }

  /// Return a pointer to the 3-body type vector
  inline const std::vector<int> * pair_types3() const { return &vector_t3; }
  
  /// Return the number of atom types stored
  inline int num_types() { return _num_types; }
  
  /// Return the number of pair type terms
  inline int num_terms() { return _num_terms; }

  /// Return the number of 3-body type terms
  inline int num_terms3() { return _num_terms3; }
  
  /// Returns true if the energy fitness statistic can perform ols
  inline bool ols_capable() { return fitstat_energy->ols_capable(); }
  
protected:
  BasePotentialOp() {} 
  Error *error;
  FitStat *fitstat_energy, *fitstat_force;
  bool _valid_data;  
  int _energy_flag, _force_flag;
  int my_rank;
  double epsilon_p; // Epsilon for forward finite difference

  std::valarray<double> eval_mY;  // Vector evaluated energies (pair component);
  std::valarray<double> eval_dm;  // Vector evaluated forces (pair component);
  // Atom types (as indexed by ui_pair) for each pair interaction in order
  std::vector<int> vector_t, vector_t3;

  // Advance to next token and Generate an error if tok is null
  char * check_tokens(char *tok);
  
  int _num_types, _num_terms, _num_terms3;
};

#endif
