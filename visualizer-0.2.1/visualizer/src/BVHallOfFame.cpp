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

#include "BVHallOfFame.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
void BVHallOfFame::read(XML::ConstIterator inIter)
{
  mMembers.resize(0);
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType()!=XML::eData) continue;
    if(lChild->getValue()=="Member") {
      mMembers.resize(mMembers.size()+1);
      if(lChild->getAttribute("generation").empty() == false) {
        mMembers.back().mGeneration = stringTo<unsigned int>(lChild->getAttribute("generation"));
      }
      if(lChild->getAttribute("deme").empty() == false) {
        mMembers.back().mDeme = stringTo<unsigned int>(lChild->getAttribute("deme"));
      }

      for(XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
        if(lChild2->getType() != XML::eData) continue;
        if(lChild2->getValue() == "Individual") {
          mMembers.back().mIndividual.read(lChild2);
        }
      }
    }
  }
}


/*!
 *
 */
void BVHallOfFame::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioStreamer.openTag("center");
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "0");
  ioStreamer.insertAttribute("width", "96%");
  for(unsigned int i=0; i<mMembers.size(); ++i) {
    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    if((i%2)==0) ioStreamer.insertAttribute("style", "background-color: #CCCCDD");
    else ioStreamer.insertAttribute("style", "background-color: #F5F5F5");
    std::ostringstream lOSS;
    lOSS << "Individual of deme " << (mMembers[i].mDeme+1) << ", generation " << mMembers[i].mGeneration;
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    mMembers[i].mIndividual.write(ioStreamer, ioContext);
    ioStreamer.closeTag();
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
}

