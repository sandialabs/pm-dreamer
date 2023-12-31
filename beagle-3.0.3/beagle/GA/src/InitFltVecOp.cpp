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
 *  \file   beagle/GA/src/InitFltVecOp.cpp
 *  \brief  Source code of class GA::InitFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a GA float vectors uniformly distributed initialization operator.
 *  \param inFloatVectorSize Size of the float vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GA::InitFltVecOp::InitFltVecOp(unsigned int inFloatVectorSize,
                               Beagle::string inReproProbaName,
                               Beagle::string inName) :
  InitializationOp(inReproProbaName, inName),
  mFloatVectorSize(new UInt(inFloatVectorSize))
{ }


/*!
 *  \brief Initialize the GA float vectors uniformly distributed initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitFltVecOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  InitializationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
    mFloatVectorSize = castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
  } else {
    Register::Description lDescription(
      "Initial float vectors sizes",
      "UInt",
      mFloatVectorSize->serialize(),
      "Float vector size of initialized individuals."
    );
    ioSystem.getRegister().addEntry("ga.init.vectorsize", mFloatVectorSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.maxvalue")) {
    mMaxInitValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.init.maxvalue"]);
  } else {
    mMaxInitValue = new DoubleArray(1,1.0);
    std::ostringstream lOSS;
    lOSS << "Maximum initialization values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the initialization value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the maximum initialization value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the maximum initialization value vector.";
    Register::Description lDescription(
      "Maximum initialization values",
      "DoubleArray",
      "1.0",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.init.maxvalue", mMaxInitValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.minvalue")) {
    mMinInitValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.init.minvalue"]);
  } else {
    mMinInitValue = new DoubleArray(1,-1.0);
    std::ostringstream lOSS;
    lOSS << "Minimum initialization values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the initialization value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the minimum initialization value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the minimum initialization value vector.";
    Register::Description lDescription(
      "Minimum initialization values",
      "DoubleArray",
      "-1.0",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.init.minvalue", mMinInitValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.inc")) {
    mIncValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.inc"]);
  } else {
    mIncValue = new DoubleArray(1,0.0);
    std::ostringstream lOSS;
    lOSS << "Increments of valid values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the value is not evenly divisible by the ";
    lOSS << "increment, the value will be set to the closest valid ";
    lOSS << "value.";
    Register::Description lDescription(
      "Increments of valid values",
      "DoubleArray",
      "0.0",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.inc", mIncValue, lDescription);
  }
  Beagle_StackTraceEndM("void GA::InitFltVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Initialize real-valued GA individual with numbers uniformly distributed in a given range.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitFltVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mFloatVectorSize->getWrappedValue() == 0) {
    string lMessage = "GA::InitFltVecOp::initIndividual: ";
    lMessage += "float vector size parameter is zero; ";
    lMessage += "could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }  
#endif // BEAGLE_NDEBUG
  outIndividual.resize(1);
  GA::FloatVector::Handle lFloatVector = castHandleT<GA::FloatVector>(outIndividual[0]);
  lFloatVector->resize(mFloatVectorSize->getWrappedValue());
  for(unsigned int j=0; j<lFloatVector->size(); ++j) {
    const double lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
    const double lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
    const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
    (*lFloatVector)[j] = ioContext.getSystem().getRandomizer().rollUniform(lMinVal,lMaxVal);
    if(lIncVal!=0.0) {
      (*lFloatVector)[j] = lIncVal * round((*lFloatVector)[j] / lIncVal);
      if((*lFloatVector)[j] > lMaxVal) (*lFloatVector)[j] = lMaxVal;
      if((*lFloatVector)[j] < lMinVal) (*lFloatVector)[j] = lMinVal;
    }
  }
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::GA::InitFltVecOp",
    string("Float vector initialized as: ")+lFloatVector->serialize()
  );
  Beagle_StackTraceEndM("void GA::InitFltVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)");
}

