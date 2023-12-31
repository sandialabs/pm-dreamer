/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/GP/src/Argument.cpp
 *  \brief  Implementation of class GP::Argument.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:06 $
 */

#include "beagle/GP.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct an argument primitive.
 *  \param inValueAlloc Allocator of arguments values.
 *  \param inEvalMode Evaluation mode of the arguments 
 *    (see \ref Beagle::GP::ArgumentT::ArgumentsEvaluationMode).
 *  \param inIndex Argument index.
 *  \param inName Name of the argument primitive.
 */
GP::Argument::Argument(Object::Alloc::Handle inValueAlloc,
                       unsigned int inEvalMode,
                       unsigned int inIndex,
                       Beagle::string inName) :
  GP::Primitive(0,inName),
  mSharedData(new SharedData(inValueAlloc,inEvalMode)),
  mIndex(inIndex)
{ }


/*!
 *  \brief Protected constructor of an argument, used internally.
 *  \param inSharedData Handle to share data between arguments.
 *  \param inIndex Index of the argument.
 *  \param inName Name of the argument primitive.
 */
GP::Argument::Argument(GP::Argument::SharedData::Handle inSharedData,
                       unsigned int inIndex,
                       Beagle::string inName) :
  GP::Primitive(0,inName),
  mSharedData(inSharedData),
  mIndex(inIndex)
{ }


/*!
 *  \brief Generate a new argument of the same type than the actual one.
 *  \param inIndex Index the generated argument is refering to.
 *  \return Handle to the generated argument primitive.
 */
GP::Argument::Handle GP::Argument::generateArgument(unsigned int inIndex) const
{
  Beagle_StackTraceBeginM();
  return new GP::Argument(mSharedData, inIndex, getName());
  Beagle_StackTraceEndM("GP::Argument::Handle GP::Argument::generateArgument(unsigned int inIndex) const");
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return typing of the arguments.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info of the arguments.
 */
const std::type_info* GP::Argument::getReturnType(GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Object::Bag::Alloc::Handle lObjAlloc =
    castHandleT<Object::Bag::Alloc>(mSharedData->mCaches.getTypeAlloc());
  Object::Handle lObj = lObjAlloc->getContainerTypeAlloc()->allocate();
  return &typeid(*lObj);
  Beagle_StackTraceEndM("const std::type_info* GP::Argument::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Return selection weight of the argument primitive.
 *  \param inNumberArguments Number of arguments to get weight for.
 *  \param ioContext Evolutionary context.
 *  \return Selection weight for the given number of arguments.
 */
double GP::Argument::getSelectionWeight(unsigned int inNumberArguments, 
                                        GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  if(ioContext.getGenotypeIndex() == 0) return 0.0;
  if((inNumberArguments==0) || (inNumberArguments==GP::Primitive::eAny)) {
    const unsigned int lTreeNbArgs = ioContext.getGenotype().getNumberArguments();
    if(lTreeNbArgs > 0) return double(lTreeNbArgs);
  }
  return 0.0;
  Beagle_StackTraceEndM("double GP::Argument::getSelectionWeight(unsigned int inNumberArguments, GP::Context& ioContext) const");
}


/*!
 *  \brief Return a reference to the actual argument.
 *  \param inNumberArguments Number of arguments asked for.
 *  \param ioContext Evolutionary context.
 *  \return Handle to argument.
 */
GP::Primitive::Handle GP::Argument::giveReference(unsigned int inNumberArguments,
                                                  GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mIndex!=eGenerator) return this;
  const unsigned int lTreeNbArgs = ioContext.getGenotype().getNumberArguments();
  Beagle_AssertM(lTreeNbArgs > 0);
  const unsigned int lGenIndex =
    ioContext.getSystem().getRandomizer().rollInteger(0,(lTreeNbArgs-1));
  return generateArgument(lGenIndex);
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::Argument::giveReference(unsigned int inNumberArguments, GP::Context& ioContext)");
}


/*!
 *  \brief Execute argument primitive.
 *  \param outResult Result containing value of argument.
 *  \param ioContext Evolutionary context.
 */
void GP::Argument::execute(GP::Datum& outResult, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Object::Bag::Alloc::Handle lCacheBagAlloc =
    castHandleT<Object::Bag::Alloc>(mSharedData->mCaches.getTypeAlloc());
  Object::Alloc::Handle lValuesAlloc = lCacheBagAlloc->getContainerTypeAlloc();
  if(mSharedData->mEvalMode==ePreCompute) {
    Beagle_AssertM(mIndex < mSharedData->mCaches.back()->size());
    lValuesAlloc->copy(outResult, *(*mSharedData->mCaches.back())[mIndex]);
  }
  else if(mSharedData->mEvalMode==eJustInTime) {
    GP::Context::Handle lActualContext = mSharedData->mEvalContext.back();
    mSharedData->mEvalContext.pop_back();
    lActualContext->setAllowedNodesExecution(ioContext.getAllowedNodesExecution());
    lActualContext->setAllowedExecutionTime(ioContext.getAllowedExecutionTime());
    lActualContext->setNodesExecutionCount(ioContext.getNodesExecutionCount());
    lActualContext->getExecutionTimer() = ioContext.getExecutionTimer();
    getArgument(mIndex, outResult, *lActualContext);
    ioContext.getExecutionTimer() = lActualContext->getExecutionTimer();
    ioContext.setNodesExecutionCount(lActualContext->getNodesExecutionCount());
    ioContext.setAllowedExecutionTime(lActualContext->getAllowedExecutionTime());
    ioContext.setAllowedNodesExecution(lActualContext->getAllowedNodesExecution());
    mSharedData->mEvalContext.push_back(lActualContext);
  }
  else if(mSharedData->mEvalMode==eCaching) {
    Beagle_AssertM(mIndex < mSharedData->mCaches.back()->size());
    if((*mSharedData->mCaches.back())[mIndex]!=NULL) {
      lValuesAlloc->copy(outResult, *(*mSharedData->mCaches.back())[mIndex]);
    }
    else {
      Object::Bag::Handle lActualCache = mSharedData->mCaches.back();
      mSharedData->mCaches.pop_back();
      GP::Context::Handle lActualContext = mSharedData->mEvalContext.back();
      mSharedData->mEvalContext.pop_back();
      lActualContext->setAllowedNodesExecution(ioContext.getAllowedNodesExecution());
      lActualContext->setAllowedExecutionTime(ioContext.getAllowedExecutionTime());
      lActualContext->setNodesExecutionCount(ioContext.getNodesExecutionCount());
      lActualContext->getExecutionTimer() = ioContext.getExecutionTimer();
      getArgument(mIndex, outResult, *lActualContext);
      ioContext.getExecutionTimer() = lActualContext->getExecutionTimer();
      ioContext.setNodesExecutionCount(lActualContext->getNodesExecutionCount());
      ioContext.setAllowedExecutionTime(lActualContext->getAllowedExecutionTime());
      ioContext.setAllowedNodesExecution(lActualContext->getAllowedNodesExecution());
      mSharedData->mEvalContext.push_back(lActualContext);
      mSharedData->mCaches.push_back(lActualCache);
      (*lActualCache)[mIndex] = lValuesAlloc->clone(outResult);
    }
  }
  else {
    throw Beagle_InternalExceptionM(string("Undefined evaluation mode (")+
      uint2str(mSharedData->mEvalMode)+string(") for arguments!"));
  }
  Beagle_StackTraceEndM("void GP::Argument::execute(GP::Datum& outResult, GP::Context& ioContext)");
}


/*!
 *  \brief Return whether selection weight is stable for given number of arguments.
 *  \param inNumberArguments Number of arguments to test weight stability.
 *  \return True if selection weight is table for given number of arguments, false if not.
 */
bool GP::Argument::isSelectionWeightStable(unsigned int inNumberArguments) const
{
  Beagle_StackTraceBeginM();
  if((inNumberArguments==0) || (inNumberArguments==GP::Primitive::eAny)) return false;
  return true;
  Beagle_StackTraceEndM("bool GP::Argument::isSelectionWeightStable(unsigned int inNumberArguments) const");
}


/*!
 *  \brief Push execution context to prepare the execution of the argument.
 *  \param inNumberArguments Number of arguments of called tree.
 *  \param ioContext Evolutionary context.
 */
void GP::Argument::pushExecutionContext(unsigned int inNumberArguments, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  switch(mSharedData->mEvalMode) {
    case eCaching:
    {
      Object::Bag::Alloc::Handle lCacheBagAlloc =
        castHandleT<Object::Bag::Alloc>(mSharedData->mCaches.getTypeAlloc());
      Object::Alloc::Handle lValuesAlloc = lCacheBagAlloc->getContainerTypeAlloc(); 
      Object::Bag::Handle lNewCache = castHandleT<Object::Bag>(lCacheBagAlloc->allocate());
      lNewCache->resize(inNumberArguments);
      mSharedData->mCaches.push_back(lNewCache);
      GP::Context::Alloc& lContextAlloc = 
        castObjectT<GP::Context::Alloc&>(ioContext.getSystem().getContextAllocator());
      GP::Context::Handle lNewEvalContext = 
       castHandleT<GP::Context>(lContextAlloc.clone(ioContext));
      mSharedData->mEvalContext.push_back(lNewEvalContext);
      break;
    }
    case eJustInTime:
    {
      GP::Context::Alloc& lContextAlloc = 
        castObjectT<GP::Context::Alloc&>(ioContext.getSystem().getContextAllocator());
      GP::Context::Handle lNewEvalContext = 
       castHandleT<GP::Context>(lContextAlloc.clone(ioContext));
      mSharedData->mEvalContext.push_back(lNewEvalContext);
      break;
    }
    case ePreCompute:
    {
      Object::Bag::Alloc::Handle lCacheBagAlloc =
        castHandleT<Object::Bag::Alloc>(mSharedData->mCaches.getTypeAlloc());
      Object::Alloc::Handle lValuesAlloc = lCacheBagAlloc->getContainerTypeAlloc(); 
      Object::Bag::Handle lNewCache = castHandleT<Object::Bag>(lCacheBagAlloc->allocate());
      lNewCache->resize(inNumberArguments);
      for(unsigned int i=0; i<inNumberArguments; ++i) getArgument(i, *(*lNewCache)[i], ioContext);
      mSharedData->mCaches.push_back(lNewCache);
    }
    default:
    {
      throw Beagle_InternalExceptionM(string("Undefined evaluation mode (")+
        uint2str(mSharedData->mEvalMode)+string(") for the arguments!"));
    }
  }
  Beagle_StackTraceEndM("void GP::Argument::pushExecutionContext(unsigned int inNumberArguments, GP::Context& ioContext)");
}


/*!
 *  \brief Pop execution context as the execution of the argument is done.
 */
void GP::Argument::popExecutionContext()
{
  Beagle_StackTraceBeginM();
  switch(mSharedData->mEvalMode) {
    case eCaching:
    {
      mSharedData->mCaches.pop_back();
      mSharedData->mEvalContext.pop_back();
      break;
    }
    case eJustInTime:
    {
      mSharedData->mEvalContext.pop_back();
      break;
    }
    case ePreCompute:
    {
      mSharedData->mCaches.pop_back();
      break;
    }
    default:
    {
      throw Beagle_InternalExceptionM(string("Undefined evaluation mode (")+
        uint2str(mSharedData->mEvalMode)+string(") for the arguments!"));
    }
  }
  Beagle_StackTraceEndM("void GP::Argument::popExecutionContext()");
}


/*!
 *  \brief Read an argument primitive from XML subtree.
 *  \param inIter XML iterator to read primitive from.
 *  \param ioContext Evolutionary context.
 */
void GP::Argument::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  GP::Primitive::readWithContext(inIter, ioContext);
  string lIndexValue = inIter->getAttribute("id").c_str();
  if(lIndexValue.empty()==false) {
    mIndex = str2uint(lIndexValue);
    Beagle_AssertM(mIndex < ioContext.getGenotype().getNumberArguments());
  }
  Beagle_StackTraceEndM("void GP::Argument::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)");
}


/*!
 *  \brief Validate the arguments position in the tree.
 *  \param ioContext Evolutionary context.
 *  \return True if the argument is correctly positioned, false if not.
 *  \throw Beagle::AssertException If the context is in a bad state.
 */
bool GP::Argument::validate(GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  if(GP::Primitive::validate(ioContext) == false) return false;
  if(ioContext.getGenotypeIndex() == 0) return false;
  if(mIndex >= ioContext.getGenotype().getNumberArguments()) return false;
  return true;
  Beagle_StackTraceEndM("bool GP::Argument::validate(GP::Context& ioContext) const");
}


/*!
 *  \brief Write argument primitive into XML streamer.
 *  \param ioStreamer XML streamer to write primitive into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::Argument::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  if(mIndex!=eGenerator) ioStreamer.insertAttribute("id", uint2str(mIndex));
  Beagle_StackTraceEndM("void GP::Argument::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
