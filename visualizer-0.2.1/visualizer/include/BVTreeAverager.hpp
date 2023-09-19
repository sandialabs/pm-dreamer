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

#ifndef BVTreeAverager_hpp
#define BVTreeAverager_hpp

#include "XML/Node.hpp"
#include "EPIG/EPIG.hpp"
#include <list>

/*!
 *
 */
class BVTreeAverager {

public:

  /*!
   *
   */
  class Node {
  public:
     Node();
    ~Node();

    void         addXMLTree(const PACC::XML::Node& inNode);
    unsigned int adjust();
    void         draw(EPIG::Group& ioEPIG,
                      double inPosX,
                      double inPosY,
                      double inOpenAngle,
                      double inCloseAngle,
                      double inLevelGap,
                      unsigned int inTotalCount,
                      unsigned int inActualLevel) const;
            
    std::list<BVTreeAverager::Node*> mChildren;
    unsigned int                     mCount;
    unsigned int                     mSize;
    unsigned int                     mNbLeaves;
  };

   BVTreeAverager() { }
  ~BVTreeAverager() { }

  void addXMLTree(const PACC::XML::Node& inTree) {mRoot.addXMLTree(inTree);}
  void draw(std::ostream& ioOS);
   
private:

  BVTreeAverager::Node mRoot;
      
};

#endif // BVTreeAverager_hpp
