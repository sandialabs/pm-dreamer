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

#include <climits>
#include <cfloat>
#include <sstream>
#include <stdio.h>
#include "EPIG/EPIG.hpp"
#include "BVGraph.hpp"
#include "BVUtil.hpp"

using namespace EPIG;

/*!
 *
 */
BVGraph::BVGraph(std::string inTitle, std::string inXLabel, std::string inYLabel) :
  mTitle(inTitle),
  mXLabel(inXLabel),
  mYLabel(inYLabel)
{ }



/*!
 *
 */
void BVGraph::drawSimpleSerie(std::ostream& ioOS, const std::vector<BVGraph::SimpleSerie>& inSerie)
{
  if(inSerie.size()==0) return;

  // Define EPIG groups
  EPIG::Group lGroupConst;
  EPIG::Group lGroupGraduation;
  EPIG::Group lGroupLegend;
  EPIG::Group lGroupPolyline;

  // Axes
  Rectangle lRect(Point(0.0,0.0), Size(600.0,600.0), Fill(Color::cWhite));
  lGroupConst << lRect;
  Line lAxeX(Point(80.0,80.0), Point(531.0,80.0), Stroke(Color::cBlack));
  lGroupConst << lAxeX;
  Line lAxeY(Point(80.0,80.0), Point(80.0,531.0), Stroke(Color::cBlack));
  lGroupConst << lAxeY;

  // Title
  if(mTitle.empty()==false) {
    Text lTitle(mTitle, Point(300.0,570.0), TextAnchor::cMiddle);
    lGroupConst << lTitle;
  }

  // X-Y labels
  if(mXLabel.empty()==false) {
    Text lXLabel(mXLabel, Point(300.0,35.0), TextAnchor::cMiddle);
    lGroupConst << lXLabel;
  }
  if(mYLabel.empty()==false) {
    Text lYLabel(mYLabel, Point(0.0,0.0), TextAnchor::cMiddle);
    lYLabel.setTransform(Translate(35.0,300.0) + Rotate(90.0));
    lGroupConst << lYLabel;
  }

  // Compute min/max of series
  std::pair<double,double> lMin(DBL_MAX,DBL_MAX);
  std::pair<double,double> lMax(DBL_MIN,DBL_MIN);
  for(unsigned int i=0; i<inSerie.size(); ++i) {
    if(inSerie[i].mT < lMin.first)  lMin.first = inSerie[i].mT;
    if(inSerie[i].mT > lMax.first)  lMax.first = inSerie[i].mT;
    if(inSerie[i].mF < lMin.second) lMin.second = inSerie[i].mF;
    if(inSerie[i].mF > lMax.second) lMax.second = inSerie[i].mF;
  }

  // Adjust min/max of Y axis to round values
  double lDiffY=lMax.second-lMin.second;
  if((lMin.second >= 0.0) && (lMin.second < (0.4*lDiffY))) lMin.second=0.0;
  unsigned int lNbGradsY=2;
  if((lMin.second >= 0.0) && (lMax.second <= 1.0) && (lDiffY >= 0.4)) {
    lMin.second=0.0;
    lMax.second=1.0;
    lNbGradsY=5;
  }
  else if(lDiffY>300.0) {
    lMin.second = 100.0*std::floor(0.01*lMin.second);
    lMax.second = 100.0*std::ceil(0.01*lMax.second);
    int lTmp = (int)(lMax.second-lMin.second);
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>30.0) {
    lMin.second = 10.0*std::floor(0.1*lMin.second);
    lMax.second = 10.0*std::ceil(0.1*lMax.second);
    int lTmp = (int)(lMax.second-lMin.second);
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>3.0) {
    lMin.second = std::floor(lMin.second);
    lMax.second = std::ceil(lMax.second);
    int lTmp = (int)(10.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.3) {
    lMin.second = 0.1*std::floor(10.0*lMin.second);
    lMax.second = 0.1*std::ceil(10.0*lMax.second);
    int lTmp = (int)(100.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.03) {
    lMin.second = 0.01*std::floor(100.0*lMin.second);
    lMax.second = 0.01*std::ceil(100.0*lMax.second);
    int lTmp = (int)(1000.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.003) {
    lMin.second = 0.001*std::floor(1000.0*lMin.second);
    lMax.second = 0.001*std::ceil(1000.0*lMax.second);
    int lTmp = (int)(10000.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  unsigned int lNbGradsX=2;
  int lTmp = (int)(lMax.first-lMin.first);
  if((lTmp%5) == 0) lNbGradsX=5;
  else if((lTmp%4) == 0) lNbGradsX=4;
  else if((lTmp%6) == 0) lNbGradsX=6;
  else if((lTmp%3) == 0) lNbGradsX=3;
  else if((lTmp%7) == 0) lNbGradsX=7;

  // Draw graduations
  for(unsigned int i=0; i<=lNbGradsX; ++i) {
    double lTx  = (double(i) * 450.0 / double(lNbGradsX)) + 80.0;
    double lVal = (double(i) * (lMax.first-lMin.first) / double(lNbGradsX)) + lMin.first;
    lGroupGraduation << Line(Point(lTx,75.0), Point(lTx,80.0), Stroke(Color::cBlack));
    lGroupGraduation << Text(toString(lVal), Point(lTx,60.0), TextAnchor::cMiddle);
  }
  for(unsigned int i=0; i<=lNbGradsY; ++i) {
    double lTy  = (double(i) * 450.0 / double(lNbGradsY)) + 80.0;
    double lVal = (double(i) * (lMax.second-lMin.second) / double(lNbGradsY)) + lMin.second;
    lGroupGraduation << Line(Point(75.0,lTy), Point(80.0,lTy), Stroke(Color::cBlack));
    lGroupGraduation << Text(toString(lVal), Point(60.0,lTy-5.0), TextAnchor::cMiddle);
  }

  // Compute data transformations
  const double lRangeX = lMax.first  - lMin.first;
  const double lRangeY = lMax.second - lMin.second;
  double lScaleX=0.0;
  double lScaleY=0.0;
  if(lRangeX==0.0) {
    lScaleX  = 450.0;
    lScaleY  = 450.0 / lRangeY;
  }
  else if(lRangeY==0.0) {
    lScaleX  = 450.0 / lRangeX;
    lScaleY  = 450.0;
  }
  else {
    lScaleX  = 450.0 / lRangeX;
    lScaleY  = 450.0 / lRangeY;
  }
  const double lTransX = 80.0 - (lMin.first  * lScaleX);
  const double lTransY = 80.0 - (lMin.second * lScaleY);

  // Create EPIG point lists for mean, max and min
  PointList lPL;
  for(unsigned int i=0; i<inSerie.size(); ++i) {
    lPL += Point((float)((double(inSerie[i].mT) * lScaleX) + lTransX),
                 (float)((inSerie[i].mF * lScaleY) + lTransY));
  }

  // Draw polyline
  lGroupPolyline << Polyline(lPL, StrokeDashArray("0"));
  lGroupPolyline.addStyle(Stroke(Color::cBlack));

  // Finish set-up of groups
  lGroupConst.addStyle(FontSize(11) );
  lGroupGraduation.addStyle(FontSize(11) );
  lGroupPolyline.addStyle(FillOpacity(0));
  lGroupLegend.addStyle(FontSize(11) );

  // Create EPIG Frame and write draw to disk
  Frame lFrame(Point(0.0,0.0), Size(600.0,600.0));
  lFrame << lGroupConst << lGroupGraduation << lGroupPolyline;
  SVGDocument lDocument(mTitle, lFrame);
  lDocument.write(ioOS);
}


/*!
 *
 */
void BVGraph::drawStatsSerie(std::ostream& ioOS, const std::vector<BVGraph::StatsSerie>& inSerie)
{
  if(inSerie.size()==0) return;

  // Define EPIG groups
  EPIG::Group lGroupConst;
  EPIG::Group lGroupGraduation;
  EPIG::Group lGroupLegend;
  EPIG::Group lGroupPolyline;
  EPIG::Group lGroupStdDev;
  
  // Axes
  Rectangle lRect(Point(0.0,0.0), Size(600.0,600.0), Fill(Color::cWhite));
  lGroupConst << lRect;
  Line lAxeX(Point(80.0,80.0), Point(531.0,80.0), Stroke(Color::cBlack));
  lGroupConst << lAxeX;
  Line lAxeY(Point(80.0,80.0), Point(80.0,531.0), Stroke(Color::cBlack));
  lGroupConst << lAxeY;

  // Title
  if(mTitle.empty()==false) {
    Text lTitle(mTitle, Point(300.0,570.0), TextAnchor::cMiddle);
    lGroupConst << lTitle;
  }

  // X-Y labels
  if(mXLabel.empty()==false) {
    Text lXLabel(mXLabel, Point(300.0,35.0), TextAnchor::cMiddle);
    lGroupConst << lXLabel;
  }
  if(mYLabel.empty()==false) {
    Text lYLabel(mYLabel, Point(0.0,0.0), TextAnchor::cMiddle);
    lYLabel.setTransform(Translate(35.0,300.0) + Rotate(90.0));
    lGroupConst << lYLabel;
  }
  
  // Compute min/max of series
  std::pair<double,double> lMin(DBL_MAX,DBL_MAX);
  std::pair<double,double> lMax(DBL_MIN,DBL_MIN);
  for(unsigned int i=0; i<inSerie.size(); ++i) {
    if(inSerie[i].mT < lMin.first) lMin.first = inSerie[i].mT;
    if(inSerie[i].mT > lMax.first) lMax.first = inSerie[i].mT;
    if(inSerie[i].mMax < lMin.second) lMin.second = inSerie[i].mMax;
    if(inSerie[i].mMax > lMax.second) lMax.second = inSerie[i].mMax;
    if(inSerie[i].mMin < lMin.second) lMin.second = inSerie[i].mMin;
    if(inSerie[i].mMin > lMax.second) lMax.second = inSerie[i].mMin;
  }

  // Adjust min/max of Y axis to round values
  double lDiffY=lMax.second-lMin.second;
  if((lMin.second >= 0.0) && (lMin.second < (0.4*lDiffY))) lMin.second=0.0;
  unsigned int lNbGradsY=2;
  if((lMin.second >= 0.0) && (lMax.second <= 1.0) && (lDiffY >= 0.4)) {
    lMin.second=0.0;
    lMax.second=1.0;
    lNbGradsY=5;
  }
  else if(lDiffY>300.0) {
    lMin.second = 100.0*std::floor(0.01*lMin.second);
    lMax.second = 100.0*std::ceil(0.01*lMax.second);
    int lTmp = (int)(lMax.second-lMin.second);
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>30.0) {
    lMin.second = 10.0*std::floor(0.1*lMin.second);
    lMax.second = 10.0*std::ceil(0.1*lMax.second);
    int lTmp = (int)(lMax.second-lMin.second);
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>3.0) {
    lMin.second = std::floor(lMin.second);
    lMax.second = std::ceil(lMax.second);
    int lTmp = (int)(10.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.3) {
    lMin.second = 0.1*std::floor(10.0*lMin.second);
    lMax.second = 0.1*std::ceil(10.0*lMax.second);
    int lTmp = (int)(100.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.03) {
    lMin.second = 0.01*std::floor(100.0*lMin.second);
    lMax.second = 0.01*std::ceil(100.0*lMax.second);
    int lTmp = (int)(1000.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  else if(lDiffY>0.003) {
    lMin.second = 0.001*std::floor(1000.0*lMin.second);
    lMax.second = 0.001*std::ceil(1000.0*lMax.second);
    int lTmp = (int)(10000.0*(lMax.second-lMin.second));
    if((lTmp%5) == 0) lNbGradsY=5;
    else if((lTmp%4) == 0) lNbGradsY=4;
    else if((lTmp%6) == 0) lNbGradsY=6;
    else if((lTmp%3) == 0) lNbGradsY=3;
    else if((lTmp%7) == 0) lNbGradsY=7;
  }
  unsigned int lNbGradsX=2;
  int lTmp = (int)(lMax.first-lMin.first);
  if((lTmp%5) == 0) lNbGradsX=5;
  else if((lTmp%4) == 0) lNbGradsX=4;
  else if((lTmp%6) == 0) lNbGradsX=6;
  else if((lTmp%3) == 0) lNbGradsX=3;
  else if((lTmp%7) == 0) lNbGradsX=7;
  
  // Draw graduations
  for(unsigned int i=0; i<=lNbGradsX; ++i) {
    double lTx  = (double(i) * 450.0 / double(lNbGradsX)) + 80.0;
    double lVal = (double(i) * (lMax.first-lMin.first) / double(lNbGradsX)) + lMin.first;
    lGroupGraduation << Line(Point(lTx,75.0), Point(lTx,80.0), Stroke(Color::cBlack));
    lGroupGraduation << Text(toString(lVal), Point(lTx,60.0), TextAnchor::cMiddle);
  }
  for(unsigned int i=0; i<=lNbGradsY; ++i) {
    double lTy  = (double(i) * 450.0 / double(lNbGradsY)) + 80.0;
    double lVal = (double(i) * (lMax.second-lMin.second) / double(lNbGradsY)) + lMin.second;
    lGroupGraduation << Line(Point(75.0,lTy), Point(80.0,lTy), Stroke(Color::cBlack));
    lGroupGraduation << Text(toString(lVal), Point(60.0,lTy-5.0), TextAnchor::cMiddle);
  }

  // Compute data transformations
  const double lRangeX = lMax.first-lMin.first;
  const double lRangeY = lMax.second-lMin.second;
  double lScaleX=0.0;
  double lScaleY=0.0;
  if(lRangeX==0.0) {
    lScaleX  = 450.0;
    lScaleY  = 450.0 / lRangeY;
  }
  else if(lRangeY==0.0) {
    lScaleX  = 450.0 / lRangeX;
    lScaleY  = 450.0;
  }
  else {
    lScaleX  = 450.0 / lRangeX;
    lScaleY  = 450.0 / lRangeY;
  }
  const double lTransX = 80.0 - (lMin.first  * lScaleX);
  const double lTransY = 80.0 - (lMin.second * lScaleY);

  // Create EPIG point lists for mean, max and min
  PointList lMeanPL, lMaxPL, lMinPL;
  for(unsigned int i=0; i<inSerie.size(); ++i) {
    lMeanPL += Point((float)((double(inSerie[i].mT) * lScaleX) + lTransX),
                     (float)((inSerie[i].mMean * lScaleY) + lTransY));
    lMaxPL += Point((float)((double(inSerie[i].mT) * lScaleX) + lTransX),
                    (float)((inSerie[i].mMax * lScaleY) + lTransY));
    lMinPL += Point((float)((double(inSerie[i].mT) * lScaleX) + lTransX),
                    (float)((inSerie[i].mMin * lScaleY) + lTransY));
  }

  // Draw mean polyline
  lGroupPolyline << Polyline(lMeanPL, StrokeDashArray("0"));
  lGroupPolyline.addStyle(Stroke(Color::cBlack));

  // Draw max polyline
  lGroupPolyline << Polyline(lMaxPL, StrokeDashArray("11"));
  lGroupPolyline.addStyle(Stroke(Color::cBlack));
  // Draw min polyline
  lGroupPolyline << Polyline(lMinPL, StrokeDashArray("7"));
  lGroupPolyline.addStyle(Stroke(Color::cBlack));

  // Create point list for std deviation
  PointList lStdPL;
  for(unsigned int i=0; i<inSerie.size(); ++i) {
    lStdPL += Point((float)((double(inSerie[i].mT) * lScaleX) + lTransX),
                  (float)(((inSerie[i].mMean + inSerie[i].mStdDev) * lScaleY) + lTransY));
  }
  for(unsigned int i=inSerie.size(); i>0; --i) {
    lStdPL += Point((float)((double(inSerie[i-1].mT) * lScaleX) + lTransX),
                    (float)(((inSerie[i-1].mMean - inSerie[i-1].mStdDev) * lScaleY) + lTransY));
  }
  
  // Draw std deviation region  
  lGroupStdDev << Polyline(lStdPL, Stroke(Color::cNone));
  lGroupStdDev.addStyle(FillOpacity(0.1f) + Fill(Color::cGray));
  
  // Finish set-up of groups
  lGroupConst.addStyle(FontSize(11) );
  lGroupGraduation.addStyle(FontSize(11) );
  lGroupPolyline.addStyle(FillOpacity(0));
  lGroupLegend.addStyle(FontSize(11) );

  // Create EPIG Frame and write draw to disk
  Frame lFrame(Point(0.0,0.0), Size(600.0,600.0));
  lFrame << lGroupConst << lGroupGraduation << lGroupPolyline << lGroupStdDev;
  SVGDocument lDocument(mTitle, lFrame);
  lDocument.write(ioOS);
}




