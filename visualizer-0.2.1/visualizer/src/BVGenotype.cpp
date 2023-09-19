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

#include <cfloat>
#include <stdexcept> 
#include "BVGenotype.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
BVGenotype::BVGenotype() :
  mType(BVGenotype::eUnknown)
{ }


/*!
 *
 */
const std::vector<bool>& BVGenotype::getBitString() const
{
  if(mType!=BVGenotype::eBitString) throw std::runtime_error("The genotype is not a bit string!");
  return mBitString;
}


/*!
 *
 */
const std::vector<double>& BVGenotype::getFloatVector() const
{
  if(mType!=BVGenotype::eFloatVector) throw std::runtime_error("The genotype is not a float vector!");
  return mFloatVector;
}


/*!
 *
 */
const std::vector< std::pair<double,double> >& BVGenotype::getESVector() const
{
  if(mType!=BVGenotype::eESVector) throw std::runtime_error("The genotype is not an ES vector!");
  return mESVector;
}


/*!
 *
 */
const XML::Node& BVGenotype::getGPTree() const
{
  if(mType!=BVGenotype::eGPTree) throw std::runtime_error("The genotype is not a GP tree!");
  return mGPTree;
}


/*!
 *
 */
void BVGenotype::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  mTypeString = inIter->getAttribute("type");
  if(mTypeString == "bitstring") {
    mType = BVGenotype::eBitString;
    readBitString(inIter);
  }
  else if(mTypeString == "floatvector") {
    mType = BVGenotype::eFloatVector;
    readFloatVector(inIter);
  }
  else if((mTypeString=="ESVector") || (mTypeString=="esvector")) {
    mType = BVGenotype::eESVector;
    readESVector(inIter);
  }
  else if(mTypeString == "gptree") {
    mType = BVGenotype::eGPTree;
    readGPTree(inIter);
  }
  else {
    mType = BVGenotype::eOther;
    mGenotypeString = verbatimXML(XMLToString(inIter, false));
  }
}


/*!
 *
 */
void BVGenotype::readBitString(XML::ConstIterator inIter)
{
  mBitString.resize(0);
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getFirstChild()->getType()!=XML::eString) return;
  mGenotypeString = inIter->getFirstChild()->getValue();
  mBitString.resize(mGenotypeString.size());
  for(unsigned int i=0; i<mGenotypeString.size(); ++i) {
    if(mGenotypeString[i] == '1') mBitString[i] = true;
    else mBitString[i] = false;
  }
}


/*!
 *
 */
void BVGenotype::readFloatVector(XML::ConstIterator inIter)
{
  mFloatVector.resize(0);
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getFirstChild()->getType()!=XML::eString) return;
  std::istringstream lISS(inIter->getFirstChild()->getValue());
  while(lISS.good()) {
    double lValue=DBL_MAX;
    lISS >> lValue;
    mFloatVector.push_back(lValue);
    if(lISS.good()==false) break;
    int lDelim=lISS.get();
    if((lISS.good()==false) || (lDelim==-1)) break;
  }
  std::ostringstream lOSS;
  lOSS << "(";
  for(unsigned int i=0; i<mFloatVector.size(); ++i) {
    lOSS << mFloatVector[i];
    if(i==(mFloatVector.size()-1)) lOSS << ")";
    else lOSS << ",";
  }
  mGenotypeString = lOSS.str();
}


/*!
 *
 */
void BVGenotype::readESVector(XML::ConstIterator inIter)
{
  mESVector.resize(0);
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getFirstChild()->getType()!=XML::eString) return;
  std::istringstream lISS(inIter->getFirstChild()->getValue());
  char lCharBuf1='\0', lCharBuf2='\0', lCharBuf3='\0', lCharBuf4='/';
  double lFloatBuf1=0.0, lFloatBuf2=0.0;
  for(lISS >> lCharBuf1 >> lFloatBuf1 >> lCharBuf2 >> lFloatBuf2 >> lCharBuf3; lISS.good();
      lISS >> lCharBuf4 >> lCharBuf1 >> lFloatBuf1 >> lCharBuf2 >> lFloatBuf2 >> lCharBuf3) {
    if((lCharBuf1!='(') || (lCharBuf2!=',') || (lCharBuf3!=')') || (lCharBuf4!='/')) break;
    lCharBuf1=lCharBuf2=lCharBuf3=lCharBuf4='\0';
    mESVector.push_back(std::pair<double,double>(lFloatBuf1,lFloatBuf2));
  }
  std::ostringstream lOSS;
  lOSS << "Values=(";
  for(unsigned int i=0; i<mESVector.size(); ++i) {
    lOSS << mESVector[i].first;
    if(i==(mESVector.size()-1)) lOSS << ")";
    else lOSS << ",";
  }
  lOSS << ", Sigmas=(";
  for(unsigned int i=0; i<mESVector.size(); ++i) {
    lOSS << mESVector[i].second;
    if(i==(mESVector.size()-1)) lOSS << ")";
    else lOSS << ",";
  }
  mGenotypeString = lOSS.str();
}


/*!
 *
 */
void BVGenotype::readGPTree(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getFirstChild()->getType()!=XML::eData) return;
  XML::ConstIterator lChild = inIter->getFirstChild();
  mGPTree = *lChild;
  std::ostringstream lOSS;
  writeSExpression(lOSS, lChild);
  mGenotypeString = lOSS.str();
}


/*!
 *
 */
void BVGenotype::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  switch(mType) {
    case BVGenotype::eBitString: writeBitString(ioStreamer, ioContext); break;
    case BVGenotype::eFloatVector: writeFloatVector(ioStreamer, ioContext); break;
    case BVGenotype::eESVector: writeESVector(ioStreamer, ioContext); break;
    case BVGenotype::eGPTree: writeGPTree(ioStreamer, ioContext); break;
    case BVGenotype::eOther: ioStreamer.insertStringContent(mGenotypeString); break;
    default: break;
  }
}


/*!
 *
 */
void BVGenotype::writeBitString(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.insertStringContent(mGenotypeString);
}


/*!
 *
 */
void BVGenotype::writeESVector(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.insertStringContent(mGenotypeString);
}


/*!
 *
 */
void BVGenotype::writeFloatVector(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.insertStringContent(mGenotypeString);
}


/*!
 *
 */
void BVGenotype::writeGPTree(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.insertStringContent(mGenotypeString);
  ioStreamer.openTag("form");
  ioStreamer.insertAttribute("method", "post");
  ioStreamer.insertAttribute("action", ioContext.mBaseURL+std::string("/svgtree.php"));
  ioStreamer.openTag("input", false);
  ioStreamer.insertAttribute("type", "hidden");
  ioStreamer.insertAttribute("name", "data");
  std::ostringstream lOSS1;
  XML::Streamer lStreamer(lOSS1, 0);
  mGPTree.serialize(lStreamer, 0);
  std::string lIndivStr = lOSS1.str();
  std::ostringstream lOSS2;
  for(unsigned int i=0; i<lIndivStr.size(); ++i) {
    switch(lIndivStr[i]) {
    case '\"': lOSS2 << "&quot;"; break;
    case '\n': break;
    default: lOSS2 << lIndivStr[i]; break;
    }
  }
  ioStreamer.insertAttribute("value", lOSS2.str());
  ioStreamer.closeTag();
  ioStreamer.openTag("input");
  ioStreamer.insertAttribute("type", "submit");
  ioStreamer.insertAttribute("value", "View Tree");
  ioStreamer.closeTag();
  ioStreamer.closeTag();
}

  
/*!
 *
 */
void BVGenotype::writeSExpression(std::ostream& ioOS, XML::ConstIterator inIter) const
{
  if(inIter->getType() != XML::eData) return;
  if(inIter->getFirstChild() != false) ioOS << '(';
  ioOS << inIter->getValue();

  XML::AttributeList::const_iterator lAttrIter=inIter->begin();
  if(++lAttrIter != inIter->end()) {
    ioOS << '[';
    while(lAttrIter!=inIter->end()) {
      ioOS << lAttrIter->first << "=\"" << lAttrIter->second << '\"';
      if(++lAttrIter != inIter->end()) ioOS << ',';
      else ioOS << ']';
    }
  }
  
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    ioOS << ' ';
    writeSExpression(ioOS, lChild);
  }
  if(inIter->getChildCount() != 0) ioOS << ')';
}



