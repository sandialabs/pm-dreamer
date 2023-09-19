/***************************************************************************
                         StatsCalcFitSimpleMPIOp.hpp
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

#ifndef StatsCalcFitSimpleMPIOp_hpp
#define StatsCalcFitSimpleMPIOp_hpp

#include "beagle/GP/StatsCalcFitnessSimpleOp.hpp"

namespace Beagle {
namespace GP {

class StatsCalcFitSimpleMPIOp : public StatsCalcFitnessSimpleOp {

 public:
  typedef AllocatorT<StatsCalcFitSimpleMPIOp,StatsCalcFitSimpleMPIOp::Alloc>
          Alloc;
  typedef PointerT<StatsCalcFitSimpleMPIOp,StatsCalcFitSimpleMPIOp::Handle>
          Handle;
  typedef ContainerT<StatsCalcFitSimpleMPIOp,StatsCalcFitSimpleMPIOp::Bag>
          Bag;

  explicit StatsCalcFitSimpleMPIOp(bool three_body,
                                   string inName="GP-StatsCalcFitSimpleMPIOp");
  virtual ~StatsCalcFitSimpleMPIOp() { }

  virtual void calculateStatsVivarium(Stats& outStats,
                                      Beagle::Vivarium& ioVivarium,
                                      Beagle::Context& ioContext) const;

 protected:
  std::map<std::string,std::string> binary_map;  
  int my_rank,num_islands;  
  bool three_body;
  
  void get_hof(int send_rank, Beagle::Vivarium& ioVivarium,
               Beagle::Context& ioContext) const;
  int pretty_string(Tree& lTree, int start_node, std::string &out,
                    bool bracket, std::vector<std::string> *types=NULL,
                    const char ascii_start=96) const;
};

}
}

#endif
