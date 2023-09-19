/***************************************************************************
                               HFCompMPIOp.hpp
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


#ifndef HFCOMPMPIOP_hpp
#define HFCOMPMPIOP_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"

namespace Beagle {

class HFCompMPIOp : public ReplacementStrategyOp {

public:
  typedef AllocatorT<HFCompMPIOp,ReplacementStrategyOp::Alloc>
          Alloc;
  typedef PointerT<HFCompMPIOp,ReplacementStrategyOp::Handle>
          Handle;
  typedef ContainerT<HFCompMPIOp,ReplacementStrategyOp::Bag>
          Bag;

  HFCompMPIOp(string inName="HFCompMPIOp");
  virtual ~HFCompMPIOp() { delete [] ifitnesses; }

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);

protected:
  int num_islands, my_rank;
  std::vector<std::string> received_inds;

  Individual::Bag generateIndividuals(unsigned int inN, Deme& ioDeme, Context& ioContext) const;

  // Initialize the fitness thresholds
  void initialize_thresholds(Deme &, Context &);
  // Update fitness thresholds
  void update_fitnesses(Deme &, Context &);
  // Add individuals to population from string
  bool inds_from_string(Deme&, Context &, bool &lChanged);
  // Migrate individuals in from another Deme on same proc
  void migrate_in(Deme &, Context &, bool &);
  // Migrate individuals out to another Deme on same proc
  void migrate_out(Deme &, Context &, bool &);
  // Add random individuals or delete unfit ones to attain correct size
  void fix_population_size(Deme &, Context &, bool &);
  // Receive any individuals to add from another proc
  #ifdef GPMPI
  void get_inds(Deme &, Context &,int *);
  #endif

  Fitness::Bag      mFitnessThresholds;  //!< Fitness thresholds used for migration.
  Float::Handle     mHFCPercentile;      //!< HFC percentile for fitness thresholds.
  Float::Handle     mFirstThreshold;     //!< First threshold for population
  Float::Handle     mScale;              //!< Scale parameter for fitness thresholds
  UInt::Handle      mMigrationInterval;  //!< Random ring migration interval (gens)
  UInt::Handle      mNumberMigrants;     //!< Number of migrants.
  UInt::Handle      mHFCCopyNum;         //!< Number of islands w/ same fitness thresh
  UIntArray::Handle mPopSize;            //!< Population size for the evolution.
  UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism
  float *ifitnesses;                   //! < Fitness thresholds for each proc
  bool adaptive;
  unsigned old_popsize;
  int my_copy_rank;
  int num_copies;
};

}

#endif
