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

#include "BVSeeds.hpp"

using namespace PACC;


/*!
 *
 */
void BVSeeds::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Seeds") return;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Individual") {
      mIndividuals.resize(mIndividuals.size()+1);
      mIndividuals.back().read(lChild);
    }
  }
}


/*!
 *
 */
void BVSeeds::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  // Header
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("Seeds");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Seeds content here

  ioContext.writeFooter(ioStreamer);
}

