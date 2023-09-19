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
#include "XML/Streamer.hpp"
#include "BVRegister.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
void BVRegister::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Register") return;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Entry") {
      std::string lKey = lChild->getAttribute("key");
      if(lKey!="") {
        XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(lChild2) {
          if(lChild2->getType()==XML::eString) {
            mParameters[lKey] = lChild2->getValue();
          }
          else if(lChild2->getType()==XML::eData) {
            mParameters[lKey] = XMLToString(lChild2, false);
          }
          else mParameters[lKey];
        }
      }
    }
  }
}


/*!
 *
 */
void BVRegister::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  // Creater register representation
  std::ostringstream lOSS;
  XML::Streamer lStreamer(lOSS, 2);
  lStreamer.openTag("Register");
  for(std::map<std::string,std::string>::const_iterator lIter=mParameters.begin();
      lIter!=mParameters.end(); ++lIter) {
    lStreamer.openTag("Entry");
    lStreamer.insertAttribute("key", lIter->first);
    if(lIter->second.empty()==false) lStreamer.insertStringContent(lIter->second);
    lStreamer.closeTag(false);
  }
  lStreamer.closeTag();
  std::string lRegisterString = verbatimXML(lOSS.str());

  // Header
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("Register");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Content
  ioStreamer.openTag("center");
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "0");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("bgcolor", "#CCCCDD");
  ioStreamer.openTag("tt");
  ioStreamer.insertStringContent(lRegisterString);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioContext.writeFooter(ioStreamer);
}

