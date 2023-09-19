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

#ifndef BVDeme_hpp
#define BVDeme_hpp

#include "XML/Iterator.hpp"
#include "BVStats.hpp"
#include "BVHallOfFame.hpp"
#include "BVIndividual.hpp"

/*!
 *
 */
class BVDeme {

public:

  BVDeme() { }
  ~BVDeme() { }

  void read(PACC::XML::ConstIterator inIter);
  void writeTC(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  void writeContent(PACC::XML::Streamer& ioStreamer,
                    BVContextHTML& ioContext,
                    std::string inPrefix,
                    unsigned int inGeneration,
                    unsigned int inDemeIndex) const;

private:

  void computeMeanStdDev(const std::vector< std::vector<double> >& inData,
                         std::vector<double>& outMeans,
                         std::vector<double>& outStdDev) const;
  void computeCovar(const std::vector< std::vector<double> >& inData,
                    const std::vector<double>& inMeans,
                    std::vector< std::vector<double> >& outCovar) const;
  void writeAnalysisFitnessSimple(PACC::XML::Streamer& ioStreamer,
                                  BVContextHTML& ioContext,
                                  std::string inPrefix) const;
  void writeAnalysisFitnessMO(PACC::XML::Streamer& ioStreamer,
                              BVContextHTML& ioContext,
                              std::string inPrefix) const;
  void writeAnalysisFitnessKoza(PACC::XML::Streamer& ioStreamer,
                                BVContextHTML& ioContext,
                                std::string inPrefix) const;
  void writeAnalysisGenoBitString(PACC::XML::Streamer& ioStreamer,
                                  BVContextHTML& ioContext,
                                  std::string inPrefix,
                                  unsigned int inGenoIndex,
                                  bool inMultipleGeno) const;
  void writeAnalysisGenoFloatVector(PACC::XML::Streamer& ioStreamer,
                                    BVContextHTML& ioContext,
                                    std::string inPrefix,
                                    unsigned int inGenoIndex,
                                    bool inMultipleGeno) const;
  void writeAnalysisGenoES(PACC::XML::Streamer& ioStreamer,
                           BVContextHTML& ioContext,
                           std::string inPrefix,
                           unsigned int inGenoIndex,
                           bool inMultipleGeno) const;
  void writeAnalysisGenoGPTree(PACC::XML::Streamer& ioStreamer,
                               BVContextHTML& ioContext,
                               std::string inPrefix,
                               unsigned int inGeneration,
                               unsigned int inDemeIndex,
                               unsigned int inGenoIndex,
                               bool inMultipleGeno) const;
  void writePopulation(const std::vector<BVIndividual>& inPopulation,
                       PACC::XML::Streamer& ioStreamer,
                       BVContextHTML& ioContext) const;
      
  BVStats                   mStats;           
  BVHallOfFame              mHallOfFame;   
  std::vector<BVIndividual> mMigrationBuffer;  
  std::vector<BVIndividual> mPopulation;
  
};

#endif // BVDeme_hpp

