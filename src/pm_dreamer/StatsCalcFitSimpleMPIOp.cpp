/***************************************************************************
                         StatsCalcFitSimpleMPIOp.cpp
                             -------------------
                               W. Michael Brown

  Update statistics for the vivarium based on statistics on all procs

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : July 12 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#include "beagle/GP.hpp"
#include "StatsCalcFitSimpleMPIOp.hpp"

#include <cmath>
#include <sstream>
#ifdef GPMPI
#include "mpi.h"
#endif

using namespace Beagle;


GP::StatsCalcFitSimpleMPIOp::StatsCalcFitSimpleMPIOp(bool tin,
                                                     Beagle::string inName) :
  StatsCalcFitnessSimpleOp(inName), my_rank(0), num_islands(1), three_body(tin)
{ 
  #ifdef GPMPI
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_islands);
  #endif
  binary_map["ADD"]="+";
  binary_map["SUB"]="-";
  binary_map["MUL"]="*";
  binary_map["DIV"]="/";
  binary_map["POW"]="^";
  binary_map["ABS"]="abs";
  binary_map["SIN"]="sin";
  binary_map["COS"]="cos";
  binary_map["EXP"]="exp";
  binary_map["LOG"]="log";
  binary_map["Pi"]="pi";
}

void GP::StatsCalcFitSimpleMPIOp::calculateStatsVivarium(Stats& outStats,
                                                         Beagle::Vivarium& ioVivarium,
                                                         Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  GP::StatsCalcFitnessSimpleOp::calculateStatsVivarium(outStats,ioVivarium,
                                                       ioContext);

  struct {
    float value;
    int i;
  } hof_in, hof_out;
  FitnessSimple::Handle fitp = castHandleT<FitnessSimple>
    (ioContext.getVivarium().getHallOfFame()[0].mIndividual->getFitness());
  hof_out.value=fitp->getValue();
  hof_out.i=my_rank;

  #ifdef GPMPI
  if (num_islands>1) {
    // Items processed and popsize
    unsigned lProcessed[3],tProcessed[3];
    lProcessed[0]=static_cast<unsigned>(outStats.getItem("processed"));
    lProcessed[1]=static_cast<unsigned>(outStats.getItem("total-processed"));
    lProcessed[2]=outStats.getPopSize();
    MPI_Reduce(lProcessed,tProcessed,3,MPI_UNSIGNED,MPI_SUM,0,MPI_COMM_WORLD);
    if (my_rank==0) {
      outStats.setGenerationValues("vivarium", ioContext.getGeneration(),tProcessed[2], true);
      outStats.modifyItem("processed",tProcessed[0]);
      outStats.modifyItem("total-processed",tProcessed[1]);
    }

    if(ioVivarium.size() == 0) return;

    int osize=outStats.size();
    int pack_size=2*osize;
    double dstats[pack_size];
    double ostats[pack_size];

    // Average and std
    for(unsigned int i=0; i<osize; ++i) {
      dstats[2*i]=outStats[i].mAvg*static_cast<double>(lProcessed[2]); // Sum of fitnesses
      dstats[2*i+1]=pow2Of<double>(dstats[2*i])/static_cast<double>(lProcessed[2]);
      dstats[2*i+1]+=pow2Of<double>(outStats[i].mStd)*static_cast<double>(lProcessed[2]-1);
    }
    MPI_Reduce(dstats,ostats,pack_size,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (my_rank==0) {
      for (int i=0; i<osize; ++i) {
        outStats[i].mAvg=ostats[2*i]/static_cast<double>(tProcessed[2]);
        double lVariance=ostats[2*i+1]-(pow2Of<double>(ostats[2*i])/
                         static_cast<double>(tProcessed[2]));
        outStats[i].mStd=sqrt(lVariance/static_cast<double>(tProcessed[2]-1));
      }
    }

    // Min and max
    for(unsigned int i=0; i<osize; ++i) {
      dstats[2*i]=outStats[i].mMax;
      dstats[2*i+1]=-outStats[i].mMin;
    }
    MPI_Reduce(dstats,ostats,pack_size,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    if (my_rank==0)
      for(unsigned int i=0; i<osize; ++i) {
        outStats[i].mMax=ostats[2*i];
        outStats[i].mMin=-ostats[2*i+1];
      }
      
    // Hall-of-Fame Maximum
    MPI_Allreduce(&hof_out,&hof_in,1,MPI_FLOAT_INT,MPI_MAXLOC,MPI_COMM_WORLD);
    if (hof_in.i!=0) {
      hof_out.value=hof_in.value;
      get_hof(hof_in.i,ioVivarium,ioContext);
    }
  }
  #endif

  if (my_rank==0) {
    std::cout << " Current Best Fit: " << outStats[0].mMax << " | "
              << " HOF Best Fit: " << hof_out.value << std::endl;
    GP::Individual& lIndividual  = castObjectT<GP::Individual&>
      (*ioContext.getVivarium().getHallOfFame()[0].mIndividual);

    if (three_body) {
      std::string left,right;
      if (lIndividual[0]->size()>1) {
        int nt=pretty_string(*lIndividual[0],1,left,false);
        if ((*lIndividual[0])[0].mPrimitive->getNumberArguments()>1)
          pretty_string(*lIndividual[0],(*lIndividual[0])[1].mSubTreeSize+1,
                        right,false,NULL,char(96+nt));
      }
      std::cout << "  2-Body: " << left << std::endl;  
      std::cout << "  3-Body: " << right << std::endl;  
    } else {  
      std::string hof;
      pretty_string(*lIndividual[0],0,hof,false);
      std::cout << "  2-Body: " << hof << std::endl;  
    }
  }
              
  Beagle_StackTraceEndM("void StatsCalcFitSimpleMPIOp::calculateStatsVivarium(Stats& outStats, Vivarium& ioVivarium, Context& ioContext) const");
}

void GP::StatsCalcFitSimpleMPIOp::get_hof(int send_rank, 
                                          Beagle::Vivarium& ioVivarium,
                                          Beagle::Context& ioContext) const {
  #ifdef GPMPI
  if (my_rank==send_rank) {                                            
    std::ostringstream out;
    PACC::XML::Streamer ind_stream(out,0);
    ioVivarium.getHallOfFame()[0].mIndividual->write(ind_stream);
    MPI_Send((void *)out.str().c_str(),out.str().length(),MPI_CHAR,
              0,0,MPI_COMM_WORLD);
  } else if (my_rank==0) {
    MPI_Status status_in;
    int mess_size;
    MPI_Probe(send_rank, 0, MPI_COMM_WORLD, &status_in);
    MPI_Get_count(&status_in, MPI_CHAR, &mess_size);
    char trees_in[mess_size];
    MPI_Recv(trees_in,mess_size,MPI_CHAR,send_rank,0,MPI_COMM_WORLD,
             &status_in);

    std::istringstream in;
    in.str(std::string(trees_in,mess_size));
    PACC::XML::Document lParser(in);
    PACC::XML::ConstFinder lIndivFinder = 
                           (PACC::XML::ConstIterator)lParser.getFirstDataTag();
    PACC::XML::ConstIterator lIndivTag = lIndivFinder.find("//Individual");

    Individual::Handle lNewIndiv =
      castHandleT<Individual>(ioContext.getDeme().getTypeAlloc()->allocate());
    ioContext.setIndividualHandle(lNewIndiv);
    ioContext.setIndividualIndex(0);
    lNewIndiv->readWithContext(lIndivTag,ioContext);
    ioVivarium.getHallOfFame()[0].mIndividual=lNewIndiv;
  }
  #endif
}

int GP::StatsCalcFitSimpleMPIOp::pretty_string(GP::Tree& lTree, int start_node,
    std::string &out, bool bracket, std::vector<std::string> *types,
    const char ascii_start) const {
  
  bool delete_types=false;
  if (types==NULL) {
    delete_types=true;
    types=new std::vector<std::string>();
  }
  int j=start_node;
  unsigned args=lTree[j].mPrimitive->getNumberArguments();

  if (args==0) {
    out=lTree[j].mPrimitive->getName();
    if (out[0]=='E') {
      std::ostringstream nout;
      PACC::XML::Streamer ind_stream(nout,0);
      lTree[j].mPrimitive->writeContent(ind_stream,false);
      int startq,endq;
      startq=nout.str().find('\"');
      endq=nout.str().find_last_of('\"');
      if (startq>=nout.str().length() || endq>=nout.str().length() ||
          startq==endq)
        out="ERROR";
      else
        out=nout.str().substr(startq+1,endq-startq-1);
      if (out.find(' ')<out.length()) {
        types->push_back(out);
        out=ascii_start+types->size();
      }
    }
  } else {
    bool mapped=false;
    std::string aname=lTree[j].mPrimitive->getName();
    std::map<std::string,std::string>::const_iterator bin_i;
    bin_i=binary_map.find(aname);
    if (bin_i!=binary_map.end()) {
      mapped=true;
      aname=bin_i->second;
    }
    if (args==1) {
      std::string unary;
      pretty_string(lTree,j+1,unary,true,types,ascii_start);
      out=aname+"("+unary+")";
    } else {
      std::string left, right;
      pretty_string(lTree,j+1,left,true,types,ascii_start);
      pretty_string(lTree,j+lTree[j+1].mSubTreeSize+1,right,true,types,
                    ascii_start);
      if (mapped==false)
        out=aname+"("+left+","+right+")";
      else {
        if (bracket)
          out="("+left+aname+right+")";
        else
          out=left+aname+right;
      }
    }      
  }
  
  if (delete_types) {
    int ntypes=types->size();
    if (types->size()>0) {
      for (int i=0; i<types->size(); i++) {
        char letter=ascii_start+i+1;
        out+=string("\n          ")+letter+string("=[")+types->at(i)+
             string("]");
      }
    }
    delete types;
    return ntypes;
  } else
    return 0;
}
