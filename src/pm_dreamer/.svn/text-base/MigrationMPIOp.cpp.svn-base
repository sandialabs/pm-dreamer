/***************************************************************************
                             MigrationMPIOp.cpp
                             -------------------
                               W. Michael Brown

  Operator for individual migration between MPI procs

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun 11 2008
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
#include "MigrationMPIOp.hpp"

#ifdef GPMPI
#include "mpi.h"
#include <sstream>
#endif

using namespace Beagle;

MigrationMPIOp::MigrationMPIOp(Beagle::string inName) :
  Operator(inName),
  mMigrationInterval(NULL),
  mNumberMigrants(NULL),
  mPopSize(NULL),
  num_islands(1),
  my_rank(0)
{ 
  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_islands);
  #endif
}

void MigrationMPIOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.mig.mpi_interval")) {
    mMigrationInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.mig.mpi_interval"));
  } else {
    mMigrationInterval = new UInt(1);
    string lLongDescrip = "Interval between each MPI migration, in number of generations. ";
    lLongDescrip += "An interval of 0 disables migration.";
    Register::Description lIntervalDescrip(
      "Interval MPI between migrations",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.mig.mpi_interval", mMigrationInterval, lIntervalDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.mig.mpi_size")) {
    mNumberMigrants =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.mig.mpi_size"));
  } else {
    mNumberMigrants = new UInt(5);
    Register::Description lSizeDescrip(
      "Size of each MPI migration",
      "UInt",
      "5",
      "Number of individuals migrating between each MPI proc, at a each migration."
    );
    ioSystem.getRegister().addEntry("ec.mig.mpi_size", mNumberMigrants, lSizeDescrip);
  }
  
  if(ioSystem.getRegister().isRegistered("ec.mig.mpi_split")) {
    mMigrationSplit =
        castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.mig.mpi_split"));
  } else {
    mMigrationSplit = new UInt(0);
    string lLongDescrip = "Size of independent groups used for migration. ";
    lLongDescrip+="When 0, migration can occur between all islands. When ";
    lLongDescrip+="non-zero, migration only occurs between islands in groups ";
    lLongDescrip+="of the specified size (with sequential ranks in the same ";
    lLongDescrip+="group. ";
    Register::Description lSplitDescrip(
        "Size of independent groups of islands used for migration.",
        "UInt",
        "0",
        lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.mig.mpi_split", mMigrationSplit,
                                    lSplitDescrip);
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
  Beagle_StackTraceEndM("void MigrationMPIOp::initialize(System& ioSystem)");
}

/// Perform migration
void MigrationMPIOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mMigrationInterval==NULL) 
    throw Beagle_RunTimeExceptionM(string("The MPI migration interval of the migration operator ")+
                                   string("has not been initialized. It is most likely that ")+
                                   string("you need to call the migration operator's ")+
                                   string("initialize() method."));
#endif // BEAGLE_NDEBUG
  unsigned int lMigrationInterval = mMigrationInterval->getWrappedValue();
  if((lMigrationInterval == 0) || (ioContext.getGeneration() == 0)) return;
  if((ioContext.getGeneration() % lMigrationInterval) != 0) return;
  if (mMigrationSplit->getWrappedValue() == 1) return;
  if(num_islands < 2) {
    Beagle_LogBasicM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationMPIOp",
      string("Warning: Migration between islands (processes) can't occur because there is only 1 island. This")+
      string(" can occur if the code is not compiled with MPI or only a single process is run.")
    );
    return;
  }
  if (mMigrationSplit->getWrappedValue()>1)
    if(num_islands%mMigrationSplit->getWrappedValue() == 1) {
      Beagle_LogBasicM(
          ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationMPIOp",
      string("Warning: Migration between islands (processes) can't occur because ec.mig.mpi_split is set such that a group contains only 1 island.")
                    );
      return;
    }
  unsigned int lNbMigrants = minOf<unsigned int>(mNumberMigrants->getWrappedValue(),ioDeme.size());
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "migration", "Beagle::MigrationMPIOp",
    string("Migrating ")+uint2str(lNbMigrants)+
    string(" individuals from the ")+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  migrate(ioDeme, ioContext, lNbMigrants);
  Beagle_StackTraceEndM("void MigrationMPIOp::operate(Deme& ioDeme, Context& ioContext)");
}

// Generate a random permutation of processors such that no processor
// is migrating with itself
// -- Migration will only occur for islands inclusive (start,start+N)
void MigrationMPIOp::random_comm_graph(int *comm_ind, Context& ioContext,
                                       const int start, const int N) {
  Beagle_StackTraceBeginM();
  // Start with ring topology
  int pend=start+N;
  int end=pend-1;
  for (int i=start; i<pend; i++)
    comm_ind[i]=i+1;
  comm_ind[end]=start;

  // Random permutation
  // -- Store the destination of outgoing migrants in comm_ind[0..num_islands-1]
  if (N>2) {
    int lN=N*2;
    for (int i=0; i<lN; i++) {
      int one=ioContext.getSystem().getRandomizer().rollInteger(start,end);
      int two=ioContext.getSystem().getRandomizer().rollInteger(start,end);
      if (comm_ind[one]!=two && comm_ind[two]!=one) {
        int t=comm_ind[one];
        comm_ind[one]=comm_ind[two];
        comm_ind[two]=t;
      }
    }
  }

  // -- Store the destination of incoming migrants in comm_ind[num_islands...]
  for (int i=start; i<pend; i++)
    comm_ind[comm_ind[i]+num_islands]=i;
  Beagle_StackTraceEndM("void MigrationMPIOp::random_comm_graph");
}

bool MigrationMPIOp::inds_from_string(Deme &ioDeme, Context &ioContext,
                                      const unsigned *ind, const char* xstring,
                                      const unsigned xstring_size,
                                      const unsigned number) {
  std::istringstream in;
  in.str(std::string(xstring,xstring_size));
  PACC::XML::Document lParser(in);
  PACC::XML::ConstFinder lIndivFinder = 
                           (PACC::XML::ConstIterator)lParser.getFirstDataTag();

  PACC::XML::ConstIterator lIndivTag = lIndivFinder.find("//Individual");
  for (unsigned i=0; i<number; i++) {
    if(!lIndivTag) return false;
    ioContext.setIndividualHandle(ioDeme[ind[i]]);
    ioContext.setIndividualIndex(0);
    ioDeme[ind[i]]->readWithContext(lIndivTag, ioContext);
    lIndivTag=lIndivFinder.findNext();
  }
  return true;
}  

void MigrationMPIOp::migrate(Deme& ioDeme, Context& ioContext, 
                             unsigned int inNumberMigrants)
{
  Beagle_StackTraceBeginM();
  
  #ifdef GPMPI
  MPI_Status status_out,status_in;
  
  // Decide what procs to migrate individuals to
  int comm_ind[num_islands*2];
  if (my_rank==0) {
    int split=mMigrationSplit->getWrappedValue();
    if (split==0)
      random_comm_graph(comm_ind,ioContext,0,num_islands);
    else {
      for (int i=0; i<num_islands; i+=split) {
        int mN=split;
        if (i+split>num_islands)
          mN=num_islands-i;
        random_comm_graph(comm_ind,ioContext,i,mN);
      }
    }
  }
  MPI_Bcast(comm_ind,num_islands*2,MPI_INT,0,MPI_COMM_WORLD);

  unsigned int lNbMigrants = minOf<unsigned int>(mNumberMigrants->getWrappedValue(),ioDeme.size());
  string outs="<MPI>\n";
  unsigned mig_ind[lNbMigrants];
  for (int i=0; i<lNbMigrants; i++) {
    std::ostringstream out;
    PACC::XML::Streamer ind_stream(out,0);
    // Migrate out
    mig_ind[i] =
      ioContext.getSystem().getRandomizer().rollInteger(0,ioDeme.size()-1);
    ioDeme[mig_ind[i]]->write(ind_stream);
    outs+=out.str();
  }
  outs+="</MPI>\n";
  
  // Send Individuals Out
  MPI_Request sendr;
  MPI_Isend((void *)outs.c_str(),outs.length(),MPI_CHAR,
            comm_ind[my_rank],my_rank,MPI_COMM_WORLD,&sendr);
  
  // Get incoming individuals
  int mess_size;
  MPI_Probe(comm_ind[my_rank+num_islands], comm_ind[my_rank+num_islands], 
            MPI_COMM_WORLD, &status_in);
  MPI_Get_count(&status_in, MPI_CHAR, &mess_size);
  if (mess_size<1)
    std::cerr << "WARNING: Unexpected MPI message of size 0 during migration "
              << "on process " << my_rank << ". Skipping immigration.\n";
  else {
    char trees_in[mess_size];
    MPI_Recv(trees_in,mess_size,MPI_CHAR,comm_ind[my_rank+num_islands],
             comm_ind[my_rank+num_islands],MPI_COMM_WORLD,&status_in);
    if (!inds_from_string(ioDeme,ioContext,mig_ind,trees_in,mess_size,
                          lNbMigrants))
      std::cerr << "WARNING: MPI message could not be read as individual "
                << "for migration "
                << "on process " << my_rank << ". Skipping immigration.\n";
  }

  MPI_Wait(&sendr,&status_out);
  #endif
  
  Beagle_StackTraceEndM("void MigrationMPIOp::migrate(Deme& ioDeme, Context& ioContext, unsigned int inNumberMigrants)");
}
