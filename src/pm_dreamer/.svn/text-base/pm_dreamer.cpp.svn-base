/***************************************************************************
                                pm_dreamer.cpp
                             -------------------
                               W. Michael Brown

  Driver for PM-Dreamer

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

#include "commandline.h"
#include "timekeeper.h"
#include "fit_stat.h"
#include "beagle/GP.hpp"
#include "VDouble.hpp"
#include "PowT.hpp"
#include "VDivide.hpp"
#include "VCos.hpp"
#include "VSin.hpp"
#include "VExp.hpp"
#include "VLog.hpp"
#include "EphemeralInt.hpp"
#include "EphemeralD.hpp"
#include "pair_potential.h"
#include "vpair_potential.h"
#include "two_three_potential.h"
#include "vtwo_three_potential.h"
#include "MPITerminateOp.hpp"
#include "MigrationMPIOp.hpp"
#include "HFCompMPIOp.hpp"
#include "HybridOptOp.hpp"
#include "SimplifyOp.hpp"
#include "OLSCorrectOp.hpp"
#include "DreamerOp.hpp"
#include "TermMaxTimeOp.hpp"
#include "StatsCalcFitSimpleMPIOp.hpp"
#include "RestartReadOp.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <string>

using namespace std;
using namespace Beagle;

// Describe the program parameters
void Describe(CommandLine &cl,ostream &out);
// Parse the command line parameters
void HandleArgs(CommandLine &, int, char **, Error *,const int rank);
// Add Manpage Documentation
void AddChapters(CommandLine &cl);

// If a primitive is in the superset replace, otherwise add
void replace_primitive(GP::PrimitiveSuperSet::Handle lSupSet,
                       GP::Primitive::Handle in) {
  GP::PrimitiveMap &pmap=lSupSet->getPrimitiveMap();
  GP::PrimitiveMap::iterator lMapI = pmap.find(in->getName());
  if(lMapI == pmap.end())
    lSupSet->addPrimitive(in);
  else
    lMapI->second=in;
}

int main(int argc, char *argv[]) {
  #ifdef GPMPI
  MPI_Init(&argc,&argv);
  #endif

  Error error;
  CommandLine cl;
  TimeKeeper tk;
  
  int my_rank=0, num_procs=1;

  #ifndef GPMPI
  tk.start();
  #else
  double mpi_time=MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  #endif

  // Get the filename for input data
  HandleArgs(cl,argc,argv,&error,my_rank);
  std::string filename;
  std::string confname;
  if (cl.argsize()==2) {
    filename=cl.argstring(' ',0);
    confname=cl.argstring(' ',1);
    // Test that the configuration file exists
    ifstream in;
    a::fileopen(in,confname,error);
    a::fileclose(in,error);
  }
  
  // Use vectorized evaluation?
  bool use_vectors=true;
  if (cl['z'])
    use_vectors=false;
  
  // Random Seed
  ULong rand_seed=1;
  if (cl['r'])
    rand_seed=cl.arglong('r',0);
  if (rand_seed==ULong(0))
    error.generate_error(0,"pm_dreamer","Cannot use 0 as a random seed.");
  rand_seed+=ULong(1000000)*ULong(my_rank);
  
  // Program output
  UInt verbose=0;
  if (my_rank==0 && cl['n'])
    verbose=cl.argint('n',0)/10;
  
  // Output filename
  std::string header="gp_force";
  if (cl['f'])
    header=cl.argstring('f',0);
  if (num_procs>1)
    header+="_"+a::itoa(my_rank);
  
  std::string restart_fn=header+".obm.gz";
  if (cl['c']) {
    // See if the file exists
    ifstream in(restart_fn.c_str());
    if (!in || in.fail()) {
      in.clear();
      restart_fn=header+".obm";
      in.open(restart_fn.c_str());
      if (!in || in.fail())
        error.generate_error(0,"pm_dreamer","Could not find "+restart_fn+
           " or "+restart_fn+".gz to restart optimization. Do you need -f?");
    }
    in.close();
  }

  // Potential style
  std::string potential_style="pair";
  bool three_body;
  if (cl['t'])
    potential_style=cl.argstring('t',0);
  if (potential_style=="pair" || potential_style=="paird" ||
      potential_style=="pairf" || potential_style=="pairf1")
    three_body=false;
  else if (potential_style=="twothree" || potential_style=="twothreef1" ||
           potential_style=="twothreed" || potential_style=="twothreef")
    three_body=true;
  else {
    if (my_rank==0)
      std::cerr << "Invalid potential_style: -t " << potential_style 
                << std::endl;
    exit(1);
  }
  
  // Tokens for the potential style
  std::vector<std::string> token_list;
  token_list.push_back("X");
  if (three_body) {
    token_list.push_back("R1");
    token_list.push_back("R2");
    token_list.push_back("A");
  }
  
  if (my_rank==0)
    error.note[6] << "Running " << argv[0] << " on " << num_procs
                  << " processor(s) with random seed " << rand_seed 
                  << endl << endl;
	
  GP::EvaluationOp::Handle lEvalOp, OLSFitOp, RMSDFitOp;
  int energy_flag, force_flag;
  if (potential_style=="pair" || potential_style=="paird" ||
      potential_style=="twothree" || potential_style=="twothreed")
    energy_flag=1;
  else
    energy_flag=0;
  
  if (potential_style=="pair" || potential_style=="twothree")
    force_flag=0;
  else if (potential_style=="pairf1" || potential_style=="twothreef1")
    force_flag=2;
  else
    force_flag=1;
  
  // Fitness statistic?
  FitStat *EnergyFitStat,*ForceFitStat;
  OLSAdaptiveRMSD OLSEnergyStat, OLSForceStat;
  AdaptiveRMSD RMSDEnergyStat, RMSDForceStat;
  std::string fitstat_type="RMSD";
  if (cl['s'])
    fitstat_type=cl.argstring('s',0);
  if (fitstat_type=="RMSD") {
    EnergyFitStat=new AdaptiveRMSD;
    ForceFitStat=new AdaptiveRMSD;
  } else if (fitstat_type=="OLS") {
    EnergyFitStat=new OLSAdaptiveRMSD;
    if (energy_flag==1)
      ForceFitStat=new AdaptiveRMSD;
    else
      ForceFitStat=new OLSAdaptiveRMSD;
  } else if (fitstat_type=="CORR") {
    EnergyFitStat=new FitCorrCoef;
    ForceFitStat=new FitCorrCoef;
  } else
    error.generate_error(0,"pm_dreamer","Unknown Fitness Statistic Type.");
  
  if (cl['g']) {
    unsigned start_size, end_size;
    start_size=cl.argint('g',0);
    end_size=cl.argint('g',1);
    EnergyFitStat->set_parsimony_scale(start_size,end_size);
    ForceFitStat->set_parsimony_scale(start_size,end_size);
  }

  if (use_vectors==false) {
    if (potential_style=="pair" || potential_style=="paird" ||
        potential_style=="pairf" || potential_style=="pairf1") {
      lEvalOp = new PairPotentialOp(&error,EnergyFitStat,
                                    ForceFitStat,energy_flag,force_flag);
      castHandleT<BasePotentialOp>(lEvalOp)->read_data(filename,true);
      OLSFitOp = new PairPotentialOp(&error,&OLSEnergyStat,
                                     &OLSForceStat,energy_flag,force_flag,
                                     *(castHandleT<PairPotentialOp>(lEvalOp)));
      RMSDFitOp = new PairPotentialOp(&error,&RMSDEnergyStat,
                                      &RMSDForceStat,energy_flag,force_flag,
                                      *(castHandleT<PairPotentialOp>(lEvalOp)));
    } else {
      lEvalOp = new TwoThreePotentialOp(&error,EnergyFitStat,
                                        ForceFitStat,energy_flag,force_flag);
      castHandleT<BasePotentialOp>(lEvalOp)->read_data(filename,true);
      OLSFitOp = new TwoThreePotentialOp(&error,&OLSEnergyStat,
                                         &OLSForceStat,energy_flag,force_flag,
                                         *(castHandleT<TwoThreePotentialOp>(lEvalOp)));
      RMSDFitOp = new TwoThreePotentialOp(&error,&RMSDEnergyStat,
                                          &RMSDForceStat,energy_flag,force_flag,
                                          *(castHandleT<TwoThreePotentialOp>(lEvalOp)));
    }
  } else {
    if (potential_style=="paird" || potential_style=="pairf" ||
        potential_style=="pairf1" || potential_style=="pair") {
      lEvalOp = new VPairPotentialOp(&error,EnergyFitStat,
                                      ForceFitStat,energy_flag,force_flag);
      castHandleT<BasePotentialOp>(lEvalOp)->read_data(filename,true);
      OLSFitOp = new VPairPotentialOp(&error,&OLSEnergyStat,
                                      &OLSForceStat,energy_flag,force_flag,
                                      *(castHandleT<VPairPotentialOp>(lEvalOp)));
      RMSDFitOp = new VPairPotentialOp(&error,&RMSDEnergyStat,
                                       &RMSDForceStat,energy_flag,force_flag,
                                       *(castHandleT<VPairPotentialOp>(lEvalOp)));
    } else {
      lEvalOp = new VTwoThreePotentialOp(&error,EnergyFitStat,
                                         ForceFitStat,energy_flag,force_flag);
      castHandleT<BasePotentialOp>(lEvalOp)->read_data(filename,true);
      OLSFitOp = new VTwoThreePotentialOp(&error,&OLSEnergyStat,
                                          &OLSForceStat,energy_flag,force_flag,
                                          *(castHandleT<VTwoThreePotentialOp>(lEvalOp)));
      RMSDFitOp = new VTwoThreePotentialOp(&error,&RMSDEnergyStat,
                                           &RMSDForceStat,energy_flag,force_flag,
                                           *(castHandleT<VTwoThreePotentialOp>(lEvalOp)));
    }
  }
  
  // Get atom type information for E_dt primitive
  int num_types=castHandleT<BasePotentialOp>(lEvalOp)->num_types();
  int num_terms=castHandleT<BasePotentialOp>(lEvalOp)->num_terms();
  int num_terms3=castHandleT<BasePotentialOp>(lEvalOp)->num_terms3();
  const vector<int> *lookup, *lookup3;
  lookup=castHandleT<BasePotentialOp>(lEvalOp)->pair_types();
  lookup3=castHandleT<BasePotentialOp>(lEvalOp)->pair_types3();
  if (num_types>1 && lookup->empty())
    error.generate_error(0,"pm_dreamer",
      "The chosen template does not support multiple particle types.");
  
  try {
    // 1: Build primitives.
    GP::PrimitiveSuperSet::Handle lSupSet = new GP::PrimitiveSuperSet;
    if (use_vectors) {
      replace_primitive(lSupSet,new GP::AddT<VDouble>);
      replace_primitive(lSupSet,new GP::SubtractT<VDouble>);
      replace_primitive(lSupSet,new GP::MultiplyT<VDouble>);
      replace_primitive(lSupSet,new GP::VDivide);
      replace_primitive(lSupSet,new GP::PowT<VDouble>);
      replace_primitive(lSupSet,new GP::AbsT<VDouble>);
      replace_primitive(lSupSet,new GP::VSin);
      replace_primitive(lSupSet,new GP::VCos);
      replace_primitive(lSupSet,new GP::VExp);
      replace_primitive(lSupSet,new GP::VLog);
      for (int i=0; i<token_list.size(); i++)
        replace_primitive(lSupSet,new GP::TokenT<VDouble>(token_list[i]));
      replace_primitive(lSupSet,new GP::VEphemeralInt);
      replace_primitive(lSupSet,new GP::VEphemeralD);
      replace_primitive(lSupSet,new GP::TokenT<VDouble>("Pi", VDouble(M_PI)));
      if (num_terms>1)
        replace_primitive(lSupSet,new GP::VEphemeralD(num_terms,lookup));
      if (num_terms3>1)
        replace_primitive(lSupSet,new GP::VEphemeralD(num_terms3,lookup3,
                                                      NULL,"E_t"));
    } else {
      lSupSet->addPrimitive(new GP::Pow);
      for (int i=0; i<token_list.size(); i++)
        lSupSet->addPrimitive(new GP::TokenT<Double>(token_list[i]));
      lSupSet->addPrimitive(new GP::EphemeralInt);
      lSupSet->addPrimitive(new GP::EphemeralD);
      lSupSet->addPrimitive(new GP::TokenT<Double>("Pi", Double(M_PI)));
      lSupSet->addPrimitive(new GP::EphemeralDouble);
    }

    // 2: Build a system.
    GP::System::Handle lSystem = new GP::System(lSupSet);
    // Turn off console IO
    Register::Description io_level_d("Console level","Int","0","Console out");
    lSystem->getRegister().addEntry("lg.console.level",new UInt(verbose),io_level_d);
    // Set the random seed
    Register::Description rand_d("Random Seed","Long","0","Random Seed");
    lSystem->getRegister().addEntry("ec.rand.seed",new ULong(rand_seed),rand_d);
    // Set the output filenames
    Register::Description lfiled("Log Filename","String","0","Log Filename");
    lSystem->getRegister().addEntry("lg.file.name",new String(header+".log"),lfiled);
    Register::Description ofiled("Milestone Filename","String","0","Milestone Filename");
    lSystem->getRegister().addEntry("ms.write.prefix",new String(header),ofiled);
    // Restart an old run
    if (cl['c']) {
      Register::Description mres_d("Milestone Restart Filename","String","","Milestone Restart Filename");
      lSystem->getRegister().addEntry("ms.restart.file",new String(restart_fn),mres_d);
    }
    if (cl['p']) {
      int hfc_copy_num=cl.argint('p',0);
      if (hfc_copy_num==0)
        error.generate_error(0,"pm_dreamer","hfc_copy_num must be >0");
      if (num_procs%hfc_copy_num!=0)
        error.generate_error(0,"pm_dreamer",
                             "hfc_copy_num does not evenly divide procs.");
      Register::Description mcn("HFC Copy Number","Int","1","HFC Copy Number");
      lSystem->getRegister().addEntry("ec.hfc.copy_num",new Int(hfc_copy_num),
                                      mcn);
      Register::Description msp("MPI Migration Split","Int","0","Migration Split");
      lSystem->getRegister().addEntry("ec.mig.mpi_split",new Int(hfc_copy_num),
                                      msp);
    }
    // 4: Build an evolver and a vivarium.
    GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
    Operator::Handle term_op=new MPITerminateOp;
    MutationOp::Handle simp_op=new SimplifyOp(lEvalOp,&error);
    Operator::Handle stats_op=new GP::StatsCalcFitSimpleMPIOp(three_body);
    lEvolver->addOperator(term_op);
    lEvolver->addOperator(stats_op);
    lEvolver->addOperator(simp_op);
    lEvolver->addOperator(new MigrationMPIOp);
    lEvolver->addOperator(new HFCompMPIOp);

    MutationOp::Handle hyb_op, rmsd_hyb_op, ols_op;
    if (use_vectors) {
      ols_op=new OLSCorrectOp<VDouble>(OLSFitOp,&error,three_body);
      hyb_op=new HybridOptOp<VDouble>(lEvalOp,simp_op);
      rmsd_hyb_op=new HybridOptOp<VDouble>(RMSDFitOp,simp_op);
      lEvolver->removeOperator("GP-MutationEphemeralDoubleOp");
      lEvolver->addOperator(new GP::MutationEphemeralOpT<VDouble>
            ("gp.mutephdbl.indpb","gp.mutephdbl.primit",
             "GP-MutationEphemeralDoubleOp"));
      if (num_terms>1)
        lEvolver->addOperator(new GP::MutationEphemeralOpT<VDouble>
              ("gp.mute2c.indpb","gp.mute2c.primit",
               "GP-MutationEphemeral2COp"));
      if (num_terms3>1)
        lEvolver->addOperator(new GP::MutationEphemeralOpT<VDouble>
              ("gp.mute3c.indpb","gp.mute3c.primit",
               "GP-MutationEphemeral3COp"));
    } else {
      ols_op=new OLSCorrectOp<Double>(OLSFitOp,&error,three_body);
      hyb_op=new HybridOptOp<Double>(lEvalOp,simp_op);
      rmsd_hyb_op=new HybridOptOp<Double>(RMSDFitOp,simp_op);
    }
    
    lEvolver->addOperator(ols_op);
    lEvolver->addOperator(hyb_op);
    lEvolver->addOperator(new DreamerOp(term_op,stats_op,hyb_op, ols_op, 
                                        rmsd_hyb_op,lEvalOp,RMSDFitOp,
                                        energy_flag,force_flag));
    lEvolver->addOperator(new TermMaxTimeOp);
    lEvolver->addOperator(new RestartReadOp(lEvalOp));
    //lEvolver->addBootStrapOp("MPITerminateOp");
    //lEvolver->addMainLoopOp("MPITerminateOp");
    GP::Vivarium::Handle lVivarium = new GP::Vivarium;
    // 5: Initialize and evolve the vivarium.
    if (my_rank==0)
      cout << "\nInitializing system...\n";
    lEvolver->initialize(lSystem, confname);
    if (cl.argsize()==0)
      error.generate_error(0,"pm_dreamer","No input data file specified");
    if (cl.argsize()==1)
      error.generate_error(0,"pm_dreamer","No configuration file specified");
    if (my_rank==0)
      cout << "\nPerforming evolution...\n";
    lEvolver->evolve(lVivarium);
    if (my_rank==0)
      cout << "Output written to gp_force.log and gp_force.obm(.gz)\n"
           << "Done. ";
  }
  catch(Exception& inException) {
    inException.terminate();
  }
  catch(exception& inException) {
    cerr << "Standard exception catched:" << endl;
    cerr << inException.what() << endl << flush;
    return 1;
  }
  catch(...) {
    cerr << "Unknown exception catched!" << endl << flush;
    return 1;
  }

  delete EnergyFitStat, ForceFitStat, OLSEnergyStat, OLSForceStat;
  
  #ifndef GPMPI
  tk.end();
  tk.print(cout);
  #else
  if (my_rank==0)
    cout << MPI_Wtime()-mpi_time << " seconds.\n\n";
  MPI_Finalize();
  #endif
  
  return 0;
}

void Describe(CommandLine &cl,ostream &out) {
  std::string name=cl.program_name();
  std::string progname=a::strcenter(name,70);
  std::string gridversion=a::strcenter("Version 0.2",70);
  out << endl << progname << endl
      << "______________________________________________________________________\n"
      << a::strcenter("W. Michael Brown",70) << endl
      << a::strcenter("6/6/2008",70) << endl
      << "______________________________________________________________________\n"
      << "Perform functional optimization of force fields using GP\n\n";
  cl.write_text_synopsis(out,70);
  out << endl << "Use '" << name << " -h > " << name
      << ".1' to generate a man page for this\n"
      << "program and type 'man ./" << name << ".1' for help\n"
      << "______________________________________________________________________\n";
  return;
}

void HandleArgs(CommandLine &cl, int argc, char *argv[], Error *error,
                const int my_rank) {
  // Arguments
  cl.addmanditory(' ',2);
  cl.addargname(' ',"input_data");
  cl.addargname(' ',"beagle_config_file");

  cl.add('t',1);
  cl.addargname('t',"energy_type");
  cl.adddescription('t',"Specify the functional form of the energy function");
  // Stuff for every executable
  cl.addhelp('h',0);
  cl.adddescription('h',"Print out the man page for help");
  cl.add('n',1);
  cl.addargname('n',"notice_level");
  cl.adddescription('n',"Set the degree of program output.  Use: \n\n\t-n  0\tNo output\n\t-n 10\tNormal program output\n\t-n 20\tParameters useful for reproducing the results\n\t-n 30\tAll output. The degree of Open Beagle Output changes at 10,20, and 30.");
  cl.add('r',1);
  cl.addargname('r',"rseed");
  cl.adddescription('r',"Specify the random seed (unsigned long). Default is 1.");
  cl.add('f',1);
  cl.addargname('f',"file_header");
  cl.adddescription('f',"Specify the header for the .log output file and the .obm milestone files. The default is gp_force.");
  cl.add('c',0);
  cl.adddescription('c',"Restart from existing milestone files. When restarting, the functional form (-t), the beagle_config_file file, and the input_data can be different from those used in the original run. This allows the user to change parameters and/or add data to refine runs. If the -f flag was used to specify a non-default file header for the restart files. The -f flag should also be specified again with the same name when using -r. When running in parallel, the same number of processes should be used for the restart. If a smaller number is used, the extra individuals will be ignored. If a larger number is used, an error is generated. When restarting an optimization, the generation number starts at the last generation in the milestone file. Therefore, the maxgens termination criterion may need to be increased. The restart files are read by the ReadRestartOp in the beagle_config_file");
  cl.add('z',0);
  cl.adddescription('z',"Disable vectorized tree evaluation. This will typically be at least 4x slower for the optimized Beagle library and greater than 15x slower for the unoptimzed library.");
  cl.add('s',1);
  cl.addargname('s',"stat_type");
  cl.adddescription('s',"Choose the fitness statistic used. Options are RMSD for adaptive RMSD, CORR for the Pearson correlation coefficient, and OLS for ordinary least squares fitting. See the Fitness section for details on each method.");
  cl.add('g',2);
  cl.addargname('g',"start_size");
  cl.addargname('g',"end_size");
  cl.adddescription('g',"Scale the fitness by the number of nodes in the tree. This can be used to reduce the average size of individuals. A tree with start_size or smaller nodes has a maximum fitness of 1.0. A tree with end_size or greater nodes has a maximum fitness of 0.0.");
  cl.add('p',1);
  cl.addargname('p',"hfc_copy_num");
  cl.adddescription('p',"Sets the ec.hfc.copy_num register as described below for HFC from the commandline. ec.mig.mpi_split is set to the same value to allow use with the MPI migration operator.");

  // Parse the commandline
  if (!cl.parse(argc,argv,error)) {
    if (my_rank==0)
      Describe(cl,cout);
    error->generate_error(0,a::filenameonly(argv[0]),"Bad Command Line\n");
  }

  // Set the notice level
  if (cl['n'])
    error->note.set_notice_level(cl.argint('n',0));

  // Generate a notice with the command line for records purposes
  std::string cm=cl.program_name();
  for (int j=1; j<argc; j++)
    cm+=' '+std::string(argv[j]);
  cm+="\n";
  error->note.notice(19,"CommandLine",cm);

  // Output the help
  if (cl['h']) {
    AddChapters(cl);
    if (my_rank==0)
      cl.write_man_page(cout,"0.2","PM-Dreamer Utilities");
    exit(0);
  }
}
