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

#include <fstream>
#include "BVVivarium.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
BVVivarium::BVVivarium() :
  mGeneration(0)
{ }


/*!
 *
 */
void BVVivarium::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Vivarium") return;
  mPopulation.resize(0);
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Stats") mStats.read(lChild);
    else if(lChild->getValue() == "HallOfFame") mHallOfFame.read(lChild);
    else if(lChild->getValue() == "Population") {
      for(XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
        if(lChild2->getType() != XML::eData) continue;
        else if(lChild2->getValue() == "Deme") {
          mPopulation.resize(mPopulation.size()+1);
          mPopulation.back().read(lChild2);
        }
      }
    }
  }
}


/*!
 *
 */
void BVVivarium::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  // Header
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent(std::string("Vivarium of generation ") + toString(mGeneration));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Internal links to sections of the vivarium.
  ioStreamer.openTag("ul");

  ioStreamer.openTag("li");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", "#viva_stats");
  ioStreamer.insertStringContent("Vivarium Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);

  if(mHallOfFame.getSize() > 0) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#viva_hof");
    ioStreamer.insertStringContent("Vivarium Hall-of-Fame");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  if(mPopulation.size() == 1) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme");
    ioStreamer.insertStringContent("Deme");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    mPopulation.back().writeTC(ioStreamer, ioContext);
  }
  else if(mPopulation.size() > 1) {
    for(unsigned int i=0; i<mPopulation.size(); ++i) {
      std::string lDemeFileName = std::string("viva_") + toString(mGeneration) +
                                  std::string("_deme_") + toString(i+1) + std::string(".html");
      ioStreamer.openTag("li");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", lDemeFileName);
      ioStreamer.insertStringContent(std::string("Deme ")+toString(i+1));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);

      std::ofstream lDemeOFS(std::string(ioContext.mOutDir+std::string("/")+lDemeFileName).c_str());
      XML::Streamer lDemeStreamer(lDemeOFS);
      ioContext.writeHeader(lDemeStreamer);
      lDemeStreamer.openTag("center");
      lDemeStreamer.openTag("h2", false);
      lDemeStreamer.insertStringContent(std::string("Deme ") + toString(i+1) +
                                     std::string(" of generation ") + toString(mGeneration));
      lDemeStreamer.closeTag(false);
      lDemeStreamer.closeTag(false);
      lDemeStreamer.openTag("br", false);
      lDemeStreamer.closeTag(false);
      mPopulation[i].writeTC(lDemeStreamer, ioContext);
      lDemeStreamer.openTag("br", false);
      lDemeStreamer.closeTag(false);

      std::ostringstream lOSSPrefix;
      lOSSPrefix << "viva" << mGeneration << "_deme" << (i+1) << "_";
      mPopulation[i].writeContent(lDemeStreamer, ioContext, lOSSPrefix.str(), mGeneration, i);

      ioContext.writeFooter(lDemeStreamer);
      lDemeOFS.close();
    }
  }
  ioStreamer.closeTag();

  // Write content of vivarium
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h3", false);
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("name", "viva_stats");
  ioStreamer.insertStringContent("Vivarium Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  mStats.write(ioStreamer, ioContext);

  if(mHallOfFame.getSize() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "viva_hof");
    ioStreamer.insertStringContent("Vivarium Hall-of-Fame");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    mHallOfFame.write(ioStreamer, ioContext);
  }

  if(mPopulation.size()==1) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h2", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme");
    ioStreamer.insertStringContent("Deme");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    mPopulation.back().writeTC(ioStreamer, ioContext);
    std::ostringstream lOSSPrefix;
    lOSSPrefix << "viva" << mGeneration << "_deme_";
    mPopulation.back().writeContent(ioStreamer, ioContext, lOSSPrefix.str(), mGeneration, 0);
  }

  ioContext.writeFooter(ioStreamer);
}

