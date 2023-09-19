/***************************************************************************
                                 fit_stat.cpp
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

#include "fit_stat.h"

void FitStat::set_parsimony_scale(unsigned start, unsigned end) {
  assert(end>start);
  _parsimony_scale=true;
  tree_size_start=start;
  tree_size_end=end;
  penalty_inc=1.0/static_cast<double>(end-start);
}

double FitStat::parsimony_by_nodes(double fit, unsigned num_nodes) {
  if (_parsimony_scale && num_nodes>tree_size_start) {
    fit-=(penalty_inc*(num_nodes-tree_size_start));
    if (fit<0.0)
      fit=0.0;
  }
  return fit;
}

double FitStat::parsimony_fitness(const valarray<double> &y, 
                                  const unsigned num_nodes) {
  double fit=fitness(y);
  return parsimony_by_nodes(fit,num_nodes);
}                                 

double FitStat::parsimony_fitness(const valarray<double> &two, 
                                     const valarray<double> &three,
                                     const unsigned num_nodes) {
  double fit=fitness(two,three);
  return parsimony_by_nodes(fit,num_nodes);
}                                 

void FitStat::swap2(double *m, int i, int j) {
  double tempv[3];
  tempv[0]=m[i];
  tempv[1]=m[i+2];
  tempv[2]=m[i+4];
  m[i]=m[j];
  m[i+2]=m[j+2];
  m[i+4]=m[j+4];
  m[j]=tempv[0];
  m[j+2]=tempv[1];
  m[j+4]=tempv[2];
}

void FitStat::swap4(double *m, int i, int j) {
  double tempv[5];
  tempv[0]=m[i];
  tempv[1]=m[i+4];
  tempv[2]=m[i+8];
  tempv[3]=m[i+12];
  tempv[4]=m[i+16];
  m[i]=m[j];
  m[i+4]=m[j+4];
  m[i+8]=m[j+8];
  m[i+12]=m[j+12];
  m[i+16]=m[j+16];
  m[j]=tempv[0];
  m[j+4]=tempv[1];
  m[j+8]=tempv[2];
  m[j+12]=tempv[3];
  m[j+16]=tempv[4];
}

bool FitStat::least_squares2(const valarray<double> &a1, 
                             const valarray<double> &a2,
                             const valarray<double> &b, double x[2]) {
  // Compute m=[aTa aTb];
  double m[6]={0.0};
  int rows=a1.size();
  for (int i=0; i<rows; i++) {
    m[0]+=a1[i]*a1[i];
    double t=a1[i]*a2[i];
    m[1]+=t;
    m[2]+=t;
    m[3]+=a2[i]*a2[i];
    m[4]+=a1[i]*b[i]; // aTb(0)
    m[5]+=a2[i]*b[i]; // aTb(1)
  }
  
  // Solve ax=b
  for (int i = 0; i < 1; i++) {
    int p = i;
    for (int j = i+1; j < 2; j++)
      if (fabs(m[i*2+j]) > fabs(m[i*2+i])) swap2(m,i,j);

    while (m[i*2+p] == 0.0 && p < 2) p++;

    if (p == 2) return false;
    if (p != i) swap2(m,i,p);

    for (int j = i+1; j < 2; j++) {
      double t = m[i*2+j]/m[i*2+i];
      for (int k=i+1; k<3; k++) m[k*2+j]-=t*m[k*2+i];
    }
  }

  if (m[3] == 0.0) return false;
  
  // back substitution
  x[1] = m[5]/m[3];
  x[0] = (m[4]-m[2]*x[1]) / m[0];

  return true;
}

bool FitStat::least_squares4(const double *start_mat, 
                             const valarray<double> &a2,
                             const valarray<double> &a3, 
                             const valarray<double> &a4,
                             const valarray<double> &b, double x[4]) {
  // Compute m=[aTa aTb];
  double m[20];
  memcpy(m,start_mat,20*sizeof(double));
  int rows=a2.size();
  double t;
  for (int i=0; i<rows; i++) {
    m[10]+=a3[i]*a3[i];
    m[15]+=a4[i]*a4[i];
    m[2]+=a3[i];
    m[8]+=a3[i];
    m[3]+=a4[i];
    m[12]+=a4[i];
    t=a2[i]*a3[i];
    m[6]+=t;
    m[9]+=t;
    t=a2[i]*a4[i];
    m[7]+=t;
    m[13]+=t;
    t=a3[i]*a4[i];
    m[11]+=t;
    m[14]+=t;
    
    // aTb
    m[16]+=b[i];
    m[17]+=a2[i]*b[i];
    m[18]+=a3[i]*b[i];
    m[19]+=a4[i]*b[i];
  }
  
  // Solve ax=b
  for (int i = 0; i < 3; i++) {
    int p = i;
    for (int j = i+1; j < 4; j++)
      if (fabs(m[i*4+j]) > fabs(m[i*4+i])) swap4(m,i,j);

    while (m[i*4+p] == 0.0 && p < 4) p++;

    if (p == 4) return false;
    if (p != i) swap4(m,i,p);

    for (int j = i+1; j < 4; j++) {
      double t = m[i*4+j]/m[i*4+i];
      for (int k=i+1; k<5; k++) m[k*4+j]-=t*m[k*4+i];
    }
  }

  if (m[15] == 0.0) return false;
  
  // back substitution
  x[3] = m[19]/m[15];
  for (int i = 2; i >= 0; i--) {
    double sumax = 0.0;
    for (int j = i+1; j < 4; j++) sumax += m[j*4+i]*x[j];
    x[i] = (m[16+i]-sumax) / m[i*4+i];
  }

  return true;
}

void AdaptiveRMSD::init(const valarray<double> &xin,const valarray<int> &size_x_in) {
  assert(xin.size()>0 && size_x_in.size()==xin.size());
  x.resize(xin.size());
  size_x.resize(size_x_in.size());

  for (int i=0; i<size_x_in.size(); i++)
    size_x[i]=static_cast<double>(size_x_in[i]);
  x=xin/size_x;
  double nrange=x.max()-x.min();
  size_x*=nrange;
  x=xin/size_x;

  _range=xin.max()-xin.min();
}

double AdaptiveRMSD::fitness(const valarray<double> &y_in) {
  valarray<double> y(y_in/size_x);
  y-=x;
  y*=y;
  double rmsd=sqrt(y.sum()/static_cast<double>(y.size()));
  return 1.0/(rmsd+1.0);
}

// Get the unnormalized RMSD
double AdaptiveRMSD::rmsd(const valarray<double> &y) {
  valarray<double> fy(y-x*size_x);
  fy*=fy;
  return sqrt(fy.sum()/static_cast<double>(fy.size()));
}

void OLSAdaptiveRMSD::init(const valarray<double> &xin,const valarray<int> &size_x_in) {
  assert(xin.size()>0 && size_x_in.size()==xin.size());
  x.resize(xin.size());
  size_x.resize(size_x_in.size());

  for (int i=0; i<size_x_in.size(); i++)
    size_x[i]=static_cast<double>(size_x_in[i]);

  x=xin/size_x;
  min_x=x.min();
  range_x=x.max()-min_x;
  x=(x-min_x)/range_x;
  
  x_mean=x.sum()/static_cast<double>(x.size());
  x_cen.resize(x.size());
  x_cen=x-x_mean;
  sxx=(x_cen*x_cen).sum();
  cov.resize(x.size());
}

void OLSAdaptiveRMSD::init(const valarray<double> &xin,
                           const valarray<int> &size_x_in,
                           const valarray<int> &size_three) {
  assert(xin.size()>0 && size_x_in.size()==xin.size());
  x.resize(xin.size());
  size_x.resize(size_x_in.size());

  for (int i=0; i<size_x_in.size(); i++)
    size_x[i]=static_cast<double>(size_x_in[i]);

  x=xin/size_x;
  min_x=0.0;
  range_x=x.max()-x.min();
  x/=range_x;
  
  x_mean=x.sum()/static_cast<double>(x.size());
  x_cen.resize(x.size());
  x_cen=x-x_mean;
  sxx=(x_cen*x_cen).sum();
  cov.resize(x.size());

  a2.resize(xin.size());
  for (int i=0; i<a2.size(); i++)
    a2[i]=static_cast<double>(size_three[i])/static_cast<double>(size_x_in[i]);
  for (int i=0; i<20; i++)
    start_mat[i]=0.0;
  for (int i=0; i<a2.size(); i++) {
    start_mat[0]+=1.0;
    start_mat[5]+=a2[i]*a2[i];
    start_mat[1]+=a2[i];
    start_mat[4]+=a2[i];
  }
}

double OLSAdaptiveRMSD::fitness(const valarray<double> &y_in) {
  valarray<double> y(y_in/size_x);
  min_y=y.min();
  range_y=y.max()-min_y;
  if (range_y<1e-8 || range_y>1e16) {
    a_store=0.0;
    return 0.0;
  }
  y=(y-min_y)/range_y;

  ols_scale(y);

  y-=x;
  y*=y;
  double rmsd=sqrt(y.sum()/static_cast<double>(y.size()))*range_x;
  return 1.0/(rmsd+1.0);
}

void OLSAdaptiveRMSD::ols_scale(valarray<double> &y) {
  double y_mean=y.sum()/static_cast<double>(y.size());
  cov=(y-y_mean)*x_cen;
  a_store=cov.sum()/sxx;
  b_store=y_mean-a_store*x_mean;
  y=(y-b_store)/a_store;
}

void OLSAdaptiveRMSD::ols_correct(double &a, double &b) const {
  if (a_store==0 || range_y==0) {
    b=0.0;
    a=1.0;
  } else {
    b=min_x-min_y*range_x/(a_store*range_y)-b_store*range_x/a_store;
    a=range_x/(a_store*range_y);
  }
}

double OLSAdaptiveRMSD::fitness(const valarray<double> &two, 
                                const valarray<double> &three) {
  valarray<double> a3(two/size_x);
  valarray<double> a4(three/size_x);
  hf_range=a3.max()-a3.min();
  if (hf_range<1e-8 || hf_range>1e16) {
    hf_range=0.0;
    hg_range=0.0;
    return 0.0;
  }
  a3/=hf_range;
  hg_range=a4.max()-a4.min();
  if (hg_range<1e-8 || hg_range>1e16) {
    hf_range=0.0;
    hg_range=0.0;
    return 0.0;
  }
  a4/=hg_range;
  
  solution=least_squares4(start_mat,a2,a3,a4,x,ls_store);
  
  if (!solution) {
    valarray<double> total(two+three);
    return fitness(total); 
  }
  
  a3=ls_store[2]*a3+ls_store[0]+ls_store[3]*a4+ls_store[1]*a2;
  
  a3-=x;
  a3*=a3;
  double rmsd=sqrt(a3.sum()/static_cast<double>(a3.size()))*range_x;
  return 1.0/(rmsd+1.0);
}

void OLSAdaptiveRMSD::ols_correct(double &a, double &b, 
                                  double &c, double &d) const {
  if (!solution) {
    ols_correct(a,b);
    c=a;
    d=0.0;
    return;
  }
  
  if (hf_range==0) {
    a=1.0;
    b=0.0;
  } else {
    b=ls_store[0]*range_x;
    a=ls_store[2]*range_x/hf_range;
  }
  
  if (hg_range==0) {
    c=1.0;
    d=0.0;
  } else {
    d=ls_store[1]*range_x;
    c=ls_store[3]*range_x/hg_range;
  }
}

void FitCorrCoef::init(const valarray<double> &xin,const valarray<int> &size_x_in) {
  assert(xin.size()>0 && size_x_in.size()==xin.size());
  x.resize(xin.size());
  size_x.resize(size_x_in.size());

  for (int i=0; i<size_x_in.size(); i++)
    size_x[i]=static_cast<double>(size_x_in[i]);
  x=xin/size_x;
  double range=x.max()-x.min();
  x=(x-x.min())/range;
  x_sum=x.sum();
  x_std=(x*x).sum()-x_sum*x_sum/static_cast<double>(x.size());
}

void FitCorrCoef::init(const valarray<double> &x,const valarray<int> &size_two,
                       const valarray<int> &size_three) {
  init(x,size_two);
  a2.resize(x.size());
  for (int i=0; i<a2.size(); i++)
    a2[i]=static_cast<double>(size_three[i])/static_cast<double>(size_two[i]);
  for (int i=0; i<20; i++)
    start_mat[i]=0.0;
  for (int i=0; i<a2.size(); i++) {
    start_mat[0]+=1.0;
    start_mat[5]+=a2[i]*a2[i];
    start_mat[1]+=a2[i];
    start_mat[4]+=a2[i];
  }
}

double FitCorrCoef::pearson(const valarray<double> &y) {
  double sy=y.sum();
  double sy2=(y*y).sum();
  double ysize=static_cast<double>(y.size());
  double y_std=(sy2-sy*sy/ysize);
  
  double sxy=(x*y).sum();
  double fit=sxy-(x_sum*sy)/ysize;
  fit/=sqrt(x_std*y_std);

  fit=1.0-fabs(fit);
  return 1.0/(fit*100.0+1.0);
}  

double FitCorrCoef::fitness(const valarray<double> &y_in) {
  valarray<double> y(y_in/size_x);
  double range=y.max()-y.min();
  if (range<1e-8 || range>1e16)
    return 0.0;
  y=(y-y.min())/range;
  return pearson(y);
}

double FitCorrCoef::fitness(const valarray<double> &two, 
                            const valarray<double> &three) {
  valarray<double> a3(two/size_x);
  valarray<double> a4(three/size_x);
  double range=a3.max()-a3.min();
  if (range<1e-8 || range>1e16)
    return 0.0;
  a3=(a3-a3.min())/range;
  range=a4.max()-a4.min();
  if (range<1e-8 || range>1e16)
    return 0.0;
  a4=(a4-a4.min())/range;
  
  double ans[4];
  bool solution=least_squares4(start_mat,a2,a3,a4,x,ans);
  
  if (!solution) {
    valarray<double> total(two+three);
    return fitness(total); 
  }
  
  return pearson(ans[2]*a3+ans[0]+ans[3]*a4+ans[1]*a2);
}
