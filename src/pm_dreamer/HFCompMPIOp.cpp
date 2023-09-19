/***************************************************************************
                               HFCompMPIOp.cpp
                             -------------------
                               W. Michael Brown

  Operator for Hierachical Fair Competition between MPI procs

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun  2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#include "beagle/Beagle.hpp"
#include "HFCompMPIOp.hpp"

#include <cmath>
#include <algorithm>

#ifdef GPMPI
#include "mpi.h"
#endif

using namespace Beagle;


HFCompMPIOp::HFCompMPIOp(Beagle::string inName) :
  ReplacementStrategyOp(inName), my_rank(0), num_islands(1), adaptive(true),
  old_popsize(0)
{
  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_islands);
  #endif
  ifitnesses=new float[num_islands];
}

void HFCompMPIOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  ReplacementStrategyOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ec.hfc.percentile")) {
    mHFCPercentile =
      castHandleT<Float>(ioSystem.getRegister().getEntry("ec.hfc.percentile"));
  } else {
    mHFCPercentile = new Float(0.85f);
    string lLongDescrip = "Percentile of fitness measure to use as HFC migration threshold ";
    lLongDescrip += "of next deme. For example, a threshold of 0.85 means that the fitness ";
    lLongDescrip += "used as threshold to accept migrant into following deme is taken as the ";
    lLongDescrip += "fitness of the individual that is better than 85% of the other individuals ";
    lLongDescrip += "in its deme.";
    Register::Description lDescrip(
      "HFC percentile",
      "Float",
      "0.85",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.percentile", mHFCPercentile, lDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.interval")) {
    mMigrationInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hfc.interval"));
  } else {
    mMigrationInterval = new UInt(1);
    string lLongDescrip = "Interval between each hierarchical fair competition ";
    lLongDescrip += "migration, in number of generations. ";
    lLongDescrip += "An interval of 0 disables HFC migrations.";
    Register::Description lIntervalDescrip(
      "Interval between HFC migrations",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.interval", mMigrationInterval, lIntervalDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.copy_num")) {
    mHFCCopyNum =
        castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hfc.copy_num"));
  } else {
    mHFCCopyNum = new UInt(1);
    string lCopyDescrip = "Number of islands that have the same fitness ";
    lCopyDescrip += "thresholds.";
    Register::Description lCopyR(
        "Number of islands that have the same fitness thresholds",
        "UInt",
        "1",
        lCopyDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.copy_num", mHFCCopyNum, 
                                    lCopyR);
  }

  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.first")) {
    mFirstThreshold =
      castHandleT<Float>(ioSystem.getRegister().getEntry("ec.hfc.first"));
  } else {
    mFirstThreshold = new Float(-1.0f);
    string lLongDescrip = "First threshold used for migration if adaptive thresholds are not used. ";
    lLongDescrip += "If the value is negative, adaptive thresholds are used according to the ";
    lLongDescrip += "ec.hfc.percentile register. If positive, ec.hfc.percentile is ignored and a ";
    lLongDescrip += "set of thresholds is used from ec.hfc.first up to 1.0.";
    Register::Description lDescrip(
      "HFC First Threshold",
      "Float",
      "-1.0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.first", mFirstThreshold, lDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.scale")) {
    mScale =
      castHandleT<Float>(ioSystem.getRegister().getEntry("ec.hfc.scale"));
  } else {
    mScale = new Float(1.0f);
    string lLongDescrip = "Ratio between the fitness threshold differences of populations ";
    lLongDescrip += "in a geometric series. If adaptive scaling is used (ec.hfc.percentile>0) ";
    lLongDescrip += "this parameter is ignored. Otherwise, the differences between ";
    lLongDescrip += "populations are scaled using this parameter. Default 1.0.";
    Register::Description lDescrip(
      "HFC Fitness Threshold Scaling",
      "Float",
      "1.0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.scale", mScale, lDescrip);
  }

  Beagle_StackTraceEndM("void HFCompMPIOp::initialize(System& ioSystem)");
}

void HFCompMPIOp::initialize_thresholds(Deme& ioDeme, Context& ioContext)
{
  Beagle_AssertM(mPopSize->size()!=0);
  mFitnessThresholds.resize(0);
  mFitnessThresholds.resize(mPopSize->size());

  // Number of islands with the same fitness thresholds
  num_copies=mHFCCopyNum->getWrappedValue();
  if (num_copies<1) {
    Beagle_LogBasicM(
        ioContext.getSystem().getLogger(),
        "HFC", "Beagle::HFCompMPIOp",
        string("Warning: ec.hfc.copy_num must be >0. Using 1.")
    );
    num_copies=1;
  }
  if (num_islands%num_copies!=0) {
    Beagle_LogBasicM(
        ioContext.getSystem().getLogger(),
        "HFC", "Beagle::HFCompMPIOp",
        string("Warning: # procs is not evenly divisible by ec.hfc.copy_num. Using 1.")
    );
    num_copies=1;
  }
  if (num_islands/num_copies==0) {
    Beagle_LogBasicM(
        ioContext.getSystem().getLogger(),
        "HFC", "Beagle::HFCompMPIOp",
        string("Warning: ec.hfc.copy_num is > half # of islands. Using 1.")
    );
    num_copies=1;
  }

  // My copy rank is the highest rank with the same fitness threshold
  my_copy_rank=my_rank+num_copies-my_rank%num_copies-1;

  float first=mFirstThreshold->getWrappedValue();
  if (first==0 || first>=1) {
    std::cerr << "ec.hfc.first cannot be 0 or >= 1. Using adaptive thresholds.\n";
    first=-1;
  }
  if (first<0) {
    adaptive=true;
  } else {
    adaptive=false;
    float r=mScale->getWrappedValue();
    if (r<0) {
      std::cerr << "ec.hfc.scale must be >0. Using a value of 1.0.\n";
      r=1.0;
    }
    
    int nI=num_islands/num_copies;

    float t_inc;
    if (r==1.0 || nI*mPopSize->size()<3)
      t_inc=(1.0-first)/static_cast<float>(nI*mPopSize->size()-1);
    else
      t_inc=(1.0-first)*(1.0-r)/(1.0-pow(static_cast<double>(r),
                          static_cast<double>(nI*mPopSize->size()-2)));

    float t=first;
    // Set the fitness thresholds for the islands
    for (int i=0; i<num_islands; ) {
      float t_copy=t;
      int ie=i+num_copies;
      for (; i<ie; i++) {
        t=t_copy;
        if (i<num_copies)
          ifitnesses[i]=0;
        else
          ifitnesses[i]=t;
        for (int j=0; j<mPopSize->size(); j++) {
          if (my_rank==i) {
            mFitnessThresholds[j]=castHandleT<Fitness>
                (ioDeme[0]->getFitnessAlloc()->allocate());
            castHandleT<FitnessSimple>(mFitnessThresholds[j])->setValue(Float(t));
          }
          if (i>=num_copies || j!=0) {
            t+=t_inc;
            t_inc*=r;
          }
        }
      }
    }
  }
  
  if (adaptive==false && num_islands>0 && my_rank==0 && num_copies>1) {
    std::cout << "--------------------------------\n"
              << "| Fitness Thresholds by Island: \n"
              << "| Copies: " << num_copies << std::endl
              << "--------------------------------\n"
              << "| Proc  Fitness\n"
              << "--------------------------------\n";
    for (int i=0; i<num_islands; i++)
      std::cout << i << " " << ifitnesses[i] << std::endl;
    std::cout << "--------------------------------\n\n";
  }

}

void HFCompMPIOp::update_fitnesses(Deme& ioDeme, Context& ioContext)
{
  // Update fitness threshold.
  if(!(ioContext.getDemeIndex() == 0 && my_rank==0)) {
    Beagle_AssertM(mHFCPercentile->getWrappedValue() < 1.0);
    const unsigned int lThresholdIndex =
      (unsigned int)std::ceil((1.0-mHFCPercentile->getWrappedValue()) * float(ioDeme.size()-1));
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    for(unsigned int i=0; i<lThresholdIndex; ++i) {
      std::pop_heap(ioDeme.begin(), ioDeme.end()-i, IsLessPointerPredicate());
    }
    mFitnessThresholds[ioContext.getDemeIndex()] =
      castHandleT<Fitness>(ioDeme[0]->getFitnessAlloc()->clone(*ioDeme[0]->getFitness()));
  }

  // Get fitness thresholds from all procs
  #ifdef GPMPI
  if (ioContext.getDemeIndex() == 0) {
    float my_fitness;
    if (my_rank==0)
      my_fitness=0;
    else
      my_fitness=castHandleT<FitnessSimple>(mFitnessThresholds[0])->getValue();
    MPI_Allgather(&my_fitness,1,MPI_FLOAT,ifitnesses,1,MPI_FLOAT,
                  MPI_COMM_WORLD);
  }
  #endif
}

bool HFCompMPIOp::inds_from_string(Deme &ioDeme, Context &ioContext,
                                   bool &lChanged) {
  for (int j=0; j<received_inds.size(); j++) {
    // Get the number of individuals to read
    std::istringstream in;
    in.str(received_inds[j]);
    int number;
    in >> number;
    if (!in)
      return false;
    if (number==0)
      return true;
    
    PACC::XML::Document lParser(in);
    PACC::XML::ConstFinder lIndivFinder = 
                           (PACC::XML::ConstIterator)lParser.getFirstDataTag();

    PACC::XML::ConstIterator lIndivTag = lIndivFinder.find("//Individual");
    for (unsigned i=0; i<number; i++) {
      if(!lIndivTag) return false;
      Individual::Handle lNewIndiv =
        castHandleT<Individual>(ioContext.getDeme().getTypeAlloc()->allocate());
      ioContext.setIndividualHandle(lNewIndiv);
      ioContext.setIndividualIndex(0);
      lNewIndiv->readWithContext(lIndivTag,ioContext);
      ioDeme.push_back(lNewIndiv);
      lChanged = true;
      lIndivTag=lIndivFinder.findNext();
    }
  }
  return true;
}  

void HFCompMPIOp::migrate_in(Deme &ioDeme, Context &ioContext,bool &lChanged) {
  // Migrating individuals into this deme from same proc
  if(ioContext.getDemeIndex() != 0) {
    Individual::Bag& lInMigBuffer =
      ioContext.getVivarium()[ioContext.getDemeIndex()-1]->getMigrationBuffer();
    for(unsigned int i=0; i<lInMigBuffer.size(); ++i) {
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "migration", "Beagle::HFCompMPIOp",
        string("Individual migrated into the ")+uint2ordinal(ioContext.getDemeIndex())+
        string(" deme: ")+lInMigBuffer[i]->serialize()
      );
      ioDeme.push_back(lInMigBuffer[i]);
      lChanged = true;
    }
    lInMigBuffer.resize(0);
  }
}

void HFCompMPIOp::migrate_out(Deme &ioDeme, Context &ioContext,bool &lChanged) {
  // Migrating individual out of this deme on same proc
  if((ioContext.getDemeIndex() != (mPopSize->size()-1)) &&
     (mFitnessThresholds[ioContext.getDemeIndex()+1] != NULL)) {
    Individual::Bag& lOutMigBuffer = ioDeme.getMigrationBuffer();
    lOutMigBuffer.resize(0);
    Fitness::Handle lThreshold = mFitnessThresholds[ioContext.getDemeIndex()+1];
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    while(ioDeme.size() > 0) {
      if(ioDeme[0]->getFitness()->isLess(*lThreshold)) break;
      std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
      lOutMigBuffer.push_back(castHandleT<Individual>(ioDeme.back()));
      ioDeme.pop_back();
      lChanged = true;
    }
  }
}

void HFCompMPIOp::fix_population_size(Deme &ioDeme, Context &ioContext,
                                      bool &lChanged) {
  // Fill the population with randomly generated individuals, if the population is too small.
  if(ioDeme.size() < (*mPopSize)[ioContext.getDemeIndex()]) {
    const unsigned int lNbNewInd = (*mPopSize)[ioContext.getDemeIndex()] - ioDeme.size();
    Individual::Bag lNewIndividuals = generateIndividuals(lNbNewInd, ioDeme, ioContext);
    ioDeme.insert(ioDeme.end(), lNewIndividuals.begin(), lNewIndividuals.end());
    lChanged = true;
  }

  // Delete worse individuals if the population is too big.
  if(ioDeme.size() > (*mPopSize)[ioContext.getDemeIndex()]) {
    const unsigned int lNbDeletedInd = ioDeme.size() - (*mPopSize)[ioContext.getDemeIndex()];
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
    for(unsigned int i=0; i<lNbDeletedInd; ++i) {
      std::pop_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "migration", "Beagle::HFCompMPIOp",
        string("Individual erased from the last deme: ")+
        ioDeme.back()->serialize()
      );
      ioDeme.pop_back();
    }
    lChanged = true;
  }

  // If changed, reset statistics.
  if(lChanged) {
    ioDeme.getStats()->setInvalid();
    ioContext.getVivarium().getStats()->setInvalid();
  }
}

#ifdef GPMPI
void HFCompMPIOp::get_inds(Deme &ioDeme, Context &ioContext,int *comm_counts) {
  // Receive individuals to add on next operation
  MPI_Status pstatus,status_in;
  if((ioContext.getDemeIndex() == (mPopSize->size()-1)) && comm_counts[my_rank]>0) {
    for (int i=0; i<comm_counts[my_rank]; i++) {
      int mess_size=0;
      MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&pstatus);
      MPI_Get_count(&pstatus, MPI_CHAR, &mess_size);
      if (mess_size<1)
        std::cerr << "WARNING: Unexpected MPI message of size 0 during migration "
                  << "on process " << my_rank << ". Skipping immigration.\n";
      else {
        char trees_in[mess_size];
        MPI_Recv(trees_in,mess_size,MPI_CHAR,pstatus.MPI_SOURCE,
                 pstatus.MPI_TAG,MPI_COMM_WORLD,&status_in);
        received_inds.push_back(std::string(trees_in,mess_size));
      }
    }
  }
}
#endif
  

void HFCompMPIOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  bool do_migrate_out=true;

  // Situations where HFC migration should not be applied.
  if(ioDeme.size() == 0) return;
  if(mPopSize->size() < 2 && num_islands/mHFCCopyNum->getWrappedValue()<2) 
    return;
  if(mMigrationInterval->getWrappedValue() == 0) return;
  int modgv=ioContext.getGeneration() % mMigrationInterval->getWrappedValue();
  if(modgv != 0 && modgv != 1) return;
  if (modgv==1)
    do_migrate_out=false;

  if (old_popsize!=mPopSize->size()) {
    initialize_thresholds(ioDeme,ioContext);
    old_popsize=mPopSize->size();
  }
    
  // Log some information.
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "migration", "Beagle::HFCompMPIOp",
    string("Applying HFC migration to the ")+uint2ordinal(ioContext.getDemeIndex())+
    string(" deme")
  );

  // If the deme is changed.
  bool lChanged = false;
  
  if (adaptive && do_migrate_out)
    update_fitnesses(ioDeme,ioContext);

  // Add individuals from receive on previous operation
  #ifdef GPMPI
  if (ioContext.getDemeIndex()==0 && received_inds.size()>0) {
    if (!inds_from_string(ioDeme,ioContext,lChanged))
      std::cerr << "WARNING: MPI message could not be read as individual "
                << "for migration "
                << "on process " << my_rank << ". Skipping immigration.\n";
    received_inds.clear();
  }
  #endif

  migrate_in(ioDeme,ioContext,lChanged);
  
  // Migrating individual out of this deme onto different proc
  #ifdef GPMPI
  MPI_Status status_out,status_in;
  std::vector<MPI_Request> sendr;
  int comm_counts[num_islands];
  string message[num_islands];
  #endif
  if (do_migrate_out) {
    #ifdef GPMPI
    if (ioContext.getDemeIndex() == (mPopSize->size()-1)) {
      int my_comm_counts[num_islands];
      for (int i=0; i<num_islands; i++) my_comm_counts[i]=0;
      if (my_rank<num_islands-num_copies) {
        std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
        for (int ii=num_islands-1; ii>my_copy_rank; ii-=num_copies) {
          int i=ii;
          if (num_copies>1)
            i-=ioContext.getSystem().getRandomizer().rollInteger(0,num_copies-1);
          std::ostringstream out,totals;
          PACC::XML::Streamer ind_stream(out,0);
          int total=0;
          out << "\n<MPI>";
          while(ioDeme.size() > 0) {
            if(castHandleT<FitnessSimple>
               (ioDeme[0]->getFitness())->getValue()<ifitnesses[i]) break;
            std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
            ioDeme.back()->write(ind_stream);
            ioDeme.pop_back();
            lChanged = true;
            total++;
          }
          if (total>0) {
            out << "\n</MPI>\n";
            totals << total;
            message[i]=totals.str()+out.str();
            sendr.push_back(MPI_Request());
            // Send Individuals Out
            MPI_Isend((void *)message[i].c_str(),message[i].length(),MPI_CHAR,
                      i,my_rank,MPI_COMM_WORLD,&sendr.back());
            my_comm_counts[i]++;
          }
        }
      }
      MPI_Allreduce(my_comm_counts,comm_counts,num_islands,MPI_INT,MPI_SUM,
                    MPI_COMM_WORLD);
    }
    #endif
    
    migrate_out(ioDeme,ioContext,lChanged);
  }
  
  fix_population_size(ioDeme,ioContext,lChanged);
  
  #ifdef GPMPI
  if (do_migrate_out) {
    get_inds(ioDeme,ioContext,comm_counts);

    for (int i=0; i<sendr.size(); i++)  
      MPI_Wait(&sendr[i],&status_out);
  }
  #endif
  
  Beagle_StackTraceEndM("void HFCompMPIOp::operate(Deme& ioDeme, Context& ioContext)");
}

Individual::Bag
HFCompMPIOp::generateIndividuals(unsigned int inN,
                                 Deme& ioDeme, Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(getRootNode());
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::HFCompMPIOp",
    string("Processing using HFC replacement strategy the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eTrace,
    "replacement-strategy", "Beagle::HFCompMPIOp",
    (*this)
  );

  RouletteT<unsigned int> lRoulette;
  buildRoulette(lRoulette, ioContext);
  Individual::Bag lNewIndividuals;

  for(unsigned int i=0; i<inN; ++i) {
    unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
    BreederNode::Handle lSelectedBreeder=getRootNode();
    for(unsigned int j=0; j<lIndexBreeder; ++j)
      lSelectedBreeder=lSelectedBreeder->getNextSibling();
    Beagle_NonNullPointerAssertM(lSelectedBreeder);
    Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
    Individual::Handle lBredIndiv =
      lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
    lNewIndividuals.push_back(lBredIndiv);
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::HFCompMPIOp",
      string("Randomly generated individual to be inserted in first deme: ")+
      lBredIndiv->serialize()
    );
  }
  return lNewIndividuals;
  Beagle_StackTraceEndM("Individual::Bag HFCompMPIOp::generateIndividuals(unsigned int inN, Deme& ioDeme, Context& ioContext) const");
}
