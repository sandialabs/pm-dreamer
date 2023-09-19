/***************************************************************************
                               OLSCorrectOp.hpp
                             -------------------
                               W. Michael Brown

  Ordinary least squares optimization of expressions

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Nov 14 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef OLSCorrectOp_hpp
#define OLSCorrectOp_hpp

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

template <class DoubTp>
class OLSCorrectOp : public Beagle::MutationOp {
 public:
  typedef AllocatorT<OLSCorrectOp,Beagle::MutationOp::Alloc>
          Alloc;
  typedef PointerT<OLSCorrectOp,Beagle::MutationOp::Handle>
          Handle;
  typedef ContainerT<OLSCorrectOp,Beagle::MutationOp::Bag>
          Bag;

  explicit OLSCorrectOp(EvaluationOp::Handle fitguy, Error *error, bool three_b,
                        std::string inMutationPbName="gp.olscorrect.indpb",
                        std::string inName="GP-OLSCorrectOp");
  virtual ~OLSCorrectOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual void postInit(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

  void correct(GP::Individual& lIndividual, GP::Context& lContext,
               int start_node, GP::Primitive::Handle &prim, double constant);
 protected:
  Error *error;

  String::Handle mEphemeralName;  //!< Name of ephemeral primitive for replace.
  EvaluationOp::Handle fit_guy;
  UInt::Handle interval;          // Interval in generations for simplification
  GP::Primitive::Handle ePrim, mulPrim, addPrim;
  bool three_body, setup_ok;
};

}
}

#endif
