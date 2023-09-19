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

#include <cassert>
#include <cmath>
#include "BVTreeAverager.hpp"

using namespace PACC;
using namespace EPIG;

#ifndef M_PI
#define M_PI 3.141592653
#endif // M_PI


/*!
 *
 */
BVTreeAverager::Node::Node() :
  mCount(0),
  mSize(0),
  mNbLeaves(0)
{ }


/*!
 *
 */
BVTreeAverager::Node::~Node()
{
  for(std::list<BVTreeAverager::Node*>::iterator lIter=mChildren.begin();
      lIter!=mChildren.end(); ++lIter) {
    delete *lIter;
    *lIter = NULL;
  }
  mChildren.clear();
}


/*!
 *
 */
void BVTreeAverager::Node::addXMLTree(const XML::Node& inNode)
{
  ++mCount;
  std::list<BVTreeAverager::Node*>::iterator lNodeIter = mChildren.begin();
  for(XML::ConstIterator lXMLIter=inNode.getFirstChild(); lXMLIter; ++lXMLIter) {
    if(lNodeIter == mChildren.end()) {
      mChildren.push_back(new BVTreeAverager::Node);
      --lNodeIter;
    }
    (*lNodeIter)->addXMLTree(*lXMLIter);
    ++lNodeIter;
  }
}


/*!
 *
 */
unsigned int BVTreeAverager::Node::adjust()
{
  unsigned int lMaxDepth = 1;
  mSize = 1;
  mNbLeaves = (mChildren.size() == 0) ? 1 : 0;
  for(std::list<BVTreeAverager::Node*>::const_iterator lIter=mChildren.begin();
      lIter!=mChildren.end(); ++lIter) {
    assert(*lIter != NULL);
    unsigned int lDepth = (*lIter)->adjust() + 1;
    if(lDepth > lMaxDepth) lMaxDepth = lDepth;
    mSize     += (*lIter)->mSize;
    mNbLeaves += (*lIter)->mNbLeaves;
  }
  return lMaxDepth;
}


/*!
 *
 */
void BVTreeAverager::Node::draw(EPIG::Group& ioEPIG,
                                double inPosX,
                                double inPosY,
                                double inOpenAngle,
                                double inCloseAngle,
                                double inLevelGap,
                                unsigned int inTotalCount,
                                unsigned int inActualLevel) const
{
  if(mChildren.empty()==false) {
    const double lStepAngle = (inCloseAngle - inOpenAngle) / double(mChildren.size());
    double lChildOpenAngle  = inOpenAngle;
    double lChildCloseAngle = inOpenAngle + lStepAngle;
    for(std::list<BVTreeAverager::Node*>::const_iterator lIter=mChildren.begin();
        lIter!=mChildren.end(); ++lIter) {
      const double lR         = inLevelGap * double(inActualLevel+1);
      const double lTheta     = 0.5 * (lChildOpenAngle + lChildCloseAngle);
      const double lChildPosX = 300.0 + (lR * std::cos(lTheta));
      const double lChildPosY = 300.0 + (lR * std::sin(lTheta));
      const double lIntensity = 0.98 - (0.98 * (double((*lIter)->mCount) / double(inTotalCount)));
      ioEPIG << Line(Point(inPosX,inPosY), Point(lChildPosX,lChildPosY), Stroke(Gray(lIntensity)));
      (*lIter)->draw(ioEPIG, lChildPosX, lChildPosY, lChildOpenAngle, lChildCloseAngle,
                     inLevelGap, inTotalCount, inActualLevel+1);
      lChildOpenAngle = lChildCloseAngle;
      lChildCloseAngle += lStepAngle;
    }
  }
  const double lIntensity = 0.98 - (0.98 * (double(mCount) / double(inTotalCount)));  
  ioEPIG << Circle(Point(inPosX,inPosY), 3.0, Fill(Gray(lIntensity)));
}


/*!
 *
 */
void BVTreeAverager::draw(std::ostream& ioOS)
{
  unsigned int lDepth=mRoot.adjust();
  if(lDepth < 17) lDepth = 17;
  EPIG::Group lTree;
  mRoot.draw(lTree, 300.0, 300.0, (0.5*M_PI), (2.5*M_PI), (280.0/(lDepth-1)), mRoot.mCount, 0);
  Frame lFrame(Point(0.0,0.0), Size(600.0,600.0));
  Rectangle lBackground(Point(0.0,0.0), Size(600.0,600.0), Fill(Color::cWhite));
  lFrame << lBackground << lTree;
  SVGDocument lDocument("SVG Average Tree", lFrame);
  lDocument.write(ioOS);
}






