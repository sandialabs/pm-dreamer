/***************************************************************************
                               OLSCorrectOp.cpp
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

#include "base_potential.h"
#include "beagle/GP.hpp"
#include "EphemeralD.hpp"
#include "OLSCorrectOp.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;

template <class DoubTp>
GP::OLSCorrectOp<DoubTp>::OLSCorrectOp(EvaluationOp::Handle fitguy, Error *error,
                                       bool three_b, std::string inMutationPbName,
                                       std::string inName) :
  Beagle::MutationOp(inMutationPbName, inName), fit_guy(fitguy), error(error),
  three_body(three_b), setup_ok(false)
{ }

template <class DoubTp>
void GP::OLSCorrectOp<DoubTp>::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  
  fit_guy->initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(1.0);
    std::ostringstream lOSS;
    lOSS << "Simplify mutation probability for an individual. ";
    lOSS << "Simplify mutation consists in replacing a invariant branch with ";
    lOSS << "the constant it evaluates to.";
    Register::Description lDescription(
      "Individual simplify mutation prob.",
      "Float",
      "1.0",
      lOSS.str()
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lDescription);
  }
  
  // Primitive type to optimize
  if(ioSystem.getRegister().isRegistered("gp.olscorrect.name")) {
    mEphemeralName = castHandleT<String>(ioSystem.getRegister()["gp.olscorrect.name"]);
  } else {
    mEphemeralName = new String("E");
    Register::Description lDescription(
      "Ephemeral primitive name",
      "String",
      "E",
      string("Name of the ephemeral primitive to be used to replace branches ")+
      string("during correction.")
    );
    ioSystem.getRegister().addEntry("gp.olscorrect.name", mEphemeralName, lDescription);
  }

  // Max evaluations
  if(ioSystem.getRegister().isRegistered("gp.olscorrect.interval")) {
    interval = castHandleT<UInt>(ioSystem.getRegister()["gp.olscorrect.interval"]);
  } else {
    interval = new UInt(20);
    string lLongDescrip = "Interval in generations between OLS correction.";
    Register::Description lProbaDescription(
      "Interval in generations between OLS correction.",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.olscorrect.interval", interval, lProbaDescription);
  }

  Beagle_StackTraceEndM("void GP::OLSCorrectOp::registerParams(Beagle::System&)");
}

template <class DoubTp>
void GP::OLSCorrectOp<DoubTp>::postInit(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  
  fit_guy->postInit(ioSystem);
  
  BasePotentialOp::Handle fit_potential=castHandleT<BasePotentialOp>(fit_guy);

  GP::System& lSystem = castObjectT<GP::System&>(ioSystem);
  GP::PrimitiveSuperSet& lSuperSet = lSystem.getPrimitiveSuperSet();
  ePrim=lSuperSet[0]->getPrimitiveByName(mEphemeralName->getWrappedValue());
  mulPrim=lSuperSet[0]->getPrimitiveByName("MUL");
  addPrim=lSuperSet[0]->getPrimitiveByName("ADD");
  setup_ok=true;
  if (ePrim == NULL) {
    error->addwarning(204,9,"OLSCorrectOp",
     "OLSCorrectOp will be ignored (no primitive). Please set gp.olscorrect.name.");
    *interval = 0;
    setup_ok=false;
  }
  if (mulPrim == NULL || addPrim == NULL) {
    error->addwarning(204,9,"OLSCorrectOp",
     "OLSCorrectOp will be ignored (no ADD or MUL primitive).");
    *interval = 0;
    setup_ok=false;
  }
  
  Beagle_StackTraceEndM("void GP::OLSCorrectOp::postInit(Beagle::System&)");
}

template <class DoubTp>
bool GP::OLSCorrectOp<DoubTp>::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();

  if (interval->getWrappedValue() == 0 || setup_ok==false) 
    return false;
  if ( (ioContext.getGeneration() % interval->getWrappedValue()) != 0) 
    return false;

  GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
  GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);

  GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
  unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
  ioContext.setGenotypeHandle(ioIndividual[0]);
  ioContext.setGenotypeIndex(0);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::OLSCorrectOp",
    std::string("Individual before OLS correct mutation: ")+
    lIndividual.serialize()
  );

  bool changed=false;
  double a, b, c, d;
  if (three_body)
    castHandleT<BasePotentialOp>(fit_guy)->ols_correct(lIndividual, lContext, 
                                                       a, b, c, d);
  else
    castHandleT<BasePotentialOp>(fit_guy)->ols_correct(lIndividual, lContext, a, b);

  if (a>(1+1e-32) || a<(1-1e-32)) {
    changed=true;
    if (three_body) {
      correct(lIndividual,lContext,1,mulPrim,a);
      (*lIndividual[0])[0].mSubTreeSize+=2;
    } else
      correct(lIndividual,lContext,0,mulPrim,a);
  }    
  if (b>(0+1e-32) || b<(0-1e-32)) {
    changed=true;
    if (three_body) {
      correct(lIndividual,lContext,1,addPrim,b);
      (*lIndividual[0])[0].mSubTreeSize+=2;
    } else
      correct(lIndividual,lContext,0,addPrim,b);
  }
  if (three_body) {
    unsigned right_tree=(*lIndividual[0])[1].mSubTreeSize+1;
    if (right_tree<lIndividual[0]->size()) {
      if (c>(1+1e-32) || c<(1-1e-32)) {
        changed=true;
        correct(lIndividual,lContext,right_tree,mulPrim,c);
        (*lIndividual[0])[0].mSubTreeSize+=2;
      }    
      if (d>(0+1e-32) || d<(0-1e-32)) {
        changed=true;
        correct(lIndividual,lContext,right_tree,addPrim,d);
        (*lIndividual[0])[0].mSubTreeSize+=2;
      }
    }
  }    
  
  lContext.setGenotypeHandle(lOldTreeHandle);
  lContext.setGenotypeIndex(lOldTreeIndex);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::OLSCorrectOp",
    std::string("Individual after GP simplify mutation: ")+
    ioIndividual.serialize()
  );

  return changed;
  Beagle_StackTraceEndM("bool GP::OLSCorrectOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}

template <class DoubTp>
void GP::OLSCorrectOp<DoubTp>::correct(GP::Individual& lIndividual, 
                                       GP::Context& lContext, int start_node, 
                                       GP::Primitive::Handle &prim,
                                       double constant) {
  Beagle_StackTraceBeginM();
  GP::Tree::Handle lActualTree = lIndividual[0];
  GP::Tree& lTree = *lActualTree;
  int tree_size=(*lIndividual[0])[start_node].mSubTreeSize;
  lIndividual[0]->insert(lIndividual[0]->begin()+start_node,
                         Node(castHandleT<EphemeralT<DoubTp> >(ePrim)->
                                 generate(ePrim->getName(),lContext),1));
  (*lIndividual[0])[start_node].mPrimitive->setValue(DoubTp(constant));
  lIndividual[0]->insert(lIndividual[0]->begin()+start_node, 
                         GP::Node(prim,tree_size+2));

  Beagle_StackTraceEndM("bool OLSCorrectOp::correct(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}

template class OLSCorrectOp<Double>;
template class OLSCorrectOp<VDouble>;

