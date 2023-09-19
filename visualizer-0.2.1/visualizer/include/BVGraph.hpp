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

#ifndef BVGraph_hpp
#define BVGraph_hpp

#include <vector>
#include "EPIG/EPIG.hpp"
#include "BVLogger.hpp"


/*!
 *
 */
class BVGraph
{
  
 public:

  /*!
   *
   */
  struct SimpleSerie {
    unsigned int mT;
    double       mF;

    SimpleSerie(unsigned int inT=0, double inF=0.0) : mT(inT), mF(inF) { }
  };

  /*!
   *
   */
  struct StatsSerie {
    unsigned int mT;
    double       mMean;
    double       mStdDev;
    double       mMax;
    double       mMin;

    StatsSerie(unsigned int inT=0,
                double inMean=0.0,
                double inStdDev=0.0,
                double inMax=0.0,
                double inMin=0.0) :
      mT(inT), mMean(inMean), mStdDev(inStdDev), mMax(inMax), mMin(inMin)
    { }
  };

 
  BVGraph(std::string inTitle="", std::string inXLabel="", std::string inYLabel="");

  void drawSimpleSerie(std::ostream& ioOS, const std::vector<BVGraph::SimpleSerie>& inSerie);
  void drawStatsSerie(std::ostream& ioOS, const std::vector<BVGraph::StatsSerie>& inSerie);
  
private:

  std::string mTitle;
  std::string mXLabel;
  std::string mYLabel;
  
};


#endif // BVGraph_hpp

