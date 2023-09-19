/***************************************************************************
                               HybridOptOp.hpp
                             -------------------
                               W. Michael Brown

  Hybrid Optimization of Double Constants in Expression Tree

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun 20 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef HybridOptOp_hpp
#define HybridOptOp_hpp

#include <string>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/System.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/EphemeralT.hpp"
#include "VDouble.hpp"

namespace Beagle {
namespace GP {


/// Before hybrid optimization of double constants in expression tree
template <class DoubTp>
class HybridOptOp : public Beagle::MutationOp {
public:
  typedef AllocatorT<HybridOptOp<DoubTp>,Beagle::MutationOp::Alloc>
          Alloc;
  typedef PointerT<HybridOptOp<DoubTp>,Beagle::MutationOp::Handle>
          Handle;
  typedef ContainerT<HybridOptOp<DoubTp>,Beagle::MutationOp::Bag>
          Bag;

  explicit HybridOptOp(EvaluationOp::Handle fitguy,
                       MutationOp::Handle simplify_op,
                       string inMutationPbName="gp.hybopt.indpb",
                       string inEphemeralNameParamName="gp.hybopt.primit",
                       string inName="GP-HybridOptOp");
  virtual ~HybridOptOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual void postInit(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  inline EvaluationOp::Handle fitness_op() { return fit_guy; }

protected:
  bool optimize(Beagle::Individual& ioIndividual, Beagle::Context& ioContext,
                unsigned optimization_type, unsigned max_iters, 
                unsigned simplify_type);

  String::Handle mEphemeralName;           //!< Name of the ephemeral primitive mutated.
  string         mEphemeralNameParamName;  //!< Name of the ephemeral primitive name parameter.
  UInt::Handle opt_type; // Type of the optimization
  UInt::Handle maxi;     // Maximum number of iterations
  UInt::Handle simplify; // Simplify before optimization
  UInt::Handle s_inter;  // Simplification interval
  UInt::Handle mtypes;   // If true, optimize E_p and E_t in addition to name
  EvaluationOp::Handle fit_guy;
  MutationOp::Handle simple_op;
};

}

}

// Objective function for evaluation of an individual in C
extern "C" double gsl_objfun(const gsl_vector *x, void *params);
// Parameters for fitness evaluation in C
struct GSL_Params { Beagle::GP::Individual::Handle ind; 
                    Beagle::GP::Context::Handle ioContext;
                    Beagle::EvaluationOp::Handle fit_guy; 
                    std::vector< std::pair<unsigned int,unsigned int>,
                       BEAGLE_STLALLOCATOR< std::pair<unsigned int,
                                                      unsigned int> > > *nodes;
                    std::vector< std::pair<unsigned int,unsigned int>,
                       BEAGLE_STLALLOCATOR< std::pair<unsigned int,
                                                      unsigned int> > > *vnodes;
                    bool vector_double;
                    unsigned verbose;
};

template <class DoubTp> inline bool gsl_use_vect();
template <> inline bool gsl_use_vect<Beagle::Double>() { return false; }
template <> inline bool gsl_use_vect<Beagle::VDouble>() { return true; }
 
#endif
