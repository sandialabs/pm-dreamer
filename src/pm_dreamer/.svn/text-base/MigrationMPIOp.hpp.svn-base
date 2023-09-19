/***************************************************************************
                             MigrationMPIOp.hpp
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

#ifndef MigrationMPIOp_hpp
#define MigrationMPIOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"
#include "beagle/UInt.hpp"


namespace Beagle {

class MigrationMPIOp : public Operator {

public:
  typedef AbstractAllocT<MigrationOp,Operator::Alloc>
          Alloc;
  typedef PointerT<MigrationOp,Operator::Handle>
          Handle;
  typedef ContainerT<MigrationOp,Operator::Bag>
          Bag;

  explicit MigrationMPIOp(string inName="MigrationMPIOp");
  virtual ~MigrationMPIOp() { }

  virtual void initialize(System& ioSystem);
  /// Perform migration between Deme on this proc to random Deme on another
  virtual void operate(Deme& ioDeme, Context& ioContext);
  // Perform a migration
  void migrate(Deme&, Context&, unsigned int);
  
protected:
  // Decide the migration graph - called for each operate
  virtual void random_comm_graph(int *comm_ind, Context& ioContext,
                                 const int start, const int end);
  // Read in a set of individuals from a XML string
  bool inds_from_string(Deme&,Context &,const unsigned *ind,
                        const char* xstring,const unsigned xstring_size,
                        const unsigned number);

  int num_islands, my_rank;
  UInt::Handle      mMigrationInterval;  //!< Random ring migration interval (gens)
  UInt::Handle      mNumberMigrants;     //!< Number of migrants
  UInt::Handle      mMigrationSplit;     //!< Number of groups of islands for migration
  UIntArray::Handle mPopSize;            //!< Population size for the evolution.
};

}

#endif
