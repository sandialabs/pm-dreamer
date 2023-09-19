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

#include <fstream>
#include "XML/Iterator.hpp"
#include "XML/Node.hpp"
#include "BVGraph.hpp"
#include "BVLogger.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
void BVLogger::convertItemsToSeries(std::vector<BVLogger::ItemSerie>& outSeries) const
{
  typedef std::map<std::string,std::vector<BVLogger::ItemPair> > SMapItems;
  typedef std::map<std::string,SMapItems> BMapItems;
  BMapItems lBigMap;
  for(std::map<unsigned int,BVLogger::Logs>::const_iterator lIterLogs=mLogs.begin();
      lIterLogs!=mLogs.end(); ++lIterLogs) {
    for(std::list<BVStats>::const_iterator lIterStats=lIterLogs->second.mStats.begin();
        lIterStats!=lIterLogs->second.mStats.end(); ++lIterStats) {
      SMapItems& lTmpMap=lBigMap[lIterStats->mID];
      for(std::map<std::string,double>::const_iterator lIterItems=lIterStats->mItems.begin();
          lIterItems!=lIterStats->mItems.end(); ++lIterItems) {
        lTmpMap[lIterItems->first].push_back(BVLogger::ItemPair(lIterStats->mGeneration,
                                                                lIterItems->second));
      }
    }
  }
  outSeries.resize(0);
  for(BMapItems::const_iterator lIter1=lBigMap.begin(); lIter1!=lBigMap.end(); ++lIter1) {
    for(SMapItems::const_iterator lIter2=lIter1->second.begin();
        lIter2!=lIter1->second.end(); ++lIter2) {
      outSeries.push_back(BVLogger::ItemSerie(lIter1->first, lIter2->first));
      outSeries.back().mItems = lIter2->second;
    }
  }
}
  

/*!
 *
 */
void BVLogger::convertMeasuresToSeries(std::vector<BVLogger::MeasureSerie>& outSeries) const
{
  typedef std::map<std::string,std::vector<BVStats::Measure*> > SMapMeasures;
  typedef std::map<std::string,SMapMeasures> BMapMeasures;
  BMapMeasures lBigMap;
  for(std::map<unsigned int,BVLogger::Logs>::const_iterator lIterLogs=mLogs.begin();
      lIterLogs!=mLogs.end(); ++lIterLogs) {
    for(std::list<BVStats>::const_iterator lIterStats=lIterLogs->second.mStats.begin();
        lIterStats!=lIterLogs->second.mStats.end(); ++lIterStats) {
      SMapMeasures& lTmpMap=lBigMap[lIterStats->mID];
      for(std::map<std::string,BVStats::Measure>::const_iterator lIterMeasures=lIterStats->mMeasures.begin();
          lIterMeasures!=lIterStats->mMeasures.end(); ++lIterMeasures) {
        lTmpMap[lIterMeasures->first].push_back((BVStats::Measure*)&(lIterMeasures->second));
      }
    }
  }
  outSeries.resize(0);
  for(BMapMeasures::const_iterator lIter1=lBigMap.begin(); lIter1!=lBigMap.end(); ++lIter1) {
    for(SMapMeasures::const_iterator lIter2=lIter1->second.begin();
        lIter2!=lIter1->second.end(); ++lIter2) {
      outSeries.push_back(BVLogger::MeasureSerie(lIter1->first, lIter2->first));
      for(unsigned int i=0; i<lIter2->second.size(); ++i) {
        outSeries.back().mMeasures.push_back(lIter2->second[i]);
      }
    }
  }
}


/*!
 *
 */
void BVLogger::mergeMessages(std::vector<std::string>& outMergedMessages) const
{
  outMergedMessages.resize(0);
  for(std::map<unsigned int,BVLogger::Logs>::const_iterator lIter=mLogs.begin();
      lIter!=mLogs.end(); ++lIter) {
    outMergedMessages.insert(outMergedMessages.end(),
                             lIter->second.mMessages.begin(), lIter->second.mMessages.end());
  }
}


/*!
 *
 */
void BVLogger::read(XML::ConstIterator inIter) 
{
  if((inIter->getType()!=XML::eData) || (inIter->getValue()!="Logger")) return;
  BVLogger::Logs* lLog=new BVLogger::Logs(0);
  bool lGotFirstGen=false;
  unsigned int lGen=0;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()!=XML::eData) || (lChild->getValue()!="Log")) continue;
    XML::ConstIterator lChild2=lChild->getFirstChild();
    if(lChild2->getType() == XML::eString) lLog->mMessages.push_back(lChild2->getValue());
    else if(lChild2->getType() == XML::eData) {
      if(lChild2->getValue() == "Stats") {
        BVStats lStats;
        lStats.read(lChild2);
        if(lGotFirstGen==false) {
          lGen = lLog->mGeneration = lStats.mGeneration;
          BVLogger::Logs& lTmp = mLogs[lGen];
          lTmp.mStats.insert(lTmp.mStats.end(), lLog->mStats.begin(), lLog->mStats.end());
          lTmp.mMessages.insert(lTmp.mMessages.end(),
                                lLog->mMessages.begin(), lLog->mMessages.end());
          delete lLog;
          lLog = &lTmp;
          lGotFirstGen=true;
        }
        else if(lStats.mGeneration != lGen) lLog = &(mLogs[lGen]);
        lLog->mStats.push_back(lStats);
      }
      else lLog->mMessages.push_back(verbatimXML(XMLToString(lChild2, false)));
    }
  }
  if(lGotFirstGen==false) {
    BVLogger::Logs& lTmp = mLogs[0];
    lTmp.mStats.insert(lTmp.mStats.end(), lLog->mStats.begin(), lLog->mStats.end());
    lTmp.mMessages.insert(lTmp.mMessages.end(), lLog->mMessages.begin(), lLog->mMessages.end());
    delete lLog;
  }
  lLog = NULL;
}


/*!
 *
 */
void BVLogger::setContext(BVContextHTML& ioContext) const
{
  for(std::map<unsigned int,BVLogger::Logs>::const_iterator lIterMap=mLogs.begin();
      lIterMap!=mLogs.end(); ++lIterMap) {
    for(std::list<BVStats>::const_iterator lIterLogs=lIterMap->second.mStats.begin();
        lIterLogs!=lIterMap->second.mStats.end(); ++lIterLogs) {
      for(std::map<std::string,BVStats::Measure>::const_iterator lIterMeasures=lIterLogs->mMeasures.begin();
          lIterMeasures!=lIterLogs->mMeasures.end(); ++lIterMeasures) {
        ioContext.mMeasuresRead[lIterLogs->mID].insert(lIterMeasures->first);
      }
      for(std::map<std::string,double>::const_iterator lIterItems=lIterLogs->mItems.begin();
          lIterItems!=lIterLogs->mItems.end(); ++lIterItems) {
        ioContext.mItemsRead[lIterLogs->mID].insert(lIterItems->first);
      }
    }
  }
}


/*!
 *
 */
void BVLogger::writeLogs(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  // Header
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("Logs");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Content
  ioStreamer.openTag("center");
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "0");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("bgcolor", "#CCCCDD");

  std::vector<std::string> lMessages;
  mergeMessages(lMessages);
  for(unsigned int i=0; i<lMessages.size(); ++i) {
    const unsigned int lLine=i+1;
    std::ostringstream lOSS;
    if(lLine<10) lOSS << "00000" << lLine;
    else if(lLine<100) lOSS << "0000" << lLine;
    else if(lLine<1000) lOSS << "000" << lLine;
    else if(lLine<10000) lOSS << "00" << lLine;
    else if(lLine<100000) lOSS << "0" << lLine;
    ioStreamer.openTag("font");
    ioStreamer.insertAttribute("color", "#4B66D0");
    ioStreamer.openTag("tt", false);
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("&nbsp;&nbsp;&nbsp;&nbsp;");
    ioStreamer.insertStringContent(verbatimXML(lMessages[i]));
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }

  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioContext.writeFooter(ioStreamer);
}


/*!
 *
 */
void BVLogger::writeStats(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  // Header
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Write internal links to different statistics.
  std::vector<BVLogger::MeasureSerie> lMeasures;
  std::vector<BVLogger::ItemSerie> lItems;
  convertMeasuresToSeries(lMeasures);
  convertItemsToSeries(lItems);
  ioStreamer.openTag("ul");
  for(unsigned int i=0; i<lMeasures.size(); ++i) {
    std::ostringstream lOSS1;
    lOSS1 << "#measure_" << lMeasures[i].mStatsID << "_" << lMeasures[i].mMeasureID;
    std::ostringstream lOSS2;
    lOSS2 << "Measure \'" << lMeasures[i].mMeasureID;
    lOSS2 << "\' of statistics \'" << lMeasures[i].mStatsID << "\'";
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lOSS1.str());
    ioStreamer.insertStringContent(lOSS2.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  for(unsigned int i=0; i<lItems.size(); ++i) {
    std::ostringstream lOSS1;
    lOSS1 << "#item_" << lItems[i].mStatsID << "_" << lItems[i].mItemID;
    std::ostringstream lOSS2;
    lOSS2 << "Item \'" << lItems[i].mItemID;
    lOSS2 << "\' of statistics \'" << lItems[i].mStatsID << "\'";
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lOSS1.str());
    ioStreamer.insertStringContent(lOSS2.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);


  // Write measure series themselves.
  for(unsigned int i=0; i<lMeasures.size(); ++i) {
    std::ostringstream lOSS1;
    lOSS1 << "measure_" << lMeasures[i].mStatsID << "_" << lMeasures[i].mMeasureID;
    std::ostringstream lOSS2;
    lOSS2 << "Measure \'" << lMeasures[i].mMeasureID;
    lOSS2 << "\' of statistics \'" << lMeasures[i].mStatsID << "\'";
    ioStreamer.openTag("a");
    ioStreamer.insertAttribute("name", lOSS1.str());
    ioStreamer.openTag("center", false);
    ioStreamer.openTag("h2", false);
    ioStreamer.insertStringContent(lOSS2.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writeMeasureSerie(ioStreamer, lMeasures[i], ioContext);
    ioStreamer.openTag("br");
    ioStreamer.closeTag(false);
    if((i!=(lMeasures.size()-1)) || (lItems.size()!=0)) {
      ioStreamer.openTag("hr", false);
      ioStreamer.closeTag(false);
      ioStreamer.openTag("br", false);
      ioStreamer.closeTag(false);
    }
  }

  // Write item series themselves.
  for(unsigned int i=0; i<lItems.size(); ++i) {
    std::ostringstream lOSS1;
    lOSS1 << "item_" << lItems[i].mStatsID << "_" << lItems[i].mItemID;
    std::ostringstream lOSS2;
    lOSS2 << "Item \'" << lItems[i].mItemID;
    lOSS2 << "\' of statistics \'" << lItems[i].mStatsID << "\'";
    ioStreamer.openTag("a");
    ioStreamer.insertAttribute("name", lOSS1.str());
    ioStreamer.openTag("center", false);
    ioStreamer.openTag("h2", false);
    ioStreamer.insertStringContent(lOSS2.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writeItemSerie(ioStreamer, lItems[i], ioContext);
    ioStreamer.openTag("br");
    ioStreamer.closeTag(false);
    if(i!=(lItems.size()-1)) {
      ioStreamer.openTag("hr", false);
      ioStreamer.closeTag(false);
      ioStreamer.openTag("br", false);
      ioStreamer.closeTag(false);
    }
  }

  ioContext.writeFooter(ioStreamer);
}


/*!
 *
 */
void BVLogger::writeItemSerie(XML::Streamer& ioStreamer,
                              const BVLogger::ItemSerie& inSerie,
                              BVContextHTML& ioContext) const
{
  // Write CSV file
  std::string lCSVName = std::string("item_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mItemID + std::string(".csv");
  writeItemSerieCSV(ioContext.mOutDir+std::string("/")+lCSVName, inSerie);

  // Convert serie to SVG graph format
  std::vector<BVGraph::SimpleSerie> lSerie(inSerie.mItems.size());
  for(unsigned int i=0; i<lSerie.size(); ++i) {
    lSerie[i].mT = inSerie.mItems[i].first;
    lSerie[i].mF = inSerie.mItems[i].second;

  }

  // Write SVG file with graph
#ifdef HAVE_LIBZ
  std::string lGraphName = std::string("item_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mItemID + std::string(".svgz");
#else  // HAVE_LIBZ
  std::string lGraphName = std::string("item_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mItemID + std::string(".svg");
#endif // HAVE_LIBZ
  BVGraph lGraph("", "generation", inSerie.mItemID);
#ifdef HAVE_LIBZ
  ogzstream lSVGOS(std::string(ioContext.mOutDir+std::string("/")+lGraphName).c_str());
#else  // HAVE_LIBZ
  std::ofstream lSVGOS(std::string(ioContext.mOutDir+std::string("/")+lGraphName).c_str());
#endif // HAVE_LIBZ
  lGraph.drawSimpleSerie(lSVGOS, lSerie);
  lSVGOS.close();

  // Write XHTML output for the serie
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lGraphName);
  ioStreamer.insertStringContent("Graphic");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]&nbsp;&nbsp;[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVName);
  ioStreamer.insertStringContent("Data");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("object");
  ioStreamer.insertAttribute("data", lGraphName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.openTag("embed");
  ioStreamer.insertAttribute("src", lGraphName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
}


/*!
 *
 */
void BVLogger::writeItemSerieCSV(std::string inFilename, const BVLogger::ItemSerie& inSerie) const
{
  std::ofstream lOFS(inFilename.c_str());
  lOFS << "\"GENERATION\",\"VALUE\"" << std::endl;
  for(unsigned int i=0; i<inSerie.mItems.size(); ++i) {
    lOFS << inSerie.mItems[i].first << ",";
    lOFS << inSerie.mItems[i].second << std::endl;
  }
  lOFS.close();
}


/*!
 *
 */
void BVLogger::writeMeasureSerie(XML::Streamer& ioStreamer,
                                 const BVLogger::MeasureSerie& inSerie,
                                 BVContextHTML& ioContext) const
{
  // Write CSV file
  std::string lCSVName = std::string("measure_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mMeasureID + std::string(".csv");
  writeMeasureSerieCSV(ioContext.mOutDir+std::string("/")+lCSVName, inSerie);

  // Convert serie to SVG graph format
  std::vector<BVGraph::StatsSerie> lSerie(inSerie.mMeasures.size());
  for(unsigned int i=0; i<lSerie.size(); ++i) {
    lSerie[i].mT      = inSerie.mMeasures[i]->mGeneration;
    lSerie[i].mMean   = inSerie.mMeasures[i]->mAvg;
    lSerie[i].mStdDev = inSerie.mMeasures[i]->mStd;
    lSerie[i].mMax    = inSerie.mMeasures[i]->mMax;
    lSerie[i].mMin    = inSerie.mMeasures[i]->mMin;
  }

  // Write SVG file with graph
#ifdef HAVE_LIBZ
  std::string lGraphName = std::string("measure_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mMeasureID + std::string(".svgz");
#else  // HAVE_LIBZ
  std::string lGraphName = std::string("measure_") + inSerie.mStatsID +
                           std::string("_") + inSerie.mMeasureID + std::string(".svg");
#endif // HAVE_LIBZ
  BVGraph lGraph("", "generation", inSerie.mMeasureID);
#ifdef HAVE_LIBZ
  ogzstream lSVGOS(std::string(ioContext.mOutDir+std::string("/")+lGraphName).c_str());
#else  // HAVE_LIBZ
  std::ofstream lSVGOS(std::string(ioContext.mOutDir+std::string("/")+lGraphName).c_str());
#endif // HAVE_LIBZ
  lGraph.drawStatsSerie(lSVGOS, lSerie);
  lSVGOS.close();

  // Write XHTML output for the serie
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lGraphName);
  ioStreamer.insertStringContent("Graphic");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]&nbsp;&nbsp;[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVName);
  ioStreamer.insertStringContent("Data");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("object");
  ioStreamer.insertAttribute("data", lGraphName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.openTag("embed");
  ioStreamer.insertAttribute("src", lGraphName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
}


/*!
 *
 */
void BVLogger::writeMeasureSerieCSV(std::string inFilename, const BVLogger::MeasureSerie& inSerie) const
{
  std::ofstream lOFS(inFilename.c_str());
  lOFS << "\"GENERATION\",\"MEAN\",\"STDDEV\",\"MAX\",\"MIN\"" << std::endl;
  for(unsigned int i=0; i<inSerie.mMeasures.size(); ++i) {
    lOFS << inSerie.mMeasures[i]->mGeneration << ",";
    lOFS << inSerie.mMeasures[i]->mAvg << ",";
    lOFS << inSerie.mMeasures[i]->mStd << ",";
    lOFS << inSerie.mMeasures[i]->mMax << ",";
    lOFS << inSerie.mMeasures[i]->mMin << std::endl;
  }
  lOFS.close();
}


