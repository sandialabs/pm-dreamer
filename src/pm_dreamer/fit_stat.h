/***************************************************************************
                                  fit_stat.h
                             -------------------
                               W. Michael Brown

  Different Fitness Statistics

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Aug 21 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef FIT_STAT_H
#define FIT_STAT_H

#include <cassert>
#include <valarray>
using namespace std;

/// Base class for calculation of fitness statistics
/** All statistics have a minimum value of 0.0 and maximum of 1.0 **/
class FitStat {
 public:
  FitStat() : _parsimony_scale(false) {}
  /// Initialize with a vector of training samples and weights (x_i=x_i/size_xi)
  virtual void init(const valarray<double> &x,const valarray<int> &size_x) = 0;
  /// Initialize with a vector of training samples and weights (x_i=x_i/size_xi)
  /** Also stores the number of 3-body terms for 3-body potentials **/
  virtual void init(const valarray<double> &x,const valarray<int> &size_two,
                    const valarray<int> &size_three) 
    { init(x,size_two); }
  /// Calculate the fitness based on the vector y (corresponding to training x)
  virtual double fitness(const valarray<double> &y) = 0;
  /// Calculate the fitness based on the 2-body component and the 3-body comp
  virtual double fitness(const valarray<double> &two, 
                         const valarray<double> &three) {
    valarray<double> total(two+three);
    return fitness(total);
  }
  /// Turn on the flag to penalize large trees
  /** At start size the maximum fitness is 1.0, at end_size, the maximum
    * fitness is 0. Constant scaling in between **/
  virtual void set_parsimony_scale(const unsigned start_size, 
                                   const unsigned end_size);
  /// Calculate the fitness and optionally scale for parsimony
  /** Parsimony scaling only occurs if set_parsimony_scale has been called **/
  double parsimony_fitness(const valarray<double> &y, const unsigned num_nodes);
  /// Calculate the fitness and optionally scale for parsimony
  /** Parsimony scaling only occurs if set_parsimony_scale has been called **/
  double parsimony_fitness(const valarray<double> &two, 
                           const valarray<double> &three, 
                           const unsigned num_nodes);
    
  /// Calculate the fitness and optionally scale for parsimony
  /** Parsimony scaling only occurs if set_parsimony_scale has been called **/
  inline double parsimony_fitness(const valarray<double> &y, 
                                  const unsigned num_nodes,
                                  const double a, const double b) {
    return parsimony_fitness(a*y+b,num_nodes);
  }                                  
  /// Calculate the fitness and optionally scale for parsimony
  /** Parsimony scaling only occurs if set_parsimony_scale has been called **/
  inline double parsimony_fitness(const valarray<double> &two, 
                                  const valarray<double> &three, 
                                  const unsigned num_nodes,
                                  const double a, const double b,
                                  const double c, const double d) {
    return parsimony_fitness(a*two+b,c*three+d,num_nodes);
  }                                  
  
  /// Returns true if ols_correction is supported
  inline virtual bool ols_capable() 
    { return false; }
  /// Get the OLS correction (a*f+b) from last eval that optimizes the fitness
  inline virtual void ols_correct(double &a, double &b) const 
    { assert(0==1); }
  /// Get the OLS correction (a*f+b) from last eval that optimizes the fitness
  inline virtual void ols_correct(double &a, double &b, 
                                  double &c, double &d) const
    { assert(0==1); }
    
  /// Get the unnormalized RMSD
  inline virtual double rmsd(const valarray<double> &y) 
    { assert(0==1); return 0; }

  /// Get the range of input data values
  inline virtual double range() { return 0; }
 protected:
  bool _parsimony_scale;
  unsigned tree_size_start,tree_size_end;
  double penalty_inc;
  
  virtual double parsimony_by_nodes(double fitness, unsigned num_nodes);

  inline void swap2(double *,int,int);
  inline void swap4(double *,int,int);
  bool least_squares2(const valarray<double> &a1, const valarray<double> &a2,
                      const valarray<double> &b, double x[2]);
  bool least_squares4(const double *start_mat, const valarray<double> &a2,
                      const valarray<double> &a3, const valarray<double> &a4,
                      const valarray<double> &b, double x[4]);
};

/// Calculate the fitness using an Adaptive RMSD
class AdaptiveRMSD : public FitStat {
 public:
  AdaptiveRMSD() : FitStat(), _range(0), _min(0) {}
  void init(const valarray<double> &x,const valarray<int> &size_x);
  double fitness(const valarray<double> &y);
  
  /// Get the unnormalized RMSD
  double rmsd(const valarray<double> &y);
  /// Get the range of input data values
  inline double range() 
    { return _range; }
 private:
  double _range, _min;
  valarray<double> x;
  valarray<double> size_x;
};

/// Calculate the fitness using ordinary least-squares correction
class OLSAdaptiveRMSD : public FitStat {
 public:
  OLSAdaptiveRMSD() : FitStat() {}
  void init(const valarray<double> &x,const valarray<int> &size_x);
  void init(const valarray<double> &x,const valarray<int> &size_two,
            const valarray<int> &size_three) ;
  double fitness(const valarray<double> &y);
  double fitness(const valarray<double> &two, const valarray<double> &three);
  void ols_scale(valarray<double> &y);
  inline virtual bool ols_capable() 
    { return true; }
  void ols_correct(double &a, double &b) const;
  void ols_correct(double &a, double &b, double &c, double &d) const;

 private:
  valarray<double> x;
  valarray<double> size_x, a2;
 
  double x_mean;
  double sxx;
  valarray<double> x_cen;
  valarray<double> cov;
  double a_store, b_store;
  double range_x, min_x, range_y, min_y;
  bool solution;
  double ls_store[4];
  double hf_range, hg_range;
  double start_mat[20];
};

/// Calculate the fitness using Pearson Correlation Coefficient
class FitCorrCoef : public FitStat {
 public:
  FitCorrCoef() : FitStat() {}
  void init(const valarray<double> &x,const valarray<int> &size_x);
  void init(const valarray<double> &x,const valarray<int> &size_two,
            const valarray<int> &size_three) ;
  double fitness(const valarray<double> &y);
  double fitness(const valarray<double> &two, const valarray<double> &three);
  inline double pearson(const valarray<double> &y);
    
 private:
  valarray<double> x;
  valarray<double> size_x, a2;

  double x_std;
  double x_sum;
  double start_mat[20];
};
  
#endif
