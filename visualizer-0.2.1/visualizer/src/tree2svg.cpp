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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_LIBZ
#include "gzstream.h"
#endif // HAVE_LIBZ
 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "EPIG/EPIG.hpp"
#include "XML/Document.hpp"

using namespace std;
using namespace PACC;
using namespace EPIG;



void usage(char** argv);

/*!
 *
 */
struct Node {
  Node();
  ~Node();
  void build(const XML::Node& inNode);
  void draw(EPIG::Group& ioEPIG, double inX0, double inX1, double inY, double inGap) const;
  std::vector<Node*>       mChildren;
  std::vector<std::string> mLabels;
  unsigned int             mNbLeaves;
  unsigned int             mDepth;
};


/*!
 *
 */
int main(int argc, char **argv)
{
  if(argc>3) usage(argv);

  std::ostringstream lInXMLOSS;
  if(argc>1) {
    std::ifstream lInFile(argv[1]);
    for(int lChar=lInFile.get(); lInFile.good() && (lChar!=EOF); lChar=lInFile.get()) {
      if(lChar==(int)'&') {
        char lBuf[32];
        lInFile.getline(lBuf, 32, ';');
        if(lInFile.fail()) break;
        std::string lBufStr=lBuf;
        if(lBufStr=="quot") lInXMLOSS << '\"';
        else if(lBufStr=="amp") lInXMLOSS << '&';
        else if(lBufStr=="lt") lInXMLOSS << '<';
        else if(lBufStr=="gt") lInXMLOSS << '>';
      }
      else if(lChar==(int)'\\') {
        char lChar2=lInFile.get();
        if((lInFile.good()==false) || (lChar2==EOF)) break;
        if(lChar2==(int)'\"') lInXMLOSS << '\"';
        else lInXMLOSS << lChar << lChar2;
      }
      else lInXMLOSS << (char)lChar;
    }
  }
  else {
    for(int lChar=cin.get(); cin.good() && (lChar!=EOF); lChar=cin.get()) {
      if(lChar==(int)'&') {
        char lBuf[32];
        cin.getline(lBuf, 32, ';');
        if(cin.fail()) break;
        std::string lBufStr=lBuf;
        if(lBufStr=="quot") lInXMLOSS << '\"';
        else if(lBufStr=="amp") lInXMLOSS << '&';
        else if(lBufStr=="lt") lInXMLOSS << '<';
        else if(lBufStr=="gt") lInXMLOSS << '>';
      }
      else if(lChar==(int)'\\') {
        char lChar2=cin.get();
        if((cin.good()==false) || (lChar2==EOF)) break;
        if(lChar2==(int)'\"') lInXMLOSS << '\"';
        else lInXMLOSS << lChar << lChar2;
      }
      else lInXMLOSS << (char)lChar;
    }
  }
  
  std::istringstream lISS(lInXMLOSS.str());
  XML::Document lDocument;
  try {
    if(argc>1) lDocument.parse(lISS, argv[1]);
    else lDocument.parse(lISS, "Standard Input");
  }
  catch(std::exception& ioException) {
    if(argc>1) cerr << "Exception catched while parsing XML file \"" << argv[1] << "\":" << endl;
    else cerr << "Exception catched while parsing XML from standard input:" << endl;
    cerr << ioException.what() << endl;
    cerr << endl;
    exit(2);
  }

  Node lTree;
  const XML::Document lConstDoc = lDocument;
  lTree.build(*lConstDoc.getFirstDataTag());
  const double lWidth  = (90.0 * double(lTree.mNbLeaves)) + 20.0;
  const double lHeight = (60.0 * double(lTree.mDepth)) + 20.0;
  EPIG::Group lGroup;
  lTree.draw(lGroup, 10.0, (90.0 * double(lTree.mNbLeaves)), (lHeight - 25.0), 60.0);
  Frame lFrame(Point(0.0,0.0), Size(lWidth, lHeight));
  Rectangle lBackground(Point(0.0,0.0), Size(lWidth, lHeight), Fill(Color::cWhite));
  lFrame << lBackground << lGroup;

  SVGDocument lSVGDocument("SVG Tree", lFrame);
  if(argc>2) {
    ofstream lSVGOS(argv[2]);
    lSVGDocument.write(lSVGOS);
    lSVGOS.close();
  }
  else lSVGDocument.write(cout);
  
  return 0;
}


/*!
 *
 */
void usage(char** argv)
{
  cerr << argv[0] << ": XML Tree to SVG representation conversion" << endl;
  cerr << "usage> " << argv[0] << " [IN_XML_FILE [OUT_SVG_FILE]]" << endl;
  cerr << "If in/out files are omitted, standard input/output are used." << endl;
  cerr << endl;
  exit(1);
}


/*!
 *
 */
Node::Node() :
  mNbLeaves(0),
  mDepth(0)
{ }


/*!
 *
 */
Node::~Node()
{
  for(unsigned int i=0; i<mChildren.size(); ++i) {
    delete mChildren[i];
    mChildren[i] = NULL;
  }
  mChildren.resize(0);
}


/*!
 *
 */
void Node::build(const XML::Node& inNode)
{
  mNbLeaves = (inNode.getFirstChild() == true) ? 0 : 1;
  mDepth = 1;
  for(std::map<std::string,std::string>::const_iterator lAttIter=inNode.begin();
      lAttIter!=inNode.end(); ++lAttIter) {
    if(lAttIter->first == "") mLabels.push_back(lAttIter->second);
    else mLabels.push_back(lAttIter->first+std::string("=")+lAttIter->second);
    if(mLabels.back().size() > 12) mLabels.back().resize(12);
  }
  for(XML::ConstIterator lXMLIter=inNode.getFirstChild(); lXMLIter; ++lXMLIter) {
    mChildren.push_back(new Node);
    mChildren.back()->build(*lXMLIter);
    if(mChildren.back()->mDepth >= mDepth) mDepth = (mChildren.back()->mDepth+1);
    mNbLeaves += mChildren.back()->mNbLeaves;
  }
}


/*!
 *
 */
void Node::draw(EPIG::Group& ioEPIG, double inX0, double inX1, double inY, double inGap) const
{
  double lNodeX = 0.0;
  if(mChildren.empty() == false) {
    lNodeX = inX0;
    for(unsigned int i=0; i<(mChildren.size()/2); ++i) {
      lNodeX += ((double(mChildren[i]->mNbLeaves) / double(mNbLeaves)) * (inX1 - inX0));
    }
    if((mChildren.size()%2) == 1) {
      Node& lMedianNode = *mChildren[mChildren.size()/2];
      lNodeX += 0.5 * ((double(lMedianNode.mNbLeaves) / double(mNbLeaves)) * (inX1 - inX0));
    }
  }
  else lNodeX = 0.5 * (inX1 + inX0);
  double lChildX0 = inX0;
  for(unsigned int i=0; i<mChildren.size(); ++i) {
    const double lChildX1 = lChildX0 + ((double(mChildren[i]->mNbLeaves) / double(mNbLeaves)) * (inX1 - inX0));
    double lChildX = 0.0;
    if(mChildren[i]->mChildren.empty() == false) {
      lChildX = lChildX0;
      for(unsigned int j=0; j<(mChildren[i]->mChildren.size()/2); ++j) {
        lChildX += ((double(mChildren[i]->mChildren[j]->mNbLeaves) / double(mChildren[i]->mNbLeaves)) * (lChildX1 - lChildX0));
      }
      if((mChildren[i]->mChildren.size()%2) == 1) {
        Node& lMedianNode = *mChildren[i]->mChildren[mChildren[i]->mChildren.size()/2];
        lChildX += 0.5 * ((double(lMedianNode.mNbLeaves) / double(mChildren[i]->mNbLeaves)) * (lChildX1 - lChildX0));
      }
    }
    else lChildX = 0.5 * (lChildX1 + lChildX0);
    const double lChildY = inY - inGap;
    ioEPIG << Line(Point(lNodeX,inY), Point(lChildX,lChildY), Stroke(Color::cBlack));
    mChildren[i]->draw(ioEPIG, lChildX0, lChildX1, lChildY, inGap);
    lChildX0 = lChildX1;
  }
  unsigned int lMaxTextWidth=0;
  for(unsigned int i=0; i<mLabels.size(); ++i) {
    if(mLabels[i].size() > lMaxTextWidth) lMaxTextWidth = mLabels[i].size();
  }
  const double lBoxX0 = (lNodeX - (3.5 * lMaxTextWidth)) - 5.0;
  const double lBoxX1 = (lNodeX + (3.5 * lMaxTextWidth)) + 5.0;
  const double lBoxY0 = inY + 5.0;
  const double lBoxY1 = (inY - (15.0 * mLabels.size())) - 3.0;
  Polyline lBox = Point(lBoxX0-0.5, lBoxY0) +
                  Point(lBoxX1, lBoxY0) +
                  Point(lBoxX1, lBoxY1) +
                  Point(lBoxX0, lBoxY1) +
                  Point(lBoxX0, lBoxY0+0.5);
  ioEPIG << lBox;
  ioEPIG.addStyle(FillOpacity(1.0) + Fill(Color::cWhite) + Stroke(Color::cBlack));
  double lTextY = inY - 11.0;
  for(unsigned int i=0; i<mLabels.size(); ++i) {
    ioEPIG << Text(mLabels[i], Point(lNodeX,lTextY),
                   TextAnchor::cMiddle + Fill(Color::cBlack) + Stroke(Color::cBlack));
    lTextY -= 15.0;
  }
}


