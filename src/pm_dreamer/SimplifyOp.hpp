/***************************************************************************
                                SimplifyOp.hpp
                             -------------------
                               W. Michael Brown

  Shrink subtrees that evaluate to a constant or a variable

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Oct 27 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef SimplifyOp_hpp
#define SimplifyOp_hpp

#include <string>

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
#include "base_potential.h"


namespace Beagle {
namespace GP {

class SimplifyOp : public Beagle::MutationOp {
 public:
  typedef AllocatorT<SimplifyOp,Beagle::MutationOp::Alloc>
          Alloc;
  typedef PointerT<SimplifyOp,Beagle::MutationOp::Handle>
          Handle;
  typedef ContainerT<SimplifyOp,Beagle::MutationOp::Bag>
          Bag;

  explicit SimplifyOp(EvaluationOp::Handle fitguy, Error *error,
                      std::string inMutationPbName="gp.simplify.indpb",
                      std::string inName="GP-SimplifyOp");
  virtual ~SimplifyOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual void postInit(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
 protected:
  bool simplify(GP::Individual&, GP::Context&,int start_node, int end_node, 
                Double_matrix &, primitive_vector &); 
  void simplify_subtree(GP::Tree& lTree, GP::Context& ioContext,
                        int start_node, Double_matrix input, double_array &minv, 
                        double_array &maxv, double_matrix &var_diff);
 
  Error *error;

  String::Handle mEphemeralName;  //!< Name of ephemeral primitive for replace.
  EvaluationOp::Handle fit_guy;
  Double::Handle threshold;       // Threshold for invariance
  UInt::Handle num_evals;         // Maximum number of evaluations for simplification
  UInt::Handle interval;          // Interval in generations for simplification
 
  Double_matrix two_body_input,three_body_input;
  primitive_vector two_body_vars, three_body_vars;
  GP::Primitive::Handle ePrim;
  bool setup_ok, init_complete, post_complete;
};

}
}

#endif
