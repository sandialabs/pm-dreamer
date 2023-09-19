/***************************************************************************
                              base_twothree.cpp
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

#include "base_twothree.h"
#include "misc.h"

using namespace Beagle;

void BaseTwoThreeOp::calculate_variables(const ParticleConfigs &pdata,
                                         const bool verbose) {
  clear_data();
  double_array ipt(3),jpt(3),kpt(3);
  
  // --- 2-body type information
  data.num_types=pdata.num_types;
  data.stypes.clear();
  for (int i=0; i<data.num_types; i++)
    for (int j=0; j<data.num_types; j++)
      if (j<=i)
        data.stypes.push_back(a::itoa(i+1)+"-"+a::itoa(j+1));
  data.tR.clear();
  data.tRF.clear();
  std::vector<int> counts(data.stypes.size(),0);
  
  // --- 3-body type information
  data.stypes3.clear();
  for (int i=0; i<data.num_types; i++)
    for (int j=0; j<data.num_types; j++)
      for (int k=0; k<data.num_types; k++)
        if (k<=j)
          data.stypes3.push_back(a::itoa(i+1)+"-"+a::itoa(j+1)+"-"+
                                 a::itoa(k+1));
  data.tR3.clear();
  data.tRF3.clear();
  std::vector<int> counts3(data.stypes3.size(),0);
  
  for (int ii=0; ii<pdata.num_configs; ii++) {
    add_empty_config();
    double cutsq=pdata.cutoff*pdata.cutoff;
    if (pdata.cutoff<0)
      cutsq=pdata.cutoff;

    int num_particles=pdata.mX[ii].size()+pdata.force_border[ii];
    int num_real=pdata.mX[ii].size();

    // Calculation for ghost atoms needed for energy
    int itype, jtype, ktype;
    for (int i=0; i<num_particles; i++) {
      if (i<num_real) {
        ipt=pdata.mX[ii][i];
        itype=pdata.particle_types[ii][i];
        // Calculate 2-body variables
        for (int j=i+1; j<pdata.mX[ii].size(); j++)
          calculate_distance(cutsq,i,j,ipt,pdata.mX[ii][j],pdata.forcei[ii],
                             itype,pdata.particle_types[ii][j],counts);
        for (int j=0; j<pdata.force_border[ii]; j++)
          calculate_distance(cutsq,i,-2,ipt,pdata.ghost_atoms[ii][j],
                             pdata.forcei[ii],itype,
                             pdata.ghost_atom_types[ii][j],counts);
      } else {
        ipt=pdata.ghost_atoms[ii][i-num_real];
        itype=pdata.ghost_atom_types[ii][i-num_real];
      }
      
      // Calculate 3-body variables
      for (int j=0; j<num_particles-1; j++) {
        if (i==j)
          continue;
        if (j<num_real) {
          jpt=pdata.mX[ii][j];
          jtype=pdata.particle_types[ii][j];
        } else {
          jpt=pdata.ghost_atoms[ii][j-num_real];
          jtype=pdata.ghost_atom_types[ii][j-num_real];
        }
        double_array rsq=pow(jpt-ipt,2.0);
        if (rsq.sum()<=cutsq || cutsq<0.0)
          for (int k=j+1; k<num_particles; k++) {
            if (i==k || (i>=num_real && j>=num_real && k>=num_real) )
              continue;
            if (k<num_real) {
              kpt=pdata.mX[ii][k];
              ktype=pdata.particle_types[ii][k];
            } else {
              kpt=pdata.ghost_atoms[ii][k-num_real];
              ktype=pdata.ghost_atom_types[ii][k-num_real];
            }
            calculate_3body(cutsq,i,j,k,ipt,jpt,kpt,pdata.forcei[ii],
                            itype,jtype,ktype,counts3);
          }
      }
    }

    data.ghost_mRF.push_back(data.mRF.back().size());
    data.ghost_3bF.push_back(data.mR1F.back().size());
    
    // Calculation for ghost atoms needed for force
    num_particles=pdata.mX[ii].size()+pdata.ghost_atoms[ii].size();
    for (int i=0; i<num_particles; i++) {
      if (i<num_real) {
        ipt=pdata.mX[ii][i];
        itype=pdata.particle_types[ii][i];
      } else {
        ipt=pdata.ghost_atoms[ii][i-num_real];
        itype=pdata.ghost_atom_types[ii][i-num_real];
      }
        
      if (i==pdata.forcei[ii]) {
        // Calculate 2-body variables
        for (int j=pdata.force_border[ii]; j<pdata.ghost_atoms[ii].size(); j++)
          calculate_distance(cutsq,i,-2,ipt,pdata.ghost_atoms[ii][j],
                             pdata.forcei[ii],
                             pdata.particle_types[ii][pdata.forcei[ii]],
                             pdata.ghost_atom_types[ii][j],counts);
      }

      // Calculate 3-body variables
      for (int j=0; j<num_particles-1; j++) {
        if (i==j)
          continue;
        if (j<num_real) {
          jpt=pdata.mX[ii][j];
          jtype=pdata.particle_types[ii][j];
        } else {
          jpt=pdata.ghost_atoms[ii][j-num_real];
          jtype=pdata.ghost_atom_types[ii][j-num_real];
        }
        double_array rsq=pow(jpt-ipt,2.0);
        if (rsq.sum()<=cutsq || cutsq<0.0)
          for (int k=j+1; k<num_particles; k++) {
            if (i==k || (i>=num_real && j>=num_real && k>=num_real) ||
                (i<num_real+pdata.force_border[ii] && 
                 j<num_real+pdata.force_border[ii] &&
                 k<num_real+pdata.force_border[ii]) )
              continue;
            if (i!=pdata.forcei[ii] && j!=pdata.forcei[ii] && 
                k!=pdata.forcei[ii])
              continue;
            if (k<num_real) {
              kpt=pdata.mX[ii][k];
              ktype=pdata.particle_types[ii][k];
            } else {
              kpt=pdata.ghost_atoms[ii][k-num_real];
              ktype=pdata.ghost_atom_types[ii][k-num_real];
            }
            calculate_3body(cutsq,i,j,k,ipt,jpt,kpt,pdata.forcei[ii],
                            itype,jtype,ktype,counts3);
          }
      }
    }

    if (data.mR.back().empty() && data.mRF.back().empty()) 
      error->generate_error(0,"base_twothree",
       "Lines in data file must contain an energy and >0 pair interactions.");
    if (data.mR1.back().empty() && data.mR1F.back().empty())
      error->generate_error(0,"base_twothree",
       "Lines in data file must contain an energy and >0 3-body interactions.");
    
    if (my_rank==0 && verbose)
      cout << "Loaded configuration with " 
           << data.mR.back().size() + data.ghost_mRF.back()
           << " 2-body terms, " 
           << data.mR1.back().size()+data.ghost_3bF.back() 
           << " 3-body terms, and "
           << data.mRF.back().size()+data.mR1F.back().size() 
           << " force terms...\n";
  }
  if (my_rank == 0) {
    cout << "\n\n"
         << "Pair_Particle_Type Count\n"
         << "------------------------\n";
    for (int i=0; i<data.stypes.size(); i++)
      cout << data.stypes[i] << " " << counts[i] << " " << endl;
    cout << "\n\n"
         << "3body_Particle_Type Count (Center First)\n"
         << "----------------------------------------\n";
    for (int i=0; i<data.stypes3.size(); i++)
      cout << data.stypes3[i] << " " << counts3[i] << " " << endl;
  }
  _num_terms=data.stypes.size();
  _num_terms3=data.stypes3.size();
  
  data.num_configs=pdata.energies.size();
}

void BaseTwoThreeOp::postInit(Beagle::System& ioSystem) {
  GP::System& lSystem = castObjectT<GP::System&>(ioSystem);
  GP::PrimitiveSuperSet& lSuperSet = lSystem.getPrimitiveSuperSet();
  xPrim=lSuperSet[0]->getPrimitiveByName("X");
  aPrim=lSuperSet[0]->getPrimitiveByName("A");
  r1Prim=lSuperSet[0]->getPrimitiveByName("R1");
  r2Prim=lSuperSet[0]->getPrimitiveByName("R2");
  edPrim=lSuperSet[0]->getPrimitiveByName("E_d");
  if (xPrim==NULL || aPrim==NULL || r1Prim==NULL || r2Prim==NULL)
    error->generate_error(0,"BaseTwoThreeOp",
     "Cannot use 3-body potential without X,A,R1, and R2 in the primitive set");
}

void BaseTwoThreeOp::calculate_distance(const double cutsq, const int i,
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

void BaseTwoThreeOp::init_fit_stat() {
  std::valarray<int> mY_sizes(0,eval_mY.size());
  std::valarray<int> mY3_sizes(0,eval_mY3.size());
  std::valarray<int> dm_sizes(0,eval_dm.size());
  std::valarray<int> dm3_sizes(0,eval_dm3.size());
  for (int vi=0; vi<data.num_configs; vi++) {
    mY_sizes[vi]=data.mR[vi].size()+data.ghost_mRF[vi];
    mY3_sizes[vi]=data.mR1[vi].size()+data.ghost_3bF[vi];
    if (_force_flag==1) {
      dm_sizes[vi*3]=data.mRF[vi].size();
      dm3_sizes[vi*3]=data.mR1F[vi].size();
      dm_sizes[vi*3+1]=data.mRF[vi].size();
      dm3_sizes[vi*3+1]=data.mR1F[vi].size();
      dm_sizes[vi*3+2]=data.mRF[vi].size();
      dm3_sizes[vi*3+2]=data.mR1F[vi].size();
    } else if (_force_flag==2) {
      dm_sizes[vi]=data.mRF[vi].size();
      dm3_sizes[vi]=data.mR1F[vi].size();
    }
  }
  if (_energy_flag)
    fitstat_energy->init(eval_mY,mY_sizes,mY3_sizes);
  if (_force_flag)
    fitstat_force->init(eval_dm,dm_sizes,dm3_sizes);
}  

void BaseTwoThreeOp::init_fit_stat(const ParticleConfigs &pdata) {
  eval_mY.resize(pdata.num_configs);
  eval_mY3.resize(pdata.num_configs);
  if (_force_flag==1) {
    eval_dm.resize(pdata.num_configs*3);
    eval_dm3.resize(pdata.num_configs*3);
  } else if (_force_flag==2) {
    eval_dm.resize(pdata.num_configs);
    eval_dm3.resize(pdata.num_configs);
  }
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

void BaseTwoThreeOp::read_data(const std::string &filename,const bool verbose){
  ifstream in;
  a::fileopen(in,filename,*error);
  std::string file_type=get_file_type(in,*error);

  if (_force_flag && file_type=="twothree")
    error->generate_error(0,"pair_potential","File type '"+file_type+
                          "' doesn't support forces. Choose another template.");

  if (file_type=="efxyz")
    read_xyz_data(in,verbose);
  else if (file_type=="efixyz")
    read_xyz_data(in,verbose,true);
  else if (file_type=="twothree")
    read_variable_data(in,verbose);
  else
    error->generate_error(0,"pair_potential","Invalid file type: "+file_type);
  a::fileclose(in,*error);
}

void BaseTwoThreeOp::read_xyz_data(istream &in, const bool verbose,
                                   const bool types) {
  ParticleConfigs pdata;
  read_xyz_configurations(in,pdata,types);
  create_ghost_particles(pdata);
  calculate_variables(pdata,verbose);
  init_fit_stat(pdata);
  pack_data();
  _valid_data=true;
}

void BaseTwoThreeOp::read_variable_data(istream &in, const bool verbose) {
  ParticleConfigs pdata;

  clear_data();

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
    Beagle::Double energy = atof(tok);
    pdata.energies.push_back(energy);
    tok = strtok(NULL," ,\t");
    
    // Pair-wise Distances
    data.mR.push_back(Double_vector());
    data.mRF.push_back(Double_vector());
    data.tR.push_back(int_vector());
    data.tRF.push_back(int_vector());
    if (strcmp(tok,"TWO")==0) {
      tok = strtok(NULL," ,\t");
      while (tok != NULL) {
        if (strcmp(tok,"#")==0 || strcmp(tok,"THREE")==0)
          break;
        data.mR.back().push_back(atof(tok));
        data.tR.back().push_back(0);
        tok = strtok(NULL," ,\t");
      }
    }
    
    // Three-body Distances
    data.mR1.push_back(Double_vector());
    data.mR2.push_back(Double_vector());
    data.mA.push_back(Double_vector());
    data.mR1F.push_back(Double_vector());
    data.mR2F.push_back(Double_vector());
    data.mAF.push_back(Double_vector());
    data.tR3.push_back(int_vector());
    data.tRF3.push_back(int_vector());
    if (strcmp(tok,"THREE")==0) {
      tok=check_tokens(tok);
      while (tok!=NULL) {
        if (strcmp(tok,"#")==0)
          break;
        data.mR1.back().push_back(atof(tok));
        tok=check_tokens(tok);
        data.mR2.back().push_back(atof(tok));
        tok=check_tokens(tok);
        data.mA.back().push_back(atof(tok));
        data.tR.back().push_back(0);
        tok = strtok(NULL," ,\t");
      }
    }
    
    if (data.mR.back().empty())
      error->generate_error(0,"base_twothree",
         "Lines in data file must at least 1 2-body term and 1 3-body term.");
    if (data.mR1.back().empty())
      error->generate_error(0,"base_twothree",
         "Lines in data file must at least 1 2-body term and 1 3-body term.");
    data.ghost_mRF.push_back(data.mRF.back().size());
    data.ghost_3bF.push_back(data.mR1F.back().size());
    
    if (my_rank==0 && verbose)
      cout << "Loaded configuration with " << data.mR.back().size()
           << " 2-body terms and " << data.mR1.back().size() 
           << " 3-body terms...\n";
  }

  if (data.mR.empty())
    error->generate_error(0,"base_twothree",
                            "No configurations in input file.");
  
  data.num_configs=pdata.energies.size();
  pdata.num_configs=pdata.energies.size();
  init_fit_stat(pdata);
  pack_data();
  _valid_data=true;
}

void BaseTwoThreeOp::clear_data() {
  data.num_configs=0;
  data.mR.clear();
  data.mR1.clear();
  data.mR2.clear();
  data.mA.clear();
  data.mRF.clear();
  data.mR1F.clear();
  data.mR2F.clear();
  data.mAF.clear();
  data.dmRF_x.clear();
  data.dmRF_y.clear();
  data.dmRF_z.clear();
  data.dmR1F_x.clear();
  data.dmR1F_y.clear();
  data.dmR1F_z.clear();
  data.dmR2F_x.clear();
  data.dmR2F_y.clear();
  data.dmR2F_z.clear();
  data.dmAF_x.clear();
  data.dmAF_y.clear();
  data.dmAF_z.clear();
  data.ghost_mRF.clear();
  data.ghost_3bF.clear();
}

void BaseTwoThreeOp::add_empty_config() {
  data.num_configs++;
  data.mR.push_back(Double_vector());
  data.mR1.push_back(Double_vector());
  data.mR2.push_back(Double_vector());
  data.mA.push_back(Double_vector());
  data.mRF.push_back(Double_vector());
  data.mR1F.push_back(Double_vector());
  data.mR2F.push_back(Double_vector());
  data.mAF.push_back(Double_vector());
  data.dmRF_x.push_back(Double_vector());
  data.dmRF_y.push_back(Double_vector());
  data.dmRF_z.push_back(Double_vector());
  data.dmR1F_x.push_back(Double_vector());
  data.dmR1F_y.push_back(Double_vector());
  data.dmR1F_z.push_back(Double_vector());
  data.dmR2F_x.push_back(Double_vector());
  data.dmR2F_y.push_back(Double_vector());
  data.dmR2F_z.push_back(Double_vector());
  data.dmAF_x.push_back(Double_vector());
  data.dmAF_y.push_back(Double_vector());
  data.dmAF_z.push_back(Double_vector());
  data.tR.push_back(int_vector());
  data.tRF.push_back(int_vector());
  data.tR3.push_back(int_vector());
  data.tRF3.push_back(int_vector());
}

void BaseTwoThreeOp::fix_dual_equation(GP::Context& ioContext, GP::Tree &lTree) {
  unsigned right_tree;
  if (lTree.size()>1)
    right_tree=lTree[1].mSubTreeSize+1;
  else
    right_tree=0;
  
  for(unsigned i=1; i<lTree.size(); ++i) {
    std::string node_name=lTree[i].mPrimitive->getName();
    if (i<right_tree) {
      if (node_name=="R1" || node_name=="R2" || node_name=="A")
        lTree[i].mPrimitive=xPrim;
      else if (node_name=="E_t")
        lTree[i].mPrimitive=castHandleT<GP::EphemeralT<VDouble> >(edPrim)->
              generate(edPrim->getName(),ioContext);
    } else {
      if (node_name=="X") {
        unsigned selected=ioContext.getSystem().getRandomizer().rollInteger(0,2);
        if (selected==0)
          lTree[i].mPrimitive=r1Prim;
        else if (selected==1)
          lTree[i].mPrimitive=r2Prim;
        else
          lTree[i].mPrimitive=aPrim;
      } else if (node_name=="E_p")
        lTree[i].mPrimitive=castHandleT<GP::EphemeralT<VDouble> >(edPrim)->
              generate(edPrim->getName(),ioContext);
    }
  }
}

void BaseTwoThreeOp::calculate_3body(const double cutsq,const int i,const int j,
                                     const int k, const double_array &ipt,
                                     const double_array &jpt, 
                                     const double_array &kpt,const int force_i,
                                     const int type1, const int type2, 
                                     const int type3, int_vector &counts){
  double xx=jpt[0]-ipt[0];
  double yy=jpt[1]-ipt[1];
  double zz=jpt[2]-ipt[2];
  double rsq=xx*xx+yy*yy+zz*zz;
  double xx2=kpt[0]-ipt[0];
  double yy2=kpt[1]-ipt[1];
  double zz2=kpt[2]-ipt[2];
  double rsq2=xx2*xx2+yy2*yy2+zz2*zz2;
  if (rsq2>cutsq && cutsq>=0.0)
    return;

  int ui=ui_three(type1,type2,type3,data.num_types);
  counts[ui]++;
  if (i!=force_i && j!=force_i && k!=force_i) {
    data.mR1.back().push_back(sqrt(rsq));
    data.mR2.back().push_back(sqrt(rsq2));
    data.mA.back().push_back( (xx*xx2+yy*yy2+zz*zz2)/sqrt(rsq*rsq2) );
    data.tR3.back().push_back(ui);
    return;
  }
  data.mR1F.back().push_back(sqrt(rsq));
  data.mR2F.back().push_back(sqrt(rsq2));
  data.mAF.back().push_back( (xx*xx2+yy*yy2+zz*zz2)/sqrt(rsq*rsq2) );
  if (j==force_i) {
    data.dmR1F_x.back().push_back(sqrt(rsq-xx*xx+(xx+epsilon_p)*(xx+epsilon_p)));
    data.dmR1F_y.back().push_back(sqrt(rsq-yy*yy+(yy+epsilon_p)*(yy+epsilon_p)));
    data.dmR1F_z.back().push_back(sqrt(rsq-zz*zz+(zz+epsilon_p)*(zz+epsilon_p)));
    data.dmR2F_x.back().push_back(sqrt(rsq2));
    data.dmR2F_y.back().push_back(sqrt(rsq2));
    data.dmR2F_z.back().push_back(sqrt(rsq2));
    data.dmAF_x.back().push_back(((xx+epsilon_p)*xx2+yy*yy2+zz*zz2)/static_cast<double>(data.dmR1F_x.back().back()*data.dmR2F_x.back().back()));
    data.dmAF_y.back().push_back(((yy+epsilon_p)*yy2+xx*xx2+zz*zz2)/static_cast<double>(data.dmR1F_y.back().back()*data.dmR2F_y.back().back()));
    data.dmAF_z.back().push_back(((zz+epsilon_p)*zz2+yy*yy2+xx*xx2)/static_cast<double>(data.dmR1F_z.back().back()*data.dmR2F_z.back().back()));
    data.tRF3.back().push_back(ui);
  } else if (k==force_i) {
    data.dmR2F_x.back().push_back(sqrt(rsq2-xx2*xx2+(xx2+epsilon_p)*(xx2+epsilon_p)));
    data.dmR2F_y.back().push_back(sqrt(rsq2-yy2*yy2+(yy2+epsilon_p)*(yy2+epsilon_p)));
    data.dmR2F_z.back().push_back(sqrt(rsq2-zz2*zz2+(zz2+epsilon_p)*(zz2+epsilon_p)));
    data.dmR1F_x.back().push_back(sqrt(rsq));
    data.dmR1F_y.back().push_back(sqrt(rsq));
    data.dmR1F_z.back().push_back(sqrt(rsq));
    data.dmAF_x.back().push_back(((xx2+epsilon_p)*xx+yy*yy2+zz*zz2)/static_cast<double>(data.dmR2F_x.back().back()*data.dmR1F_x.back().back()));
    data.dmAF_y.back().push_back(((yy2+epsilon_p)*yy+xx*xx2+zz*zz2)/static_cast<double>(data.dmR2F_y.back().back()*data.dmR1F_y.back().back()));
    data.dmAF_z.back().push_back(((zz2+epsilon_p)*zz+yy*yy2+xx*xx2)/static_cast<double>(data.dmR2F_z.back().back()*data.dmR1F_z.back().back()));
    data.tRF3.back().push_back(ui);
  } else {
    data.dmR1F_x.back().push_back(sqrt(rsq-xx*xx+(xx-epsilon_p)*(xx-epsilon_p)));
    data.dmR1F_y.back().push_back(sqrt(rsq-yy*yy+(yy-epsilon_p)*(yy-epsilon_p)));
    data.dmR1F_z.back().push_back(sqrt(rsq-zz*zz+(zz-epsilon_p)*(zz-epsilon_p)));
    data.dmR2F_x.back().push_back(sqrt(rsq2-xx2*xx2+(xx2-epsilon_p)*(xx2-epsilon_p)));
    data.dmR2F_y.back().push_back(sqrt(rsq2-yy2*yy2+(yy2-epsilon_p)*(yy2-epsilon_p)));
    data.dmR2F_z.back().push_back(sqrt(rsq2-zz2*zz2+(zz2-epsilon_p)*(zz2-epsilon_p)));
    data.dmAF_x.back().push_back(((xx-epsilon_p)*(xx2-epsilon_p)+yy*yy2+zz*zz2)/static_cast<double>(data.dmR1F_x.back().back()*data.dmR2F_x.back().back()));
    data.dmAF_y.back().push_back(((yy-epsilon_p)*(yy2-epsilon_p)+xx*xx2+zz*zz2)/static_cast<double>(data.dmR1F_y.back().back()*data.dmR2F_y.back().back()));
    data.dmAF_z.back().push_back(((zz-epsilon_p)*(zz2-epsilon_p)+yy*yy2+xx*xx2)/static_cast<double>(data.dmR1F_z.back().back()*data.dmR2F_z.back().back()));
    data.tRF3.back().push_back(ui);
  }
}

Beagle::Fitness::Handle BaseTwoThreeOp::fit_stat(const int totaln) {
  double fitness;
  if (_energy_flag) {
    fitness=fitstat_energy->parsimony_fitness(eval_mY,eval_mY3,totaln);
    if (_force_flag) {
      if (ols_capable()) {
        double a,b,c,d;
        fitstat_energy->ols_correct(a,b,c,d);
        fitness+=fitstat_force->parsimony_fitness(eval_dm,eval_dm3,totaln,
                                                  a,b,c,d);
      } else
        fitness+=fitstat_force->parsimony_fitness(eval_dm,eval_dm3,totaln);
      fitness/=2.0;
    }
  } else
    fitness=fitstat_force->parsimony_fitness(eval_dm,eval_dm3,totaln);
  
  return new FitnessSimple(fitness);
}
