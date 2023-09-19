/***************************************************************************
                               HybridOptOp.cpp
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

#include "EphemeralD.hpp"
#include <cassert>
#include "beagle/Beagle.hpp"
#include "beagle/GP.hpp"
#include "HybridOptOp.hpp"

using namespace Beagle;
using namespace GP;

template <class DoubTp>
HybridOptOp<DoubTp>::HybridOptOp(EvaluationOp::Handle fitguy,
                                 MutationOp::Handle simplify_op,
                                 string inMutationPbName,
                                 string inEphemeralNameParamName,string inName):
  fit_guy(fitguy),
  simple_op(simplify_op),
  Beagle::MutationOp(inMutationPbName, inName),
  mEphemeralNameParamName(inEphemeralNameParamName)
{ }

template <class DoubTp>
void HybridOptOp<DoubTp>::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  simple_op->initialize(ioSystem);

  // Optimization probability
  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(float(0.05));
    string lLongDescrip = "Ephemeral hybrid optimization probability for an individual. ";
    lLongDescrip += "A ephemeral hydrib optimization to change the value of a specific primitive.";
    Register::Description lProbaDescription(
      "Ephemeral hybrid optimization probability",
      "Float",
      "0.05",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lProbaDescription);
  }

  // Primitive type to optimize
  if(ioSystem.getRegister().isRegistered(mEphemeralNameParamName)) {
    mEphemeralName = castHandleT<String>(ioSystem.getRegister()[mEphemeralNameParamName]);
  } else {
    mEphemeralName = new String("E");
    Register::Description lDescription(
      "Ephemeral primitive name",
      "String",
      "E",
      string("Name of the ephemeral primitive for which the values are modified ")+
      string("by the ephemeral mutation operator.")
    );
    ioSystem.getRegister().addEntry(mEphemeralNameParamName, mEphemeralName, lDescription);
  }

  // Optimization type
  if(ioSystem.getRegister().isRegistered("gp.hybopt.type")) {
    opt_type = castHandleT<UInt>(ioSystem.getRegister().getEntry("gp.hybopt.type"));
  } else {
    opt_type = new UInt(1);
    string lLongDescrip = "Ephemeral hybrid optimization type. ";
    lLongDescrip += "If 0, all ephemeral nodes are optimized in a multidimensional ";
    lLongDescrip += "minimization. Otherwise a random node is chosen.";
    Register::Description lProbaDescription(
      "Ephemeral hybrid optimization type",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.hybopt.type", opt_type, lProbaDescription);
  }

  // Simplification
  if(ioSystem.getRegister().isRegistered("gp.hybopt.simplify")) {
    simplify = castHandleT<UInt>(ioSystem.getRegister().getEntry("gp.hybopt.simplify"));
  } else {
    simplify = new UInt(0);
    string lLongDescrip = "Simplify before optimization. ";
    lLongDescrip += "If 0, no simplification is performed.";
    Register::Description lProbaDescription(
      "Hybrid optimization simplification type",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.hybopt.simplify", simplify, lProbaDescription);
  }

  // Maximum iterations
  if(ioSystem.getRegister().isRegistered("gp.hybopt.maxi")) {
    maxi = castHandleT<UInt>(ioSystem.getRegister().getEntry("gp.hybopt.maxi"));
  } else {
    maxi = new UInt(10);
    string lLongDescrip = "Maximum iterations for hybrid optimization. ";
    Register::Description lProbaDescription(
      "Ephemeral hybrid optimization type",
      "UInt",
      "10",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.hybopt.maxi", maxi, lProbaDescription);
  }

  // Optimization of multiple particle types
  if(ioSystem.getRegister().isRegistered("gp.hybopt.mtypes")) {
    mtypes = castHandleT<UInt>(ioSystem.getRegister().getEntry("gp.hybopt.mtypes"));
  } else {
    mtypes = new UInt(1);
    string lLongDescrip = "Perform optimization for E_p and E_t. ";
    Register::Description lProbaDescription(
      "Perform optimization for E_p and E_t",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("gp.hybopt.mtypes", mtypes, 
                                    lProbaDescription);
  }

  Beagle_StackTraceEndM("void HybridOptOp::initialize(Beagle::System& ioSystem)");
}

template <class DoubTp>
void HybridOptOp<DoubTp>::postInit(Beagle::System& ioSystem) {
  simple_op->postInit(ioSystem);
  s_inter=castHandleT<UInt>(ioSystem.getRegister().getEntry("gp.simplify.interval"));
  assert(s_inter!=NULL);
}

template <class DoubTp>
bool HybridOptOp<DoubTp>::mutate(Beagle::Individual& ioIndividual, 
                                 Beagle::Context& ioContext) {
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioContext.getSystem().getLogger(),
    "mutation", "HybridOptOp",
    string("Mutating ")+uint2ordinal(ioContext.getGenotypeIndex()+1)+
    string(" individual with GP::HybridOptOp")
  );

  unsigned otype=opt_type->getWrappedValue();
  int max_iter=static_cast<int>(maxi->getWrappedValue());
  unsigned simplify_type=simplify->getWrappedValue();
  return optimize(ioIndividual,ioContext,otype,max_iter,simplify_type);

  Beagle_StackTraceEndM("bool GP::HybridOptOp::mutate(Individual& ioIndividual, Context& ioContext)");
}

template <class DoubTp>
bool HybridOptOp<DoubTp>::optimize(Beagle::Individual& ioIndividual, 
                                   Beagle::Context& ioContext, unsigned otype, 
                                   unsigned max_iter, unsigned simplify_type) {
  Beagle_StackTraceBeginM();

  bool use_mtypes=mtypes->getWrappedValue();

  if (max_iter==0)
    return false;

  bool changed=false;
  if (simplify_type!=0) {
    UInt old_sinter=*s_inter;
    *s_inter=1;
    changed=simple_op->mutate(ioIndividual,ioContext);
    *s_inter=old_sinter;
  }
  
  GP::Individual& lIndividual = castObjectT<GP::Individual&>(ioIndividual);
  GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

  float orig_fitness;
  if (lIndividual.getFitness()->isValid()==false) {
    FitnessSimple::Handle fitp = 
      castHandleT<FitnessSimple>(fit_guy->evaluate(lIndividual,lContext));
    lIndividual.setFitness(fitp);
    lIndividual.getFitness()->setValid();
    orig_fitness=fitp->getValue();
  } else 
    orig_fitness=castHandleT<FitnessSimple>(lIndividual.getFitness())->getValue();
  if (orig_fitness>=1.0)
    return changed;
  
  // Get index of potential primitives with parameters that can be selected for mutation.
  std::vector< std::pair<unsigned int,unsigned int>,
               BEAGLE_STLALLOCATOR< std::pair<unsigned int,unsigned int> > > lPotentialParam;
  std::vector< std::pair<unsigned int,unsigned int>,
               BEAGLE_STLALLOCATOR< std::pair<unsigned int,unsigned int> > > lPotentialVec;
  for(unsigned int i=0; i<lIndividual.size(); ++i) {
    GP::Tree& lTree = *lIndividual[i];
    for(unsigned int j=0; j<lTree.size(); ++j) {
      string node_name=lTree[j].mPrimitive->getName();
      if(node_name == *mEphemeralName)
        lPotentialParam.push_back(std::make_pair(i,j));
      if (use_mtypes)
        if (node_name=="E_p" || node_name=="E_t")
          lPotentialVec.push_back(std::make_pair(i,j));
    }
  }

  // Return if there is not potential parameters.
  if (lPotentialParam.empty() && lPotentialVec.empty())
    return changed;
    
  // Choose only 1 node if the optimization type is non-zero
  if (otype!=0) {
    std::vector< std::pair<unsigned int,unsigned int>,
      BEAGLE_STLALLOCATOR< std::pair<unsigned int,unsigned int> > > swapt;
    unsigned selected=lContext.getSystem().getRandomizer().rollInteger(0,lPotentialParam.size()+lPotentialVec.size()-1);
    if (selected<lPotentialParam.size()) {
      swapt.push_back(lPotentialParam[selected]);
      swapt.swap(lPotentialParam);
      lPotentialVec.clear();
    } else {
      selected-=lPotentialParam.size();
      swapt.push_back(lPotentialVec[selected]);
      lPotentialParam.clear();
    }
  }
  
  int sum_mtype=0;
  GP::Tree::Handle lSelectedTree;
  for (int i=0; i<lPotentialVec.size(); i++) {
    lSelectedTree = lIndividual[lPotentialVec[i].first];
    lContext.setGenotypeIndex(lPotentialVec[i].first);
    lContext.setGenotypeHandle(lSelectedTree);
    GP::Primitive::Handle lSelectedPrimit =
           (*lSelectedTree)[lPotentialVec[i].second].mPrimitive;
    int nsize=castHandleT<VEphemeralD>
      ((*lSelectedTree)[lPotentialVec[i].second].mPrimitive)->num_terms();
    sum_mtype+=nsize;
  }

  // Mutating a primitive
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "HybridOptOp",
    string("Individual before GP parameters mutation: ")+
    ioIndividual.serialize()
  );

  // Store original context values
  unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
  GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();

  //std::cout << "\nBEFORE: " << orig_fitness << " : " 
  //          << ioIndividual.serialize() << std::endl;
    
  // ---------------------------------------------------------------------
  // --------------------- Minimization
  // ---------------------------------------------------------------------
  int status,iter=0;
  const gsl_multimin_fminimizer_type *T=gsl_multimin_fminimizer_nmsimplex;
  gsl_multimin_fminimizer *s=NULL;
  int ns=lPotentialParam.size();
  int nv=lPotentialVec.size();
  int np=ns+sum_mtype;
  GSL_Params params;
  params.vector_double=gsl_use_vect<DoubTp>();
  params.nodes=&lPotentialParam;
  params.vnodes=&lPotentialVec;
  params.ind=&lIndividual;
  params.fit_guy=fit_guy;
  params.ioContext=&lContext;
  gsl_vector *ss=gsl_vector_alloc(np), *x=gsl_vector_alloc(np), 
             *x_orig=gsl_vector_alloc(np);
  gsl_multimin_function F;
  
  F.f = &gsl_objfun;
  F.n = np;
  F.params = (void *)(&params);
  gsl_vector_set_all(ss,1.0);
  
  s=gsl_multimin_fminimizer_alloc(T,np);
  
  // Set the initial vector to be the current values
  for (int i=0; i<ns; i++) {
    lSelectedTree = lIndividual[lPotentialParam[i].first];
    lContext.setGenotypeIndex(lPotentialParam[i].first);
    lContext.setGenotypeHandle(lSelectedTree);

    // Mutate parameter value.
    GP::Primitive::Handle lSelectedPrimit =
           (*lSelectedTree)[lPotentialParam[i].second].mPrimitive;
    DoubTp m;
    castHandleT<EphemeralT<DoubTp> >
      ((*lSelectedTree)[lPotentialParam[i].second].mPrimitive)->getValue(m);
    GP::Primitive::Handle newdb=
      castHandleT<typename GP::EphemeralT<DoubTp> >(lSelectedPrimit)->
        generate(mEphemeralName->getWrappedValue(),lContext);
    (*lSelectedTree)[lPotentialParam[i].second].mPrimitive=newdb;
    gsl_vector_set(x,i,m);
    gsl_vector_set(x_orig,i,m);
  }
  
  // Do the same for the multi-type vectors
  int vloc=ns;
  for (int i=0; i<nv; i++) {
    lSelectedTree = lIndividual[lPotentialVec[i].first];
    lContext.setGenotypeIndex(lPotentialVec[i].first);
    lContext.setGenotypeHandle(lSelectedTree);

    // Mutate parameter value.
    GP::Primitive::Handle lSelectedPrimit =
           (*lSelectedTree)[lPotentialVec[i].second].mPrimitive;
    VDouble::Handle m = new VDouble;
    castHandleT<EphemeralT<DoubTp> >(lSelectedPrimit)->getValue(*m);
    GP::Primitive::Handle newdb=
      new GP::VEphemeralD(*(castHandleT<VEphemeralD>(lSelectedPrimit)),m);
    for (int j=0; j<m->num_terms(); j++) {
      gsl_vector_set(x,vloc,m->term(j));
      gsl_vector_set(x_orig,vloc,m->term(j));
      vloc++;
    }
  }
  
  gsl_multimin_fminimizer_set(s,&F,x,ss);
  while (iter<max_iter) {
    status=gsl_multimin_fminimizer_iterate(s);
    if (status)
      break;
    iter++;
  }
  
  gsl_multimin_fminimizer_free(s);
  // ---------------------------------------------------------------------
  // --------------------- End Minimization
  // ---------------------------------------------------------------------
    
  float new_fitness = 
      castHandleT<FitnessSimple>(ioIndividual.getFitness())->getValue();
  if (new_fitness<orig_fitness)
    gsl_objfun(x_orig,(void*)&params);
  
  //std::cout << "AFTER: " << new_fitness << " : "
  //          << ioIndividual.serialize() << std::endl;

  gsl_vector_free(x);
  gsl_vector_free(x_orig);
  gsl_vector_free(ss);

  // Restore original context values
  lContext.setGenotypeIndex(lOldGenotypeIndex);
  lContext.setGenotypeHandle(lOldGenotypeHandle);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "HybridOptOp",
    string("Individual after GP parameters mutation: ")+
    ioIndividual.serialize()
  );

  return true;
  Beagle_StackTraceEndM("bool GP::HybridOptOp::optimize(...)");
}

template <class DoubTp>
void HybridOptOp<DoubTp>::readWithMap(PACC::XML::ConstIterator inIter,
                                                      Beagle::OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lEphemeralNameParamName = inIter->getAttribute("primitname").c_str();
  if(lEphemeralNameParamName.empty() == false) mEphemeralNameParamName = lEphemeralNameParamName;
  Beagle_StackTraceEndM("void GP::HybridOptOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}

template <class DoubTp>
void HybridOptOp<DoubTp>::writeContent(PACC::XML::Streamer& ioStreamer,
                                           bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("primitname", mEphemeralNameParamName);
  Beagle_StackTraceEndM("void GP::HybridOptOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

// Objective function for evaluation of an individual in C
double gsl_objfun(const gsl_vector *x, void *params) {
  GSL_Params *p=(GSL_Params *)params;

  // Replace the ephemeral nodes in the tree with x
  int ns=p->nodes->size();
  GP::Tree::Handle lSelectedTree;
  for (int i=0; i<ns; i++) {
    lSelectedTree = (*(p->ind))[(*(p->nodes))[i].first];
    p->ioContext->setGenotypeIndex((*(p->nodes))[i].first);
    p->ioContext->setGenotypeHandle(lSelectedTree);
    if (p->vector_double) {
      VDouble m=gsl_vector_get(x,i);
      castHandleT<EphemeralT<VDouble> >
        ((*lSelectedTree)[(*(p->nodes))[i].second].mPrimitive)->setValue(m);
    } else {
      Double m=gsl_vector_get(x,i);
      castHandleT<EphemeralT<Double> >
        ((*lSelectedTree)[(*(p->nodes))[i].second].mPrimitive)->setValue(m);
    }
  }

  int nv=p->vnodes->size();
  int vloc=ns;
  for (int i=0; i<nv; i++) {
    lSelectedTree = (*(p->ind))[(*(p->vnodes))[i].first];
    p->ioContext->setGenotypeIndex((*(p->vnodes))[i].first);
    p->ioContext->setGenotypeHandle(lSelectedTree);
    GP::VEphemeralD::Handle lSelectedPrimit =
      castHandleT<VEphemeralD>((*lSelectedTree)[(*(p->vnodes))[i].second].mPrimitive);
    int nsize=lSelectedPrimit->num_terms();
    for (int j=0; j<nsize; j++) {
      double m=gsl_vector_get(x,vloc);
      lSelectedPrimit->set_term(j,m);
      vloc++;
    }
    lSelectedPrimit->update_dvec();
  }
  
  FitnessSimple::Handle fitp = 
    castHandleT<FitnessSimple>(p->fit_guy->evaluate(*(p->ind),*(p->ioContext)));
  p->ind->setFitness(fitp);
  p->ind->getFitness()->setValid();
  return fitp->getValue()*-1.0;
}

template class HybridOptOp<Double>;
template class HybridOptOp<VDouble>;
