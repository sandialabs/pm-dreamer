/***************************************************************************
                                SimplifyOp.cpp
                             -------------------
                               W. Michael Brown

  Shrink subtrees that evaluate to a constant

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

#include "base_potential.h"
#include "beagle/GP.hpp"
#include "EphemeralD.hpp"
#include "SimplifyOp.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;

GP::SimplifyOp::SimplifyOp(EvaluationOp::Handle fitguy, Error *error,
                           std::string inMutationPbName,
                           std::string inName) :
  Beagle::MutationOp(inMutationPbName, inName), fit_guy(fitguy), error(error),
  setup_ok(false), init_complete(false), post_complete(false)
{ }

void GP::SimplifyOp::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if (init_complete)
    return;
  init_complete=true;

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
  if(ioSystem.getRegister().isRegistered("gp.simplify.name")) {
    mEphemeralName = castHandleT<String>(ioSystem.getRegister()["gp.simplify.name"]);
  } else {
    mEphemeralName = new String("E");
    Register::Description lDescription(
      "Ephemeral primitive name",
      "String",
      "E",
      string("Name of the ephemeral primitive for to be used to replace branches ")+
      string("during simplification.")
    );
    ioSystem.getRegister().addEntry("gp.simplify.name", mEphemeralName, lDescription);
  }

  // Invariance threshold
  if(ioSystem.getRegister().isRegistered("gp.simplify.eps")) {
    threshold = castHandleT<Double>(ioSystem.getRegister()["gp.simplify.eps"]);
  } else {
    threshold = new Double(1e-30);
    string lLongDescrip = "Threshold for simplification invariance. ";
    Register::Description lProbaDescription(
      "Threshold for simplification invariance",
      "Double",
      "1e-20",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.simplify.eps", threshold, lProbaDescription);
  }

  // Max evaluations
  if(ioSystem.getRegister().isRegistered("gp.simplify.maxe")) {
    num_evals = castHandleT<UInt>(ioSystem.getRegister()["gp.simplify.maxe"]);
  } else {
    num_evals = new UInt(0);
    string lLongDescrip = "Maximum number of function evaluations for simplification ";
    lLongDescrip += "of a single tree.";
    Register::Description lProbaDescription(
      "Maximum number of function evaluations for simplification of single tree",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.simplify.maxe", num_evals, lProbaDescription);
  }

  // Max evaluations
  if(ioSystem.getRegister().isRegistered("gp.simplify.interval")) {
    interval = castHandleT<UInt>(ioSystem.getRegister()["gp.simplify.interval"]);
  } else {
    interval = new UInt(20);
    string lLongDescrip = "Interval in generations between simplification.";
    Register::Description lProbaDescription(
      "Interval in generations between simplification.",
      "UInt",
      "20",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.simplify.interval", interval, lProbaDescription);
  }

  Beagle_StackTraceEndM("void GP::SimplifyOp::registerParams(Beagle::System&)");
}

void GP::SimplifyOp::postInit(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if (post_complete)
    return;
  post_complete=true;

  BasePotentialOp::Handle fit_potential=castHandleT<BasePotentialOp>(fit_guy);
  fit_potential->pack_simplify(num_evals->getWrappedValue(),two_body_input,
                               two_body_vars,three_body_input,three_body_vars,
                               castObjectT<GP::System&>(ioSystem));
  setup_ok=true;
  
  if (two_body_input.empty() && three_body_input.empty()) {
    error->addwarning(204,9,"SimplifyOp",
                      "SimplifyOp will be ignored (no vectorization).");
    *interval = 0;
    setup_ok=false;
    return;
  }

  GP::System& lSystem = castObjectT<GP::System&>(ioSystem);
  GP::PrimitiveSuperSet& lSuperSet = lSystem.getPrimitiveSuperSet();
  ePrim=lSuperSet[0]->getPrimitiveByName(mEphemeralName->getWrappedValue());
  if (ePrim == NULL) {
    error->addwarning(204,9,"SimplifyOp",
     "SimplifyOp will be ignored (no primitive). Please set gp.simplify.name.");
    *interval = 0;
    setup_ok=false;
  }
  
  Beagle_StackTraceEndM("void GP::SimplifyOp::postInit(Beagle::System&)");
}

bool GP::SimplifyOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  
  if (interval->getWrappedValue() == 0 || setup_ok==false) 
    return false;
  if ( (ioContext.getGeneration() % interval->getWrappedValue()) != 0) 
    return false;
  if ( (ioContext.getGeneration()==0 && interval->getWrappedValue() != 1) )
    return false;
  
  GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
  GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);

  if ((*lIndividual[0]).size()<2)
    return false;
  
  GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
  unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
  ioContext.setGenotypeHandle(ioIndividual[0]);
  ioContext.setGenotypeIndex(0);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::SimplifyOp",
    std::string("Individual before GP simplify mutation: ")+
    lIndividual.serialize()
  );
  
  bool changed;
  if (three_body_vars.empty()) {
    changed=simplify(lIndividual,lContext,0,(*lIndividual[0]).size(),
                     two_body_input,two_body_vars);
  } else {
    castHandleT<BasePotentialOp>(fit_guy)->fix_dual_equation(lContext,
                                                             *lIndividual[0]);
    unsigned right_tree=(*lIndividual[0])[1].mSubTreeSize+1;
    bool changed1=simplify(lIndividual,lContext,1,right_tree,two_body_input,
                           two_body_vars);
    right_tree=(*lIndividual[0])[1].mSubTreeSize+1;
    bool changed2=simplify(lIndividual,lContext,right_tree,
                           lIndividual[0]->size(),three_body_input,
                           three_body_vars);
    changed=changed1 || changed2;
  }
  
  lContext.setGenotypeHandle(lOldTreeHandle);
  lContext.setGenotypeIndex(lOldTreeIndex);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::SimplifyOp",
    std::string("Individual after GP simplify mutation: ")+
    ioIndividual.serialize()
  );

  return changed;
  Beagle_StackTraceEndM("bool GP::SimplifyOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}

bool GP::SimplifyOp::simplify(GP::Individual& lIndividual, 
                              GP::Context& lContext,
                              int start_node, int end_node,
                              Double_matrix &input, primitive_vector &vars) {
  Beagle_StackTraceBeginM();
  if (end_node<=start_node)
    return false;
                                
  GP::Tree::Handle lActualTree = lIndividual[0];
  GP::Tree& lTree = *lActualTree;
                                
  int tree_size=lIndividual[0]->size();
  double_array min_values(std::numeric_limits<double>::max(),tree_size);
  double_array max_values(std::numeric_limits<double>::max()*-1.0,tree_size);
  double_matrix var_diff(vars.size(),
                   double_array(std::numeric_limits<double>::max(),tree_size));

  double eps=threshold->getWrappedValue();
                                  
  for (int i=0; i<input.size(); i++)
    vars[i]->setValue(input[i]);
  
  // Check simplify 
  //std::string before=lIndividual.serialize();
  //std::cerr << "BEFORE: " << before << std::endl;
  //lTree.setContextToNode(start_node,lContext);
  //VDouble lResult;
  //lTree[start_node].mPrimitive->execute(lResult,lContext);

  GP::Primitive::Handle old_root=lTree[start_node].mPrimitive;
  simplify_subtree(lTree, lContext, start_node, input, min_values,
                   max_values, var_diff);
  lTree[start_node].mPrimitive=old_root;

  // See if there is anything to simplify
  double_array range(abs(max_values-min_values));
  double max_diff_value=var_diff[0].max();
  for (int i=1; i<var_diff.size(); i++)
    if (max_diff_value>var_diff[i].max())
      max_diff_value=var_diff[i].max();
  if (range.min()>eps && max_diff_value>eps)
    return false;
  
  lIndividual[0] = castHandleT<GP::Tree>(lIndividual.getTypeAlloc()->allocate());

  // Copy lActualTree's attributes into new tree
  lIndividual[0]->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
  lIndividual[0]->setNumberArguments(lActualTree->getNumberArguments());
  lIndividual[0]->clear();
  int full_size=lActualTree->size();
  for (int i=0; i<full_size; i++) {
    lIndividual[0]->push_back((*lActualTree)[i]);
    if ((*lActualTree)[i].mSubTreeSize<2 || i<start_node || i>=end_node)
      continue;
  
    // Replace with a constant double
    if (range[i]<=eps) {
      double meanv=(max_values[i]+min_values[i])/2.0;
      if (fabs(meanv)<1e-300)
        meanv=0.0;
      lIndividual[0]->back().mPrimitive = 
        castHandleT<GP::EphemeralT<VDouble> >(ePrim)->generate(ePrim->getName(),
                                                               lContext);
      lIndividual[0]->back().mPrimitive->setValue(VDouble(meanv));
      i=i+(*lActualTree)[i].mSubTreeSize-1;
    // Replace with x
    } else {
      for (int j=0; j<var_diff.size(); j++) {
        if (var_diff[j][i]<=eps) {
          lIndividual[0]->back().mPrimitive = vars[j];
          i=i+(*lActualTree)[i].mSubTreeSize-1;
          break;
        }
      }
    }
  }
  
  lContext.setGenotypeHandle(lIndividual[0]);
  lContext.setGenotypeIndex(0);
  if (start_node==0)
    lIndividual[0]->fixSubTreeSize(0);
  else {
    unsigned old_size=(*lIndividual[0])[start_node].mSubTreeSize;
    lIndividual[0]->fixSubTreeSize(start_node);
    old_size-=(*lIndividual[0])[start_node].mSubTreeSize;
    (*lIndividual[0])[0].mSubTreeSize-=old_size;
  }

  /*VDouble lResult2;
  unsigned right_tree=start_node;
  if (right_tree>1)
    right_tree=(*lIndividual[0])[1].mSubTreeSize+1;
  lIndividual[0]->setContextToNode(right_tree,lContext);
  (*lIndividual[0])[right_tree].mPrimitive->execute(lResult2,lContext);
  
  double rerror;
  if (lResult.size()!=lResult2.size()) {
    if (lResult.size()==1)
      rerror=(abs(lResult2.getWrappedValue().dvec-lResult[0])/lResult[0]).max();
    else
      rerror=(abs(lResult.getWrappedValue().dvec-lResult2[0])/lResult.getWrappedValue().dvec).max();
  } else
    rerror=(abs(lResult.getWrappedValue().dvec-
                lResult2.getWrappedValue().dvec)/lResult.getWrappedValue().dvec).max();
  if (lResult.max()>1e300 || lResult.max()<1e-300 || lResult.max()<1e-6)
    rerror=0;
  if (rerror>1e-4) {
    std::cerr << "Simplification error: " 
              << rerror << std::endl
              << "Before: " << before << std::endl
              << "After:  " << lIndividual.serialize() << std::endl
              << "Vec1: ";
    for (int i=0; i<lResult.size(); i++)
      std::cerr << lResult[i] << " ";
    std::cerr << "\nVec2: ";
    for (int i=0; i<lResult2.size(); i++)
      std::cerr << lResult2[i] << " ";
    std::cerr << std::endl;
    exit(1);
  }*/
  //std::cerr << "BEFORE: " << before << std::endl;
  //std::cerr << "AFTER:  " << lIndividual.serialize() << std::endl;

  return true;
  
  Beagle_StackTraceEndM("bool SimplifyOp::simplify(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)");
}

void GP::SimplifyOp::simplify_subtree(GP::Tree& lTree, GP::Context& ioContext,
                                      int start_node, Double_matrix input,
                                      double_array &minv, double_array &maxv,
                                      double_matrix &var_diff) {
  
  int j=start_node; 
  unsigned args=lTree[j].mPrimitive->getNumberArguments();
                                        
  if (args>0) {
    GP::Primitive::Handle old_left, old_right;
    unsigned i_right;
    if (args>1) {
      i_right=j+lTree[j+1].mSubTreeSize+1;
      old_right=lTree[i_right].mPrimitive;
      // Replace right branch with its answer
      simplify_subtree(lTree,ioContext,i_right,input,minv,maxv,var_diff);
    }
    old_left=lTree[j+1].mPrimitive;
    // Replace left branch
    simplify_subtree(lTree,ioContext,j+1,input,minv,maxv,var_diff);
    
    // Execute
    lTree.setContextToNode(j,ioContext);
    VDouble::Handle ans=new VDouble(input[0].size());
    lTree[j].mPrimitive->execute(*ans,ioContext);

    // Store range and difference from variables
    maxv[j]=(*ans).max();
    minv[j]=(*ans).min();
    // Equal to an input variable?
    for (int i=0; i<var_diff.size(); i++)
      var_diff[i][j]=abs((*ans-input[i]).getWrappedValue().dvec).max();

    // Replace primitive with its evaluated vector
    GP::Primitive::Handle replacement=new GP::VEphemeralD(ans);
    lTree[j].mPrimitive=replacement;
    
    // Return the old primitives to the arguments
    lTree[j+1].mPrimitive=old_left;
    if (args>1)
      lTree[i_right].mPrimitive=old_right;
  }
}
