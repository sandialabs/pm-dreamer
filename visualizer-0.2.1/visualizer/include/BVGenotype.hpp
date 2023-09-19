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

#ifndef BVGenotype_hpp
#define BVGenotype_hpp

#include <string>
#include <vector>
#include "XML/Iterator.hpp"
#include "XML/Node.hpp"
#include "XML/Streamer.hpp"
#include "BVContextHTML.hpp"

/*!
 *
 */
class BVGenotype {

public:

  enum Type {eBitString, eFloatVector, eESVector, eGPTree, eOther, eUnknown};

  BVGenotype();
  ~BVGenotype() { }

  /*!
   *
   */
  inline BVGenotype::Type getType() const
  {
    return mType;
  }

  const std::vector<bool>&                      getBitString() const;
  const std::vector<double>&                    getFloatVector() const;
  const std::vector<std::pair<double,double> >& getESVector() const;
  const PACC::XML::Node&                        getGPTree() const;
  
  void read(PACC::XML::ConstIterator inIter);
  void write(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;

private:

  void readBitString(PACC::XML::ConstIterator inIter);
  void readESVector(PACC::XML::ConstIterator inIter);
  void readFloatVector(PACC::XML::ConstIterator inIter);
  void readGPTree(PACC::XML::ConstIterator inIter);
  void writeBitString(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  void writeESVector(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  void writeFloatVector(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  void writeGPTree(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;  
  void writeSExpression(std::ostream& ioOS, PACC::XML::ConstIterator inIter) const;

  std::string      mGenotypeString;
  std::string      mTypeString;
  BVGenotype::Type mType;

  std::vector<bool>                       mBitString;
  std::vector<double>                     mFloatVector;
  std::vector< std::pair<double,double> > mESVector;
  PACC::XML::Node                         mGPTree;

};

#endif // BVGenotype_hpp
