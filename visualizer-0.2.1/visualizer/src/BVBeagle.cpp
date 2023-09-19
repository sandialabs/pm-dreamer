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
#include <stdexcept>
#include "XML/Document.hpp"
#include "XML/Iterator.hpp"
#include "BVBeagle.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
BVBeagle::BVBeagle() :
  mLoggerRead(false),
  mEvolverRead(false),
  mRegisterRead(false),
  mVivariumRead(false),
  mSeedsRead(false)
{ }


/*!
 *
 */
void BVBeagle::generateReport(BVContextHTML& ioContext, std::ostream& inLogOS) const
{
  // Setting context flags
  ioContext.mMeasuresRead.clear();
  ioContext.mItemsRead.clear();
  if(mLoggerRead) {
    mLogger.setContext(ioContext);
    ioContext.mLogsState  = BVContextHTML::eActive;
    ioContext.mStatsState = BVContextHTML::eActive;
  }
  else {
    ioContext.mLogsState  = BVContextHTML::eInactive;
    ioContext.mStatsState = BVContextHTML::eInactive;
  }
  if(mEvolverRead) ioContext.mEvolverState = BVContextHTML::eActive;
  else ioContext.mEvolverState = BVContextHTML::eInactive;
  if(mRegisterRead) ioContext.mRegisterState = BVContextHTML::eActive;
  else ioContext.mRegisterState = BVContextHTML::eInactive;
  if(mVivariumRead) ioContext.mVivariumState = BVContextHTML::eActive;
  else ioContext.mVivariumState = BVContextHTML::eInactive;
  if(mSeedsRead) ioContext.mSeedsState = BVContextHTML::eActive;
  else ioContext.mSeedsState = BVContextHTML::eInactive;

  // Writing index.html file
  std::string lIndexFile = ioContext.mOutDir + "/index.html";
  inLogOS << "Generating index file \"" << lIndexFile << "\".." << std::flush;
  std::ofstream lIndexOFS(lIndexFile.c_str());
  if(lIndexOFS.good()==false) {
    inLogOS << "failed" << std::endl;
    std::ostringstream lOSS;
    lOSS << "Unable to write file \"" << lIndexFile << "\"!";
    throw std::runtime_error(lOSS.str());
  }
  XML::Streamer lIndexStreamer(lIndexOFS);
  writeIndex(lIndexStreamer, ioContext);
  lIndexOFS.close();
  inLogOS << "done" << std::endl;

  if(mLoggerRead) {
    // Writing logs.html file
    std::string lLogsFile = ioContext.mOutDir + "/logs.html";
    inLogOS << "Generating logs file \"" << lLogsFile << "\".." << std::flush;
    std::ofstream lLogsOFS(lLogsFile.c_str());
    if(lLogsOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lLogsFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mLogsState = BVContextHTML::eInProcess;
    XML::Streamer lLogsStreamer(lLogsOFS);
    mLogger.writeLogs(lLogsStreamer, ioContext);
    ioContext.mLogsState = BVContextHTML::eActive;
    lLogsOFS.close();
    inLogOS << "done" << std::endl;

    // Writing stats.html file
    std::string lStatsFile = ioContext.mOutDir + "/stats.html";
    inLogOS << "Generating stats file \"" << lStatsFile << "\".." << std::flush;
    std::ofstream lStatsOFS(lStatsFile.c_str());
    if(lStatsOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lStatsFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mStatsState = BVContextHTML::eInProcess;
    XML::Streamer lStatsStreamer(lStatsOFS);
    mLogger.writeStats(lStatsStreamer, ioContext);
    ioContext.mStatsState = BVContextHTML::eActive;
    lStatsOFS.close();
    inLogOS << "done" << std::endl;
  }

  if(mEvolverRead) {
    // Writing evolver.html file
    std::string lEvolverFile = ioContext.mOutDir + "/evolver.html";
    inLogOS << "Generating evolver file \"" << lEvolverFile << "\".." << std::flush;
    std::ofstream lEvolverOFS(lEvolverFile.c_str());
    if(lEvolverOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lEvolverFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mEvolverState = BVContextHTML::eInProcess;
    XML::Streamer lEvolverStreamer(lEvolverOFS);
    mEvolver.write(lEvolverStreamer, ioContext);
    ioContext.mEvolverState = BVContextHTML::eActive;
    lEvolverOFS.close();
    inLogOS << "done" << std::endl;
  }

  if(mRegisterRead) {
    // Writing register.html file
    std::string lRegisterFile = ioContext.mOutDir + "/register.html";
    inLogOS << "Generating register file \"" << lRegisterFile << "\".." << std::flush;
    std::ofstream lRegisterOFS(lRegisterFile.c_str());
    if(lRegisterOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lRegisterFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mRegisterState = BVContextHTML::eInProcess;
    XML::Streamer lRegisterStreamer(lRegisterOFS);
    mRegister.write(lRegisterStreamer, ioContext);
    ioContext.mRegisterState = BVContextHTML::eActive;
    lRegisterOFS.close();
    inLogOS << "done" << std::endl;
  }

  if(mVivariumRead) {
    // Writing vivarium.html file
    std::string lVivariumFile = ioContext.mOutDir + "/vivarium.html";
    inLogOS << "Generating vivarium file \"" << lVivariumFile << "\".." << std::flush;
    std::ofstream lVivariumOFS(lVivariumFile.c_str());
    if(lVivariumOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lVivariumFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mVivariumState = BVContextHTML::eInProcess;
    XML::Streamer lVivariumStreamer(lVivariumOFS);
    writeVivariums(lVivariumStreamer, ioContext);
    ioContext.mVivariumState = BVContextHTML::eActive;
    lVivariumOFS.close();
    inLogOS << "done" << std::endl;
  }

  if(mSeedsRead) {
    // Writing seeds.html file
    std::string lSeedsFile = ioContext.mOutDir + "/seeds.html";
    inLogOS << "Generating seeds file \"" << lSeedsFile << "\".." << std::flush;
    std::ofstream lSeedsOFS(lSeedsFile.c_str());
    if(lSeedsOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lSeedsFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    ioContext.mSeedsState = BVContextHTML::eInProcess;
    XML::Streamer lSeedsStreamer(lSeedsOFS);
    mSeeds.write(lSeedsStreamer, ioContext);
    ioContext.mSeedsState = BVContextHTML::eActive;
    lSeedsOFS.close();
    inLogOS << "done" << std::endl;
  }

  if(ioContext.mAverageTreeWritten.empty() == false) {
    // Writing avt.html file
    std::string lAVTFile = ioContext.mOutDir + "/avt.html";
    inLogOS << "Generating average trees file \"" << lAVTFile << "\".." << std::flush;
    std::ofstream lAVTOFS(lAVTFile.c_str());
    if(lAVTOFS.good()==false) {
      inLogOS << "failed" << std::endl;
      std::ostringstream lOSS;
      lOSS << "Unable to write file \"" << lAVTFile << "\"!";
      throw std::runtime_error(lOSS.str());
    }
    XML::Streamer lAVTStreamer(lAVTOFS);
    writeListAVT(lAVTStreamer, ioContext);
    lAVTOFS.close();
    inLogOS << "done" << std::endl;
  }
}


/*!
 * 
 */
bool BVBeagle::parse(std::string inFileName)
{
  XML::Document lDocument;
#ifdef HAVE_LIBZ
  igzstream lGZIS(inFileName.c_str());
  lDocument.parse(lGZIS, inFileName);
  lGZIS.close();
#else  // HAVE_LIBZ
  lDocument.parse(inFileName);
#endif // HAVE_LIBZ
  const XML::Document& lConstDoc = lDocument;
  bool lValid=false;
  for(XML::ConstIterator lIter=lConstDoc.getFirstRoot(); lIter; ++lIter) {
    if((lIter->getType()==XML::eData) && (lIter->getValue()=="Beagle")) {
      read(lIter);
      mFileNames.push_back(inFileName);
      lValid=true;
    }
  }
  return lValid;
}


/*!
 *
 */
void BVBeagle::read(XML::ConstIterator inIter) {
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Beagle") return;
  unsigned int lGeneration=0;
  bool lVivariumReadHere=false;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Logger") {
      mLogger.read(lChild);
      mLoggerRead=true;
    }
    else if(lChild->getValue() == "Evolver") {
      mEvolver.read(lChild);
      mEvolverRead=true;
    }
    else if(lChild->getValue() == "Register") {
      mRegister.read(lChild);
      mRegisterRead=true;
    }
    else if(lChild->getValue() == "Vivarium") {
      mVivariums.push_back(BVVivarium());
      mVivariums.back().read(lChild);
      mVivariumRead=true;
      lVivariumReadHere=true;
    }
    else if(lChild->getValue() == "Seeds") {
      mSeeds.read(lChild);
      mSeedsRead=true;
    }
    else if(lChild->getValue() == "Milestone") {
      for(XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
        if(lChild2->getType() != XML::eData) continue;
        else if(lChild2->getValue() == "Generation") {
          XML::ConstIterator lChild3 = lChild2->getFirstChild();
          if(lChild3==false) continue;
          lGeneration = stringTo<unsigned int>(lChild3->getValue());
        }
      }
    }
  }
  if(lVivariumReadHere) mVivariums.back().setGeneration(lGeneration);
}


/*!
 *
 */
void BVBeagle::writeListAVT(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("Average Tree List");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();

  ioStreamer.openTag("ul");
  for(BVContextHTML::AVTMap::const_iterator lIterDeme=ioContext.mAverageTreeWritten.begin();
      lIterDeme!=ioContext.mAverageTreeWritten.end(); ++lIterDeme) {
    for(std::map<unsigned int,std::map<unsigned int,std::string> >::const_iterator lIterGeno=lIterDeme->second.begin();
        lIterGeno!=lIterDeme->second.end(); ++lIterGeno) {
      std::ostringstream lOSSAnchor;
      lOSSAnchor << "#deme" << lIterDeme->first << "_geno" << lIterGeno->first;
      ioStreamer.openTag("li");
      ioStreamer.openTag("a");
      ioStreamer.insertAttribute("href", lOSSAnchor.str());
      std::ostringstream lOSSMess;
      lOSSMess << "Average trees of deme " << lIterDeme->first;
      lOSSMess << ", genotype " << lIterGeno->first;
      ioStreamer.insertStringContent(lOSSMess.str());
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
    }
  }
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);    

  for(BVContextHTML::AVTMap::const_iterator lIterDeme=ioContext.mAverageTreeWritten.begin();
      lIterDeme!=ioContext.mAverageTreeWritten.end(); ++lIterDeme) {
    for(std::map<unsigned int,std::map<unsigned int,std::string> >::const_iterator lIterGeno=lIterDeme->second.begin();
        lIterGeno!=lIterDeme->second.end(); ++lIterGeno) {
      std::ostringstream lOSSAnchor;
      lOSSAnchor << "deme" << lIterDeme->first << "_geno" << lIterGeno->first;
      std::ostringstream lOSSMess;
      lOSSMess << "Average trees of deme " << lIterDeme->first;
      lOSSMess << ", genotype " << lIterGeno->first;
      ioStreamer.openTag("hr", false);
      ioStreamer.closeTag(false);
      ioStreamer.openTag("br", false);
      ioStreamer.closeTag(false);
      ioStreamer.openTag("center");
      ioStreamer.openTag("h2", false);
      ioStreamer.openTag("a");
      ioStreamer.insertAttribute("name", lOSSAnchor.str());
      ioStreamer.insertStringContent(lOSSMess.str());
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.openTag("br", false);
      ioStreamer.closeTag(false);
      
      for(std::map<unsigned int,std::string>::const_iterator lIterGen=lIterGeno->second.begin();
          lIterGen!=lIterGeno->second.end(); ++lIterGen) {
        std::ostringstream lOSSGen;
        lOSSGen << "Generation " << lIterGen->first;
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent("[");
        ioStreamer.openTag("a", false);
        ioStreamer.insertAttribute("href", lIterGen->second);
        ioStreamer.insertStringContent(lOSSGen.str());
        ioStreamer.closeTag(false);
        ioStreamer.insertStringContent("]");
        ioStreamer.openTag("br", false);
        ioStreamer.closeTag(false);
        ioStreamer.openTag("br", false);
        ioStreamer.closeTag(false);
        ioStreamer.openTag("object");
        ioStreamer.insertAttribute("data", lIterGen->second);
        ioStreamer.insertAttribute("type", "image/svg+xml");
        ioStreamer.insertAttribute("width", "600");
        ioStreamer.insertAttribute("height", "600");
        ioStreamer.openTag("embed");
        ioStreamer.insertAttribute("src", lIterGen->second);
        ioStreamer.insertAttribute("type", "image/svg+xml");
        ioStreamer.insertAttribute("width", "600");
        ioStreamer.insertAttribute("height", "600");
        ioStreamer.closeTag();
        ioStreamer.closeTag();
        ioStreamer.closeTag();
        ioStreamer.openTag("br", false);
        ioStreamer.closeTag(false);
      }
    }
  }
  
  ioContext.writeFooter(ioStreamer);
}


/*!
 *
 */
void BVBeagle::writeIndex(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  ioContext.writeHeader(ioStreamer);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h2", false);
  ioStreamer.insertStringContent("BEAGLE Visualizer Report");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();

  if(mFileNames.size() > 0) {
    ioStreamer.insertStringContent("Generated from files:");
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("ul");
    for(unsigned int i=0; i<mFileNames.size(); ++i) {
      ioStreamer.openTag("li");
      ioStreamer.openTag("tt", false);
      ioStreamer.insertStringContent(mFileNames[i]);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
    }
    ioStreamer.closeTag();
  }
  ioContext.writeFooter(ioStreamer);
}


/*!
 *
 */
void BVBeagle::writeVivariums(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  if(mVivariums.size() == 1) mVivariums.front().write(ioStreamer, ioContext);
  else {
    std::map<unsigned int,const BVVivarium*> lMap;
    for(std::list<BVVivarium>::const_iterator lIter=mVivariums.begin();
        lIter!=mVivariums.end(); ++lIter) {
      lMap[lIter->getGeneration()] = &(*lIter);
    }

    // Write header
    ioContext.writeHeader(ioStreamer);
    ioStreamer.openTag("center", false);
    ioStreamer.openTag("h2", false);
    ioStreamer.insertStringContent("Vivariums");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);

    // Content
    ioStreamer.insertStringContent("Several vivariums have been read:");
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("ul");

    for(std::map<unsigned int,const BVVivarium*>::iterator lIter=lMap.begin();
        lIter!=lMap.end(); ++lIter) {
      std::ostringstream lOSSFile;
      lOSSFile << "viva" << lIter->second->getGeneration() << ".html";
      std::string lVivaFilename=lOSSFile.str();

      ioStreamer.openTag("li");
      ioStreamer.openTag("a");
      ioStreamer.insertAttribute("href", lVivaFilename);
      std::ostringstream lOSSMess;
      lOSSMess << "Vivarium of generation " << lIter->second->getGeneration();
      ioStreamer.insertStringContent(lOSSMess.str());
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);

      lVivaFilename = ioContext.mOutDir + std::string("/") + lVivaFilename;
      std::ofstream lVivaOFS(lVivaFilename.c_str());
      XML::Streamer lVivaStreamer(lVivaOFS);
      lIter->second->write(lVivaStreamer, ioContext);
      lVivaOFS.close();
    }

    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioContext.writeFooter(ioStreamer);
  }
}



