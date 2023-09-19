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

#include "XML/Iterator.hpp"
#include "XML/Node.hpp"
#include "BVStats.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
BVStats::BVStats() :
  mPopSize(0),
  mGeneration(0)
{ }


/*!
 *
 */
void BVStats::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Stats") return;
  mID = inIter->getAttribute("id");
  std::string lPopSizeAttr = inIter->getAttribute("popsize");
  if(lPopSizeAttr.empty() == false) mPopSize = stringTo<unsigned int>(lPopSizeAttr);
  std::string lGenAttr = inIter->getAttribute("generation");
  mGeneration = stringTo<unsigned int>(lGenAttr);
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType()!=XML::eData) continue;
    else if(lChild->getValue() == "Item") {
      std::string lKey = lChild->getAttribute("key");
      if(lKey.empty()==false) {
        XML::ConstIterator lChild2=lChild->getFirstChild();
        if((lChild2==false) || (lChild2->getType()!=XML::eString)) continue;
        mItems[lKey] = stringTo<double>(lChild2->getValue());
      }
    }
    else if(lChild->getValue() == "Measure") {
      std::string lID = lChild->getAttribute("id");
      if(lID.empty()==false) readMeasure(lChild, mMeasures[lID]);
    }    
  }
}


/*!
 *
 */
void BVStats::readMeasure(XML::ConstIterator inIter, BVStats::Measure& outMeasure)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Measure") return;
  outMeasure.mID = inIter->getAttribute("id");
  outMeasure.mGeneration = mGeneration;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType()!=XML::eData) continue;
    else if(lChild->getValue() == "Avg") {
      if(lChild->getFirstChild()==false) continue;
      outMeasure.mAvg = stringTo<double>(lChild->getFirstChild()->getValue());
    }
    else if(lChild->getValue() == "Std") {
      if(lChild->getFirstChild()==false) continue;
      outMeasure.mStd = stringTo<double>(lChild->getFirstChild()->getValue());
    }
    else if(lChild->getValue() == "Max") {
      if(lChild->getFirstChild()==false) continue;
      outMeasure.mMax = stringTo<double>(lChild->getFirstChild()->getValue());
    }
    else if(lChild->getValue() == "Min") {
      if(lChild->getFirstChild()==false) continue;
      outMeasure.mMin = stringTo<double>(lChild->getFirstChild()->getValue());
    }
  }
}


/*!
 *
 */
void BVStats::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Items Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Item");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Value");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent("generation");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(mGeneration));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent("popsize");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(mPopSize));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(std::map<std::string,double>::const_iterator lIter=mItems.begin();
      lIter!=mItems.end(); ++lIter) {
    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    bool lItemLinked = false;
    std::map< std::string,std::set<std::string> >::const_iterator lIterItem=
      ioContext.mItemsRead.find(mID);
    if(lIterItem != ioContext.mItemsRead.end()) {
      lItemLinked = (lIterItem->second.find(lIter->first) != lIterItem->second.end());
    }
    if(lItemLinked) {
      ioStreamer.openTag("a", false);
      std::ostringstream lOSSLink;
      lOSSLink << "stats.html#item_" << mID << "_" << lIter->first;
      ioStreamer.insertAttribute("href", lOSSLink.str());
    }
    ioStreamer.insertStringContent(lIter->first);
    if(lItemLinked) ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td", false);
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lIter->second));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();

  ioStreamer.openTag("br");
  ioStreamer.closeTag();

  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Measures Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Measure");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.openTag("b", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td", false);
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  for(std::map<std::string,BVStats::Measure>::const_iterator lIter=mMeasures.begin();
      lIter!=mMeasures.end(); ++lIter) {
    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    bool lMeasureLinked = false;
    std::map< std::string,std::set<std::string> >::const_iterator lIterMeasure=
      ioContext.mMeasuresRead.find(mID);
    if(lIterMeasure != ioContext.mMeasuresRead.end()) {
      lMeasureLinked = (lIterMeasure->second.find(lIter->first) != lIterMeasure->second.end());
    }
    if(lMeasureLinked) {
      ioStreamer.openTag("a", false);
      std::ostringstream lOSSLink;
      lOSSLink << "stats.html#measure_" << mID << "_" << lIter->first;
      ioStreamer.insertAttribute("href", lOSSLink.str());
    }
    ioStreamer.insertStringContent(lIter->first);
    if(lMeasureLinked) ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td", false);
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lIter->second.mAvg));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td", false);
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lIter->second.mStd));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td", false);
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lIter->second.mMax));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td", false);
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lIter->second.mMin));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.openTag("br");
  ioStreamer.closeTag(false);
  
  ioStreamer.closeTag();
}

