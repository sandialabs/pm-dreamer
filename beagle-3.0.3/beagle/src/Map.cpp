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
 *  \file   beagle/src/Map.cpp
 *  \brief  Source code of class Map.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Compare equality of two maps.
 *  \param inRightObj Map to compare to this.
 *  \return True if maps are equals, false if not.
 *  \throw Beagle::BadCastException If compared maps Objects are not of the same type.
 *  \par Note:
 *    Returns true if the range [ 0, min(end(),inRightObj.end()) )
 *    are identical when compared element-by-element, and otherwise returns false.
 */
bool Map::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Map& lRightMap = castObjectT<const Map&>(inRightObj);
  Map::const_iterator lFirstIter1 = begin();
  unsigned int lSizeCompared = ( (size()<lRightMap.size()) ? size() : lRightMap.size() );
  Map::const_iterator lLastIter1 = begin();
  for(unsigned int i=0; i<lSizeCompared; ++i) ++lLastIter1;
  Map::const_iterator lFirstIter2 = lRightMap.begin();
  return std::equal(lFirstIter1, lLastIter1, lFirstIter2, IsEqualMapPairPredicate());
  Beagle_StackTraceEndM("bool Map::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Evaluate lexicographical order of two maps.
 *  \param inRightObj Map to compare to this.
 *  \return True if left map is less that the right one, false if not.
 *  \throw Beagle::BadCastException If compared map Objects are not of the same type.
 *  \par Note:
 *    Returns true if the actual object is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool Map::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Map& lRightMap = castObjectT<const Map&>(inRightObj);
  Map::const_iterator lFirstIter1 = begin();
  unsigned int lSizeCompared = ( (size()<lRightMap.size()) ? size() : lRightMap.size() );
  Map::const_iterator lLastIter1 = begin();
  for(unsigned int i=0; i<lSizeCompared; ++i) ++lLastIter1;
  Map::const_iterator lFirstIter2 = lRightMap.begin();
  Map::const_iterator lLastIter2  = lRightMap.begin();
  for(unsigned int i=0; i<lSizeCompared; ++i) ++lLastIter2;
  return std::lexicographical_compare(lFirstIter1,
                                      lLastIter1,
                                      lFirstIter2,
                                      lLastIter2,
                                      IsLessMapPairPredicate());
  Beagle_StackTraceEndM("bool Map::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a map from a XML node.
 *  \param inIter XML iterator to read the Map from.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Map::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Map"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Map> expected!");

  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Entry")) {
      string lKey = lChild->getAttribute("key").c_str();
      if(lKey.empty())
        throw Beagle_IOExceptionNodeM(*lChild, "no key attribute for actual entry!");
      if(find(lKey) == end()) {
        std::ostringstream lOSS;
        lOSS << "entry \"" << lKey << "\" doesn't exist in current map!" << std::flush;
        throw Beagle_IOExceptionNodeM(*lChild, lOSS.str().c_str());
      }
      PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
      (*this)[lKey]->read(lChild2);
    }
  }
  Beagle_StackTraceEndM("void Map::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a map into a XML streamer.
 *  \param ioStreamer XML streamer to read the Map to.
 *  \param inIndent Whether XML output should be indented.
 */
void Map::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Map", inIndent);
  for(const_iterator lIter = begin(); lIter != end(); ++lIter) {
    ioStreamer.openTag("Entry", inIndent);
    ioStreamer.insertAttribute("key", lIter->first);
    lIter->second->write(ioStreamer, inIndent);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Map::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

