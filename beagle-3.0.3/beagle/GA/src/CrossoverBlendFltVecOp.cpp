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
 *  \file   beagle/GA/src/CrossoverBlendFltVecOp.cpp
 *  \brief  Source code of class GA::CrossoverBlendFltVec.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14.2.2 $
 *  $Date: 2007/05/11 19:18:44 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a real-valued GA blend crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::GA::CrossoverBlendFltVecOp::CrossoverBlendFltVecOp(string inMatingPbName,
                                                           string inName) :
  CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Initialize the real-valued GA blend crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::GA::CrossoverBlendFltVecOp::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  CrossoverOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMatingProbaName)) {
    ioSystem.getRegister().deleteEntry(mMatingProbaName);
  }

  if(ioSystem.getRegister().isRegistered(mMatingProbaName)) {
    mMatingProba = castHandleT<Float>(ioSystem.getRegister()[mMatingProbaName]);
  } else {
    mMatingProba = new Float(float(0.3));
    Register::Description lDescription(
      "Individual blend crossover prob.",
      "Float",
      "0.3",
      "Real-valued GA blend crossover probability of a single individual."
    );
    ioSystem.getRegister().addEntry(mMatingProbaName, mMatingProba, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.maxvalue")) {
    mMaxValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.maxvalue"]);
  } else {
    mMaxValue = new DoubleArray(1,DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "Maximum values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the maximum value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the maximum value vector.";
    Register::Description lDescription(
      "Maximum vector values",
      "DoubleArray",
      dbl2str(DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.maxvalue", mMaxValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.minvalue")) {
    mMinValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.minvalue"]);
  } else {
    mMinValue = new DoubleArray(1,-DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "Minimum  values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the minimum value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the minimum value vector.";
    Register::Description lDescription(
      "Minimum values",
      "DoubleArray",
      dbl2str(-DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.minvalue", mMinValue, lDescription);
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

  if(ioSystem.getRegister().isRegistered("ga.cxblend.alpha")) {
    mAlpha = castHandleT<Double>(ioSystem.getRegister()["ga.cxblend.alpha"]);
  } else {
    mAlpha = new Double(0.5);
    Register::Description lDescription(
      "Blend crossover alpha value",
      "Double",
      "0.5",
      "GA blend crossover alpha parameter moduling amplitude of operation."
    );
    ioSystem.getRegister().addEntry("ga.cxblend.alpha", mAlpha, lDescription);
  }
  Beagle_StackTraceEndM("void GA::CrossoverBlendFltVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Mate two GA individuals for real-valued GA blend crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::GA::CrossoverBlendFltVecOp::mate(Beagle::Individual& ioIndiv1,
                                              Beagle::Context&    ioContext1,
                                              Beagle::Individual& ioIndiv2,
                                              Beagle::Context&    ioContext2)
{
  Beagle_StackTraceBeginM();
  unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
  if(lNbGenotypes == 0) return false;

  Beagle_LogDebugM(
    ioContext1.getSystem().getLogger(),
    "crossover", "Beagle::GA::CrossoverBlendFltVec",
    string("The first individual mated is (before real-valued GA blend crossover): ")+
    ioIndiv1.serialize()
  );
  Beagle_LogDebugM(
    ioContext1.getSystem().getLogger(),
    "crossover", "Beagle::GA::CrossoverBlendFltVec",
    string("The second individual mated is (before real-valued GA blend crossover): ")+
    ioIndiv2.serialize()
  );

  for(unsigned int i=0; i<lNbGenotypes; ++i) {
    GA::FloatVector::Handle lFloatVector1 = castHandleT<FloatVector>(ioIndiv1[i]);
    GA::FloatVector::Handle lFloatVector2 = castHandleT<FloatVector>(ioIndiv2[i]);
    const unsigned int lSize = minOf<unsigned int>(lFloatVector1->size(), lFloatVector2->size());
    for(unsigned int j=0; j<lSize; ++j) {
      const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
      const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
      const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
      const double lU_i = ioContext1.getSystem().getRandomizer().rollUniform();
      const double lGamma_i = ((1.0+2.0*mAlpha->getWrappedValue())*lU_i)-mAlpha->getWrappedValue();
      const double lX1_i = (*lFloatVector1)[j];
      const double lX2_i = (*lFloatVector2)[j];
      (*lFloatVector1)[j] = ((1.0-lGamma_i)*lX1_i) + (lGamma_i*lX2_i);
      (*lFloatVector2)[j] = (lGamma_i*lX1_i) + ((1.0-lGamma_i)*lX2_i);
      if(lIncVal!=0.0) {
        (*lFloatVector1)[j] = lIncVal * round((*lFloatVector1)[j] / lIncVal);
        (*lFloatVector2)[j] = lIncVal * round((*lFloatVector2)[j] / lIncVal);
      }
      if((*lFloatVector1)[j] > lMaxVal) (*lFloatVector1)[j] = lMaxVal;
      if((*lFloatVector1)[j] < lMinVal) (*lFloatVector1)[j] = lMinVal;
      if((*lFloatVector2)[j] > lMaxVal) (*lFloatVector2)[j] = lMaxVal;
      if((*lFloatVector2)[j] < lMinVal) (*lFloatVector2)[j] = lMinVal;
    }
  }

  Beagle_LogDebugM(
    ioContext1.getSystem().getLogger(),
    "crossover", "Beagle::GA::CrossoverBlendFltVec",
    string("The first individual mated is (after real-valued GA blend crossover): ")+
    ioIndiv1.serialize()
  );
  Beagle_LogDebugM(
    ioContext1.getSystem().getLogger(),
    "crossover", "Beagle::GA::CrossoverBlendFltVec",
    string("The second individual mated is (after real-valued GA blend crossover): ")+
    ioIndiv2.serialize()
  );

  return true;
  Beagle_StackTraceEndM("bool GA::CrossoverBlendFltVecOp::mate(Individual& ioIndiv1, Context& ioContext1, Individual& ioIndiv2, Context& ioContext2)");
}


