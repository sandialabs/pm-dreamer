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

#ifndef BVLogger_hpp
#define BVLogger_hpp

#include <string>
#include <map>
#include <list>
#include <iostream>
#include <vector>
#include "XML/Iterator.hpp"
#include "XML/Streamer.hpp"
#include "BVStats.hpp"
#include "BVContextHTML.hpp"

/*!
 *
 */
class BVLogger {
  
public:

  struct Logs {
    std::list<BVStats>      mStats;
    std::list<std::string>  mMessages;
    unsigned int            mGeneration;

    Logs(unsigned int inGeneration=0) : mGeneration(inGeneration) { }
  };
  
  struct MeasureSerie {
    std::string                    mStatsID;
    std::string                    mMeasureID;
    std::vector<BVStats::Measure*> mMeasures;

    MeasureSerie(std::string inStatsID="",
                 std::string inMeasureID="") :
      mStatsID(inStatsID),
      mMeasureID(inMeasureID)
    { }
  };

  typedef std::pair<unsigned int,double> ItemPair;
  
  struct ItemSerie {
    std::string                     mStatsID;
    std::string                     mItemID;
    std::vector<BVLogger::ItemPair> mItems;

    ItemSerie(std::string inStatsID="",
              std::string inItemID="") :
      mStatsID(inStatsID),
      mItemID(inItemID)
    { }
  };
  
  BVLogger() { }
  ~BVLogger() { }

  void read(PACC::XML::ConstIterator inIter);
  void setContext(BVContextHTML& ioContext) const;
  void writeLogs(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  void writeStats(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;

private:

  void convertItemsToSeries(std::vector<BVLogger::ItemSerie>& outSeries) const;
  void convertMeasuresToSeries(std::vector<BVLogger::MeasureSerie>& outSeries) const;
  void mergeMessages(std::vector<std::string>& outMergedMessages) const;
  void writeItemSerie(PACC::XML::Streamer& ioStreamer,
                      const BVLogger::ItemSerie& inSerie,
                      BVContextHTML& ioContext) const;
  void writeItemSerieCSV(std::string inFilename, const BVLogger::ItemSerie& inSerie) const;
  void writeMeasureSerie(PACC::XML::Streamer& ioStreamer,
                         const BVLogger::MeasureSerie& inSerie,
                         BVContextHTML& ioContext) const;
  void writeMeasureSerieCSV(std::string inFilename, const BVLogger::MeasureSerie& inSerie) const;
      
  std::map<unsigned int,BVLogger::Logs> mLogs;
  
};

#endif // BVLogger_hpp
