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

#include "BVIndividual.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
void BVIndividual::read(XML::ConstIterator inIter)
{
  mGenotypes.resize(0);
  if((inIter->getType()!=XML::eData) || (inIter->getValue()!="Individual")) return;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType()!=XML::eData) continue;
    if(lChild->getValue() == "Fitness") {
      mFitness.read(lChild);
    }
    else if(lChild->getValue() == "Genotype") {
      mGenotypes.resize(mGenotypes.size()+1);
      mGenotypes.back().read(lChild);
    }
  }
}


/*!
 *
 */
void BVIndividual::write(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  if(mFitness.getType() != BVFitness::eInvalid) {
    ioStreamer.insertStringContent("Fitness: ");
    mFitness.write(ioStreamer, ioContext);
  }
  if(mGenotypes.size() == 1) {
    ioStreamer.insertStringContent("Genotype: ");
    mGenotypes.front().write(ioStreamer, ioContext);
  }
  else {
    for(unsigned int i=0; i<mGenotypes.size(); ++i) {
      ioStreamer.insertStringContent(std::string("Genotype ")+toString(i+1)+std::string(": "));
      mGenotypes[i].write(ioStreamer, ioContext);
    }
  }
}


