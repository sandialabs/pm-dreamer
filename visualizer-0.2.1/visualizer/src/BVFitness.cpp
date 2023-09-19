/*
 *  BEAGLE Visualizer
 *  Copyright (C) 2004 by Christian Gagne, Patrick-Emmanuel Boulanger-Nadeau,
 *  and Vincent-Olivier Gravel.
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

#include <sstream>
#include <stdexcept>
#include "XML/Iterator.hpp"
#include "XML/Node.hpp"
#include "XML/Attribute.hpp"
#include "BVFitness.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
BVFitness::BVFitness() :
  mType(BVFitness::eUnknown)
{ }


/*!
 *
 */
double BVFitness::getFitnessSimple() const
{
  if(mType!=BVFitness::eSimple) throw std::runtime_error("The fitness is not of simple type!");
  return mFitnessValues.front();
}


/*!
 *
 */
const std::vector<double>& BVFitness::getFitnessMultiObj() const
{
  if(mType!=BVFitness::eMultiObj) throw std::runtime_error("The fitness is not multi-objective!");
  return mFitnessValues;
}


/*!
 *
 */
const std::vector<double>& BVFitness::getFitnessKoza() const
{
  if(mType!=BVFitness::eKoza) throw std::runtime_error("The fitness is not of Koza type!");
  return mFitnessValues;
}


/*!
 *
 */
void BVFitness::read(XML::ConstIterator inIter)
{
  mFitnessString.resize(0);
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  std::string lValid = inIter->getAttribute("valid");
  std::string lType = inIter->getAttribute("type");
  if(lValid=="no") mType=BVFitness::eInvalid;
  else {
    if(lType=="simple") {
      mType = BVFitness::eSimple;
      readSimple(inIter);
    }
    else if(lType == "multiobj") {
      mType = BVFitness::eMultiObj;
      readMultiObj(inIter);
    }
    else if(lType == "koza") {
      mType = BVFitness::eKoza;
      readKoza(inIter);
    }
    else {
      mType = BVFitness::eOther;
      mFitnessString = verbatimXML(XMLToString(inIter, false));
    }
  }
}


/*!
 *
 */
void BVFitness::readKoza(XML::ConstIterator inIter)
{
  mFitnessString.resize(0);
  std::string lNormalized = "0.0";
  std::string lAdjusted = "0.0";
  std::string lStandardized = "0.0";
  std::string lRaw = "0.0";
  std::string lHits = "0";
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Normalized") {
      XML::ConstIterator lChild2=lChild->getFirstChild();
      if((lChild2) && (lChild2->getType()==XML::eString)) {
        lNormalized = lChild2->getValue();
      }
    }
    else if(lChild->getValue() == "Adjusted") {
      XML::ConstIterator lChild2=lChild->getFirstChild();
      if((lChild2) && (lChild2->getType()==XML::eString)) {
        lAdjusted = lChild2->getValue();
      }
    }
    else if(lChild->getValue() == "Standardized") {
      XML::ConstIterator lChild2=lChild->getFirstChild();
      if((lChild2) && (lChild2->getType()==XML::eString)) {
        lStandardized = lChild2->getValue();
      }
    }
    else if(lChild->getValue() == "Raw") {
      XML::ConstIterator lChild2=lChild->getFirstChild();
      if((lChild2) && (lChild2->getType()==XML::eString)) {
        lRaw = lChild2->getValue();
      }
    }
    else if(lChild->getValue() == "Hits") {
      XML::ConstIterator lChild2=lChild->getFirstChild();
      if((lChild2) && (lChild2->getType()==XML::eString)) {
        lHits = lChild2->getValue();
      }
    }
  }
  std::ostringstream lOSS;
  lOSS << "(norm=" << lNormalized << ", adj=" << lAdjusted;
  lOSS << ", std=" << lStandardized << ", raw=" << lRaw;
  lOSS << ", hits=" << lHits << ")";
  mFitnessString = lOSS.str();
  
  mFitnessValues.resize(5, 0.0);
  mFitnessValues[0] = stringTo<double>(lNormalized);
  mFitnessValues[1] = stringTo<double>(lAdjusted);
  mFitnessValues[2] = stringTo<double>(lStandardized);
  mFitnessValues[3] = stringTo<double>(lRaw);
  mFitnessValues[4] = stringTo<double>(lHits);
}


/*!
 *
 */
void BVFitness::readMultiObj(XML::ConstIterator inIter)
{
  mFitnessString.resize(0);
  mFitnessValues.resize(0);
  std::ostringstream lOSS;
  bool lState = false;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()!=XML::eData) || (lChild->getValue()!="Obj")) continue;
    XML::ConstIterator lChild2=lChild->getFirstChild();
    if(lChild2->getType()!=XML::eString) continue;
    lOSS << (lState ? ',' : '(') << lChild2->getValue();
    lState = true;
    mFitnessValues.push_back(stringTo<double>(lChild2->getValue()));
  }
  if(lState) lOSS << ")";
  mFitnessString = lOSS.str();
}


/*!
 *
 */
void BVFitness::readSimple(XML::ConstIterator inIter)
{
  mFitnessString.resize(0);
  XML::ConstIterator lChild = inIter->getFirstChild();
  if((lChild==false) || (lChild->getType()!=XML::eString)) return;
  mFitnessString = lChild->getValue();
  mFitnessValues.resize(1);
  mFitnessValues.front() = stringTo<double>(lChild->getValue());
}


/*!
 *
 */
void BVFitness::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  if(mType!=BVFitness::eUnknown) ioStreamer.insertStringContent(mFitnessString);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();
}






