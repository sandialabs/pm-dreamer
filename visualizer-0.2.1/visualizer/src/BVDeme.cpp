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
 
#include <cassert>
#include <fstream>
#include <algorithm>
#include "BVTreeAverager.hpp"
#include "BVDeme.hpp"
#include "BVUtil.hpp"

using namespace PACC;


/*!
 *
 */
void BVDeme::computeMeanStdDev(const std::vector< std::vector<double> >& inData,
                               std::vector<double>& outMeans,
                               std::vector<double>& outStdDev) const
{
  outMeans.resize(0);
  outStdDev.resize(0);
  if(inData.size() == 0) return;

  // Compute mean
  outMeans.resize(inData.size(), 0.0);
  for(unsigned int i=0; i<inData.size(); ++i) {
    for(unsigned int j=0; j<inData[i].size(); ++j) outMeans[i] += inData[i][j];
    outMeans[i] /= inData[i].size();
  }
  
  // Compute standard deviation
  outStdDev.resize(inData.size(), 0.0);
  if(inData.size() == 1) return;  
  for(unsigned int i=0; i<inData.size(); ++i) {
    for(unsigned int j=0; j<inData[i].size(); ++j) {
      outStdDev[i] += ((inData[i][j] - outMeans[i]) * (inData[i][j] - outMeans[i]));
    }
    outStdDev[i] /= (inData[i].size() - 1);
    outStdDev[i] = std::sqrt(outStdDev[i]);
  }
}


/*!
 *
 */
void BVDeme::computeCovar(const std::vector< std::vector<double> >& inData,
                          const std::vector<double>& inMeans,
                          std::vector< std::vector<double> >& outCovar) const
{
  outCovar.resize(0);
  if(inData.size() == 0) return;

  // Resize covariance matrix
  outCovar.resize(inData.size());
  for(unsigned int i=0; i<outCovar.size(); ++i) outCovar[i].resize(inData.size(), 0.0);
  if(inData.size() == 1) return;
  
  // Compute covariance matrix
  assert(inMeans.size() == inData.size());
  for(unsigned int i=0; i<inData.front().size(); ++i) {
    for(unsigned int j=0; j<inData.size(); ++j) {
      for(unsigned int k=0; k<inData.size(); ++k) {
        outCovar[j][k] += (inData[j][i] - inMeans[j]) * (inData[k][i] - inMeans[k]);
      }
    }
  }
  for(unsigned int j=0; j<outCovar.size(); ++j) {
    for(unsigned int k=0; k<outCovar[j].size(); ++k) outCovar[j][k] /= (inData.front().size() - 1);
  }
}


/*!
 *
 */
void BVDeme::read(XML::ConstIterator inIter)
{
  if((inIter==false) || (inIter->getType()!=XML::eData)) return;
  if(inIter->getValue() != "Deme") return;
  for(XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != XML::eData) continue;
    else if(lChild->getValue() == "Stats") mStats.read(lChild);
    else if(lChild->getValue() == "HallOfFame") mHallOfFame.read(lChild);
    else if(lChild->getValue() == "MigrationBuffer") {
      for(XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
        if(lChild2->getType() != XML::eData) continue;
        else if(lChild2->getValue() == "Individual") {
          mMigrationBuffer.resize(mMigrationBuffer.size()+1);
          mMigrationBuffer.back().read(lChild2);
        }
      }
    }
    else if(lChild->getValue() == "Population") {
      for(XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
        if(lChild2->getType() != XML::eData) continue;
        else if(lChild2->getValue() == "Individual") {
          mPopulation.resize(mPopulation.size()+1);
          mPopulation.back().read(lChild2);
        }
      }
    }
  }
}


/*!
 *
 */
void BVDeme::writeTC(XML::Streamer& ioStreamer, BVContextHTML& ioContext) const
{
  bool lBitStringPresent=false;
  bool lFloatVectorPresent=false;
  bool lESVectorPresent=false;
  bool lGPTreePresent=false;
  bool lSimpleFitnessPresent=false;
  bool lMOFitnessPresent=false;
  bool lKozaFitnessPresent=false;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    switch(mPopulation[i].mFitness.getType()) {
      case BVFitness::eSimple:   lSimpleFitnessPresent=true; break;
      case BVFitness::eMultiObj: lMOFitnessPresent=true;     break;
      case BVFitness::eKoza:     lKozaFitnessPresent=true;   break;
      default: break;
    }
    for(unsigned int j=0; j<mPopulation[i].mGenotypes.size(); ++j) {
      switch(mPopulation[i].mGenotypes[j].getType()) {
        case BVGenotype::eBitString:   lBitStringPresent=true;   break;
        case BVGenotype::eFloatVector: lFloatVectorPresent=true; break;
        case BVGenotype::eESVector:    lESVectorPresent=true;    break;
        case BVGenotype::eGPTree:      lGPTreePresent=true;      break;
        default: break;
      }
    }
  }

  ioStreamer.openTag("ul");
  if(lSimpleFitnessPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_fit_simple");
    ioStreamer.insertStringContent("Simple Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lMOFitnessPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_fit_mo");
    ioStreamer.insertStringContent("Multi-Objective Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lKozaFitnessPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_fit_koza");
    ioStreamer.insertStringContent("Koza Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lBitStringPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_geno_bits");
    ioStreamer.insertStringContent("Bit String Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lFloatVectorPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_geno_floats");
    ioStreamer.insertStringContent("Float Vector Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lESVectorPresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_geno_es");
    ioStreamer.insertStringContent("ES Vector Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(lGPTreePresent) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_analysis_geno_gptree");
    ioStreamer.insertStringContent("GP Tree Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  ioStreamer.openTag("li");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", "#deme_stats");
  ioStreamer.insertStringContent("Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  if(mHallOfFame.getSize() > 0) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_hof");
    ioStreamer.insertStringContent("Hall-of-Fame");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }
  if(mMigrationBuffer.size() > 0) {
    ioStreamer.openTag("li");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", "#deme_migbuf");
    ioStreamer.insertStringContent("Migration Buffer");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  ioStreamer.openTag("li");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", "#deme_pop");
  ioStreamer.insertStringContent("Population");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
}


/*!
 *
 */
void BVDeme::writeContent(XML::Streamer& ioStreamer,
                          BVContextHTML& ioContext,
                          std::string inPrefix,
                          unsigned int inGeneration,
                          unsigned int inDemeIndex) const
{
  std::set<unsigned int> lBitStringGenoIndex;
  std::set<unsigned int> lFloatVectorGenoIndex;
  std::set<unsigned int> lESVectorGenoIndex;
  std::set<unsigned int> lGPTreeGenoIndex;
  bool lSimpleFitnessPresent=false;
  bool lMOFitnessPresent=false;
  bool lKozaFitnessPresent=false;
  bool lMultipleGeno=false;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mGenotypes.size() > 1) lMultipleGeno = true;
    switch(mPopulation[i].mFitness.getType()) {
      case BVFitness::eSimple:   lSimpleFitnessPresent=true; break;
      case BVFitness::eMultiObj: lMOFitnessPresent=true;     break;
      case BVFitness::eKoza:     lKozaFitnessPresent=true;   break;
      default: break;
    }
    for(unsigned int j=0; j<mPopulation[i].mGenotypes.size(); ++j) {
      switch(mPopulation[i].mGenotypes[j].getType()) {
        case BVGenotype::eBitString:   lBitStringGenoIndex.insert(j);   break;
        case BVGenotype::eFloatVector: lFloatVectorGenoIndex.insert(j); break;
        case BVGenotype::eESVector:    lESVectorGenoIndex.insert(j);    break;
        case BVGenotype::eGPTree:      lGPTreeGenoIndex.insert(j);      break;
        default: break;
      }
    }
  }

  if(lSimpleFitnessPresent) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_fit_simple");
    ioStreamer.insertStringContent("Deme Simple Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writeAnalysisFitnessSimple(ioStreamer, ioContext, inPrefix);
  }
  if(lMOFitnessPresent) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_fit_mo");
    ioStreamer.insertStringContent("Deme Multi-Objective Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writeAnalysisFitnessMO(ioStreamer, ioContext, inPrefix);
  }
  if(lKozaFitnessPresent) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_fit_koza");
    ioStreamer.insertStringContent("Deme Koza Fitness Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writeAnalysisFitnessKoza(ioStreamer, ioContext, inPrefix);
  }
  if(lBitStringGenoIndex.size() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_geno_bits");
    ioStreamer.insertStringContent("Deme Bit String Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    for(std::set<unsigned int>::const_iterator lIter=lBitStringGenoIndex.begin();
        lIter!=lBitStringGenoIndex.end(); ++lIter) {
      writeAnalysisGenoBitString(ioStreamer, ioContext, inPrefix, *lIter, lMultipleGeno);
    }
  }
  if(lFloatVectorGenoIndex.size() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_geno_floats");
    ioStreamer.insertStringContent("Deme Float Vector Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    for(std::set<unsigned int>::const_iterator lIter=lFloatVectorGenoIndex.begin();
        lIter!=lFloatVectorGenoIndex.end(); ++lIter) {
      writeAnalysisGenoFloatVector(ioStreamer, ioContext, inPrefix, *lIter, lMultipleGeno);
    }
  }
  if(lESVectorGenoIndex.size() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_geno_es");
    ioStreamer.insertStringContent("Deme ES Vector Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    for(std::set<unsigned int>::const_iterator lIter=lESVectorGenoIndex.begin();
        lIter!=lESVectorGenoIndex.end(); ++lIter) {
      writeAnalysisGenoES(ioStreamer, ioContext, inPrefix, *lIter, lMultipleGeno);
    }
  }
  if(lGPTreeGenoIndex.size() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_analysis_geno_gptree");
    ioStreamer.insertStringContent("Deme GP Tree Genotypes Analysis");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    for(std::set<unsigned int>::const_iterator lIter=lGPTreeGenoIndex.begin();
        lIter!=lGPTreeGenoIndex.end(); ++lIter) {
      writeAnalysisGenoGPTree(ioStreamer, ioContext, inPrefix, inGeneration, inDemeIndex,
                              *lIter, lMultipleGeno);
    }
  }

  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h3", false);
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("name", "deme_stats");
  ioStreamer.insertStringContent("Deme Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  mStats.write(ioStreamer, ioContext);

  if(mHallOfFame.getSize() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_hof");
    ioStreamer.insertStringContent("Deme Hall-of-Fame");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    mHallOfFame.write(ioStreamer, ioContext);
  }

  if(mMigrationBuffer.size() > 0) {
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("hr", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("center");
    ioStreamer.openTag("h3", false);
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("name", "deme_migbuf");
    ioStreamer.insertStringContent("Deme Migration Buffer");
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    writePopulation(mMigrationBuffer, ioStreamer, ioContext);
  }

  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("center");
  ioStreamer.openTag("h3", false);
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("name", "deme_pop");
  ioStreamer.insertStringContent("Deme Population");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  writePopulation(mPopulation, ioStreamer, ioContext);
}


/*!
 *
 */
void BVDeme::writeAnalysisFitnessSimple(PACC::XML::Streamer& ioStreamer,
                                        BVContextHTML& ioContext,
                                        std::string inPrefix) const
{
  // Extract simple fitness values
  std::vector<double> lFitness;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mFitness.getType() != BVFitness::eSimple) continue;
    lFitness.push_back(mPopulation[i].mFitness.getFitnessSimple());
  }

  // Write fitness values in CSV file
  std::string lCSVFileName = inPrefix + "analysis_fit_simple.csv";
  std::ofstream lCSVOFS(std::string(ioContext.mOutDir+std::string("/")+lCSVFileName).c_str());
  lCSVOFS << "\"FITNESS\"" << std::endl;
  for(unsigned int i=0; i<lFitness.size(); ++i) lCSVOFS << lFitness[i] << std::endl;
  lCSVOFS.close();
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileName.c_str());
  ioStreamer.insertStringContent("Fitness Values");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Compute mean and standard deviation
  double lMean=0.0, lStdDev=0.0, lMin=0.0, l1stQuart=0.0, lMedian=0.0, l3rdQuart=0.0, lMax=0.0;
  if(lFitness.size() > 0) {
    for(unsigned int i=0; i<lFitness.size(); ++i) lMean += lFitness[i];
    lMean /= lFitness.size();
  }
  if(lFitness.size() > 1) {
    for(unsigned int i=0; i<lFitness.size(); ++i) {
      lStdDev += ((lFitness[i]-lMean) * (lFitness[i]-lMean));
    }
    lStdDev /= (lFitness.size()-1);
  }
  std::sort(lFitness.begin(), lFitness.end());

  if(lFitness.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lFitness.size());
    if(l1stQuartIndex >= lFitness.size()) l1stQuartIndex = (lFitness.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lFitness.size());
    if(lMedianIndex >= lFitness.size()) lMedianIndex = (lFitness.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lFitness.size());
    if(l3rdQuartIndex >= lFitness.size()) l3rdQuartIndex = (lFitness.size()-1);
    lMin      = lFitness.front();
    l1stQuart = lFitness[l1stQuartIndex];
    lMedian   = lFitness[lMedianIndex];
    l3rdQuart = lFitness[l3rdQuartIndex];
    lMax      = lFitness.back();
  }

  // Write analysis
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Simple Fitness Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMean));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDev));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMin));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuart));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedian));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuart));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMax));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
}


/*!
 *
 */
void BVDeme::writeAnalysisFitnessMO(PACC::XML::Streamer& ioStreamer,
                                    BVContextHTML& ioContext,
                                    std::string inPrefix) const
{
  // Extract simple fitness values
  std::vector< std::vector<double> > lData;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mFitness.getType() != BVFitness::eMultiObj) continue;
    const std::vector<double>& lFitness = mPopulation[i].mFitness.getFitnessMultiObj();
    if(lData.size() == 0) lData.resize(lFitness.size());
    for(unsigned int j=0; j<lFitness.size(); ++j) lData[j].push_back(lFitness[j]);
  }

  // Compute mean and standard deviation
  std::vector<double> lMean, lStdDev;
  std::vector< std::vector<double> > lCovar;
  computeMeanStdDev(lData, lMean, lStdDev);
  if(lData.size() <= 100) computeCovar(lData, lMean, lCovar);

  // Write fitness values in CSV file
  std::string lCSVFileName = inPrefix + "analysis_fit_multiobj_fitness.csv";
  std::ofstream lCSVOFS(std::string(ioContext.mOutDir+std::string("/")+lCSVFileName).c_str());
  for(unsigned int i=0; i<lData.size(); ++i) {
    lCSVOFS << "\"OBJECTIVE" << i << "\"";
    if(i == (lData.size()-1)) lCSVOFS << std::endl;
    else lCSVOFS << ",";
  }
  for(unsigned int i=0; i<lData.front().size(); ++i) {
    for(unsigned int j=0; j<lData.size(); ++j) {
      lCSVOFS << lData[j][i];
      if(j == (lData.size()-1)) lCSVOFS << std::endl;
      else lCSVOFS << ",";
    }
  }
  lCSVOFS.close();
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileName.c_str());
  ioStreamer.insertStringContent("Fitness Values");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  if(lData.size() <= 100) {
    std::string lCSVCovar = inPrefix + "analysis_fit_multiobj_covar.csv";
    ioStreamer.insertStringContent("&nbsp;&nbsp;[");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lCSVCovar.c_str());
    ioStreamer.insertStringContent("Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("]");

    lCSVCovar = ioContext.mOutDir + std::string("/") + lCSVCovar;
    std::ofstream lOFSCovar(lCSVCovar.c_str());
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        lOFSCovar << lCovar[i][j];
        if(j == (lCovar[i].size()-1)) lOFSCovar << std::endl;
        else lOFSCovar << ",";
      }
    }
    lOFSCovar.close();
  }
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);


  // Output statistics
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Multi-Objective Fitness Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Objective");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(unsigned int i=0; i<lData.size(); ++i) {
    std::sort(lData[i].begin(), lData[i].end());
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lData[i].size());
    if(l1stQuartIndex >= lData[i].size()) l1stQuartIndex = (lData[i].size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lData[i].size());
    if(lMedianIndex >= lData[i].size()) lMedianIndex = (lData[i].size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lData[i].size());
    if(l3rdQuartIndex >= lData[i].size()) l3rdQuartIndex = (lData[i].size()-1);
    const double lMin      = lData[i].front();
    const double l1stQuart = lData[i][l1stQuartIndex];
    const double lMedian   = lData[i][lMedianIndex];
    const double l3rdQuart = lData[i][l3rdQuartIndex];
    const double lMax      = lData[i].back();

    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(i));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMean[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lStdDev[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMin));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l1stQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMedian));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l3rdQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMax));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  if(lData.size() <= 10) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("b", false);
    ioStreamer.insertStringContent("Multi-Objective Fitness Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("table");
    ioStreamer.insertAttribute("border", "1");
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      if(i==0) {
        ioStreamer.openTag("tr");
        ioStreamer.openTag("td");
        ioStreamer.closeTag(false);
        for(unsigned int j=0; j<lCovar[i].size(); ++j) {
          ioStreamer.openTag("td");
          ioStreamer.openTag("center");
          ioStreamer.openTag("b");
          ioStreamer.insertStringContent("s");
          ioStreamer.openTag("font", false);
          ioStreamer.insertAttribute("size", "-3");
          ioStreamer.insertStringContent(toString(j)+std::string("y"));
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
        }
        ioStreamer.closeTag(false);
      }
      ioStreamer.openTag("tr");
      ioStreamer.openTag("td");
      ioStreamer.openTag("center");
      ioStreamer.openTag("b");
      ioStreamer.insertStringContent("s");
      ioStreamer.openTag("font", false);
      ioStreamer.insertAttribute("size", "-3");
      ioStreamer.insertStringContent(std::string("x")+toString(i));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        ioStreamer.openTag("td");
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent(toString(lCovar[i][j]));
        ioStreamer.closeTag(false);
        ioStreamer.closeTag(false);
      }
      ioStreamer.closeTag(false);
    }
    ioStreamer.closeTag();
    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }
}


/*!
 *
 */
void BVDeme::writeAnalysisFitnessKoza(PACC::XML::Streamer& ioStreamer,
                                      BVContextHTML& ioContext,
                                      std::string inPrefix) const
{
  // Extract simple fitness values
  std::vector<double> lNormalized, lAdjusted, lStandardized, lRaw, lHits;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mFitness.getType() != BVFitness::eKoza) continue;
    const std::vector<double>& lFitness = mPopulation[i].mFitness.getFitnessKoza();
    assert(lFitness.size() == 5);
    lNormalized.push_back(lFitness[0]);
    lAdjusted.push_back(lFitness[1]);
    lStandardized.push_back(lFitness[2]);
    lRaw.push_back(lFitness[3]);
    lHits.push_back(lFitness[4]);
  }

  // Write fitness values in CSV file
  std::string lCSVFileName = inPrefix + "analysis_fit_koza.csv";
  std::ofstream lCSVOFS(std::string(ioContext.mOutDir+std::string("/")+lCSVFileName).c_str());
  lCSVOFS << "\"NORMALIZED\",\"ADJUSTED\",\"STANDARDIZED\",\"RAW\",\"HITS\"" << std::endl;
  for(unsigned int i=0; i<lNormalized.size(); ++i) {
    lCSVOFS << lNormalized[i] << "," << lAdjusted[i] << "," << lStandardized[i] << ",";
    lCSVOFS << lRaw[i] << "," << (unsigned int)lHits[i] << std::endl;
  }
  lCSVOFS.close();
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileName.c_str());
  ioStreamer.insertStringContent("Fitness Values");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Compute mean and standard deviation

  // Normalized fitness
  double lMeanNorm=0.0, lStdDevNorm=0.0, lMinNorm=0.0, l1stQuartNorm=0.0;
  double lMedianNorm=0.0, l3rdQuartNorm=0.0, lMaxNorm=0.0;
  if(lNormalized.size() > 0) {
    for(unsigned int i=0; i<lNormalized.size(); ++i) lMeanNorm += lNormalized[i];
    lMeanNorm /= lNormalized.size();
  }
  if(lNormalized.size() > 1) {
    for(unsigned int i=0; i<lNormalized.size(); ++i) {
      lStdDevNorm += ((lNormalized[i]-lMeanNorm) * (lNormalized[i]-lMeanNorm));
    }
    lStdDevNorm /= (lNormalized.size()-1);
  }
  std::sort(lNormalized.begin(), lNormalized.end());
  if(lNormalized.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lNormalized.size());
    if(l1stQuartIndex >= lNormalized.size()) l1stQuartIndex = (lNormalized.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lNormalized.size());
    if(lMedianIndex >= lNormalized.size()) lMedianIndex = (lNormalized.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lNormalized.size());
    if(l3rdQuartIndex >= lNormalized.size()) l3rdQuartIndex = (lNormalized.size()-1);
    lMinNorm      = lNormalized.front();
    l1stQuartNorm = lNormalized[l1stQuartIndex];
    lMedianNorm   = lNormalized[lMedianIndex];
    l3rdQuartNorm = lNormalized[l3rdQuartIndex];
    lMaxNorm      = lNormalized.back();
  }

  // Adjusted fitness
  double lMeanAdj=0.0, lStdDevAdj=0.0, lMinAdj=0.0, l1stQuartAdj=0.0;
  double lMedianAdj=0.0, l3rdQuartAdj=0.0, lMaxAdj=0.0;
  if(lAdjusted.size() > 0) {
    for(unsigned int i=0; i<lAdjusted.size(); ++i) lMeanAdj += lAdjusted[i];
    lMeanAdj /= lAdjusted.size();
  }
  if(lAdjusted.size() > 1) {
    for(unsigned int i=0; i<lAdjusted.size(); ++i) {
      lStdDevAdj += ((lAdjusted[i]-lMeanAdj) * (lAdjusted[i]-lMeanAdj));
    }
    lStdDevAdj /= (lAdjusted.size()-1);
  }
  std::sort(lAdjusted.begin(), lAdjusted.end());
  if(lAdjusted.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lAdjusted.size());
    if(l1stQuartIndex >= lAdjusted.size()) l1stQuartIndex = (lAdjusted.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lAdjusted.size());
    if(lMedianIndex >= lAdjusted.size()) lMedianIndex = (lAdjusted.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lAdjusted.size());
    if(l3rdQuartIndex >= lAdjusted.size()) l3rdQuartIndex = (lAdjusted.size()-1);
    lMinAdj      = lAdjusted.front();
    l1stQuartAdj = lAdjusted[l1stQuartIndex];
    lMedianAdj   = lAdjusted[lMedianIndex];
    l3rdQuartAdj = lAdjusted[l3rdQuartIndex];
    lMaxAdj      = lAdjusted.back();
  }

  // Standardized fitness
  double lMeanStd=0.0, lStdDevStd=0.0, lMinStd=0.0, l1stQuartStd=0.0;
  double lMedianStd=0.0, l3rdQuartStd=0.0, lMaxStd=0.0;
  if(lStandardized.size() > 0) {
    for(unsigned int i=0; i<lStandardized.size(); ++i) lMeanStd += lStandardized[i];
    lMeanStd /= lStandardized.size();
  }
  if(lStandardized.size() > 1) {
    for(unsigned int i=0; i<lStandardized.size(); ++i) {
      lStdDevStd += ((lStandardized[i]-lMeanStd) * (lStandardized[i]-lMeanStd));
    }
    lStdDevStd /= (lStandardized.size()-1);
  }
  std::sort(lStandardized.begin(), lStandardized.end());
  if(lStandardized.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lStandardized.size());
    if(l1stQuartIndex >= lStandardized.size()) l1stQuartIndex = (lStandardized.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lStandardized.size());
    if(lMedianIndex >= lStandardized.size()) lMedianIndex = (lStandardized.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lStandardized.size());
    if(l3rdQuartIndex >= lStandardized.size()) l3rdQuartIndex = (lStandardized.size()-1);
    lMinStd      = lStandardized.front();
    l1stQuartStd = lStandardized[l1stQuartIndex];
    lMedianStd   = lStandardized[lMedianIndex];
    l3rdQuartStd = lStandardized[l3rdQuartIndex];
    lMaxStd      = lStandardized.back();
  }

  // Raw fitness
  double lMeanRaw=0.0, lStdDevRaw=0.0, lMinRaw=0.0, l1stQuartRaw=0.0;
  double lMedianRaw=0.0, l3rdQuartRaw=0.0, lMaxRaw=0.0;
  if(lRaw.size() > 0) {
    for(unsigned int i=0; i<lRaw.size(); ++i) lMeanRaw += lRaw[i];
    lMeanRaw /= lRaw.size();
  }
  if(lRaw.size() > 1) {
    for(unsigned int i=0; i<lRaw.size(); ++i) {
      lStdDevRaw += ((lRaw[i]-lMeanRaw) * (lRaw[i]-lMeanRaw));
    }
    lStdDevRaw /= (lRaw.size()-1);
  }
  std::sort(lRaw.begin(), lRaw.end());
  if(lRaw.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lRaw.size());
    if(l1stQuartIndex >= lRaw.size()) l1stQuartIndex = (lRaw.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lRaw.size());
    if(lMedianIndex >= lRaw.size()) lMedianIndex = (lRaw.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lRaw.size());
    if(l3rdQuartIndex >= lRaw.size()) l3rdQuartIndex = (lRaw.size()-1);
    lMinRaw      = lRaw.front();
    l1stQuartRaw = lRaw[l1stQuartIndex];
    lMedianRaw   = lRaw[lMedianIndex];
    l3rdQuartRaw = lRaw[l3rdQuartIndex];
    lMaxRaw      = lRaw.back();
  }

  // Hits fitness
  double lMeanHits=0.0, lStdDevHits=0.0;
  unsigned int lMinHits=0, l1stQuartHits=0;
  unsigned int lMedianHits=0, l3rdQuartHits=0, lMaxHits=0;
  if(lHits.size() > 0) {
    for(unsigned int i=0; i<lHits.size(); ++i) lMeanHits += lHits[i];
    lMeanHits /= lHits.size();
  }
  if(lHits.size() > 1) {
    for(unsigned int i=0; i<lHits.size(); ++i) {
      lStdDevHits += ((lHits[i]-lMeanHits) * (lHits[i]-lMeanHits));
    }
    lStdDevHits /= (lHits.size()-1);
  }
  std::sort(lHits.begin(), lHits.end());
  if(lHits.size() > 0) {
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lHits.size());
    if(l1stQuartIndex >= lHits.size()) l1stQuartIndex = (lHits.size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lHits.size());
    if(lMedianIndex >= lHits.size()) lMedianIndex = (lHits.size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lHits.size());
    if(l3rdQuartIndex >= lHits.size()) l3rdQuartIndex = (lHits.size()-1);
    lMinHits      = (unsigned int)lHits.front();
    l1stQuartHits = (unsigned int)lHits[l1stQuartIndex];
    lMedianHits   = (unsigned int)lHits[lMedianIndex];
    l3rdQuartHits = (unsigned int)lHits[l3rdQuartIndex];
    lMaxHits      = (unsigned int)lHits.back();
  }

  // Write analysis
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Koza Fitness Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Fitness");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMeanNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDevNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMinNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuartNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedianNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuartNorm));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMaxNorm));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMeanAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDevAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMinAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuartAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedianAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuartAdj));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMaxAdj));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMeanStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDevStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMinStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuartStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedianStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuartStd));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMaxStd));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMeanRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDevRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMinRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuartRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedianRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuartRaw));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMaxRaw));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMeanHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lStdDevHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMinHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l1stQuartHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMedianHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(l3rdQuartHits));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.insertStringContent(toString(lMaxHits));
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();

  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
}


/*!
 *
 */
void BVDeme::writeAnalysisGenoBitString(PACC::XML::Streamer& ioStreamer,
                                        BVContextHTML& ioContext,
                                        std::string inPrefix,
                                        unsigned int inGenoIndex,
                                        bool inMultipleGeno) const
{
  // Header if multiple genotypes
  if(inMultipleGeno) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("h4", false);
    std::ostringstream lOSS;
    lOSS << "Analysis for genotype " << inGenoIndex;
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  // Extract simple fitness values
  std::vector< std::vector<double> > lData;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mGenotypes[inGenoIndex].getType() != BVGenotype::eBitString) continue;
    const std::vector<bool>& lGeno = mPopulation[i].mGenotypes[inGenoIndex].getBitString();
    if(lData.size() == 0) lData.resize(lGeno.size());
    for(unsigned int j=0; j<lGeno.size(); ++j) {
      lData[j].push_back(lGeno[j] ? 1.0 : 0.0);
    }
  }

  // Compute mean and standard deviation
  std::vector<double> lMean, lStdDev;
  std::vector< std::vector<double> > lCovar;
  computeMeanStdDev(lData, lMean, lStdDev);
  if(lData.size() <= 100) computeCovar(lData, lMean, lCovar);

  // Write parameters values in CSV file
  std::string lCSVFileName = inPrefix;
  if(inMultipleGeno) {
    lCSVFileName += "analysis_geno";
    lCSVFileName += toString(inGenoIndex);
    lCSVFileName += "_bits_params.csv";
  }
  else lCSVFileName += "analysis_geno_bits_params.csv";
  std::ofstream lCSVOFS(std::string(ioContext.mOutDir+std::string("/")+lCSVFileName).c_str());
  for(unsigned int i=0; i<lData.size(); ++i) {
    lCSVOFS << "\"BIT" << i << "\"";
    if(i == (lData.size()-1)) lCSVOFS << std::endl;
    else lCSVOFS << ",";
  }
  for(unsigned int i=0; i<lData.front().size(); ++i) {
    for(unsigned int j=0; j<lData.size(); ++j) {
      lCSVOFS << lData[j][i];
      if(j == (lData.size()-1)) lCSVOFS << std::endl;
      else lCSVOFS << ",";
    }
  }
  lCSVOFS.close();
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileName.c_str());
  ioStreamer.insertStringContent("Bit Strings");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  if(lData.size() <= 100) {
    std::string lCSVCovar = inPrefix;
    if(inMultipleGeno) {
      lCSVCovar += "analysis_geno";
      lCSVCovar += toString(inGenoIndex);
      lCSVCovar += "_bits_covar.csv";
    }
    else lCSVCovar += "analysis_geno_bits_covar.csv";
    ioStreamer.insertStringContent("&nbsp;&nbsp;[");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lCSVCovar.c_str());
    ioStreamer.insertStringContent("Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("]");

    lCSVCovar = ioContext.mOutDir + std::string("/") + lCSVCovar;
    std::ofstream lOFSCovar(lCSVCovar.c_str());
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        lOFSCovar << lCovar[i][j];
        if(j == (lCovar[i].size()-1)) lOFSCovar << std::endl;
        else lOFSCovar << ",";
      }
    }
    lOFSCovar.close();
  }
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);


  // Output statistics
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Bit String Genotypes Values Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Bit");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(unsigned int i=0; i<lData.size(); ++i) {
    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(i));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMean[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lStdDev[i]));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  if(lData.size() <= 10) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("b", false);
    ioStreamer.insertStringContent("Bit String Genotypes Values Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("table");
    ioStreamer.insertAttribute("border", "1");
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      if(i==0) {
        ioStreamer.openTag("tr");
        ioStreamer.openTag("td");
        ioStreamer.closeTag(false);
        for(unsigned int j=0; j<lCovar[i].size(); ++j) {
          ioStreamer.openTag("td");
          ioStreamer.openTag("center");
          ioStreamer.openTag("b");
          ioStreamer.insertStringContent("s");
          ioStreamer.openTag("font", false);
          ioStreamer.insertAttribute("size", "-3");
          ioStreamer.insertStringContent(toString(j)+std::string("y"));
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
        }
        ioStreamer.closeTag(false);
      }
      ioStreamer.openTag("tr");
      ioStreamer.openTag("td");
      ioStreamer.openTag("center");
      ioStreamer.openTag("b");
      ioStreamer.insertStringContent("s");
      ioStreamer.openTag("font", false);
      ioStreamer.insertAttribute("size", "-3");
      ioStreamer.insertStringContent(std::string("x")+toString(i));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        ioStreamer.openTag("td");
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent(toString(lCovar[i][j]));
        ioStreamer.closeTag(false);
        ioStreamer.closeTag(false);
      }
      ioStreamer.closeTag(false);
    }
    ioStreamer.closeTag();
    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }
}


/*!
 *
 */
void BVDeme::writeAnalysisGenoFloatVector(PACC::XML::Streamer& ioStreamer,
                                          BVContextHTML& ioContext,
                                          std::string inPrefix,
                                          unsigned int inGenoIndex,
                                          bool inMultipleGeno) const
{
  // Header if multiple genotypes
  if(inMultipleGeno) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("h4", false);
    std::ostringstream lOSS;
    lOSS << "Analysis for genotype " << inGenoIndex;
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  // Extract float vector values
  std::vector< std::vector<double> > lData;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mGenotypes[inGenoIndex].getType() != BVGenotype::eFloatVector) continue;
    const std::vector<double>& lGeno = mPopulation[i].mGenotypes[inGenoIndex].getFloatVector();
    if(lGeno.size() > lData.size()) lData.resize(lGeno.size());
    for(unsigned int j=0; j<lGeno.size(); ++j) lData[j].push_back(lGeno[j]);
  }

  // Compute mean and standard deviation
  std::vector<double> lMean, lStdDev;
  std::vector< std::vector<double> > lCovar;
  computeMeanStdDev(lData, lMean, lStdDev);
  if(lData.size() <= 100) computeCovar(lData, lMean, lCovar);

  // Write parameters values in CSV file
  std::string lCSVFileName = inPrefix;
  if(inMultipleGeno) {
    lCSVFileName += "analysis_geno";
    lCSVFileName += toString(inGenoIndex);
    lCSVFileName += "_floats_params.csv";
  }
  else lCSVFileName += "analysis_geno_floats_params.csv";
  std::ofstream lCSVOFS(std::string(ioContext.mOutDir+std::string("/")+lCSVFileName).c_str());
  for(unsigned int i=0; i<lData.size(); ++i) {
    lCSVOFS << "\"PARAMETER" << i << "\"";
    if(i == (lData.size()-1)) lCSVOFS << std::endl;
    else lCSVOFS << ",";
  }
  for(unsigned int i=0; i<lData.front().size(); ++i) {
    for(unsigned int j=0; j<lData.size(); ++j) {
      lCSVOFS << lData[j][i];
      if(j == (lData.size()-1)) lCSVOFS << std::endl;
      else lCSVOFS << ",";
    }
  }
  lCSVOFS.close();
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileName.c_str());
  ioStreamer.insertStringContent("Genotypes Parameters");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  if(lData.size() <= 100) {
    std::string lCSVCovar = inPrefix;
    if(inMultipleGeno) {
      lCSVCovar += "analysis_geno";
      lCSVCovar += toString(inGenoIndex);
      lCSVCovar += "_floats_covar.csv";
    }
    else lCSVCovar += "analysis_geno_floats_covar.csv";
    ioStreamer.insertStringContent("&nbsp;&nbsp;[");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lCSVCovar.c_str());
    ioStreamer.insertStringContent("Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("]");

    lCSVCovar = ioContext.mOutDir + std::string("/") + lCSVCovar;
    std::ofstream lOFSCovar(lCSVCovar.c_str());
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        lOFSCovar << lCovar[i][j];
        if(j == (lCovar[i].size()-1)) lOFSCovar << std::endl;
        else lOFSCovar << ",";
      }
    }
    lOFSCovar.close();
  }
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);


  // Output statistics
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Float Vector Genotypes Parameters Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Parameter");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(unsigned int i=0; i<lData.size(); ++i) {
    std::sort(lData[i].begin(), lData[i].end());
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lData[i].size());
    if(l1stQuartIndex >= lData[i].size()) l1stQuartIndex = (lData[i].size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lData[i].size());
    if(lMedianIndex >= lData[i].size()) lMedianIndex = (lData[i].size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lData[i].size());
    if(l3rdQuartIndex >= lData[i].size()) l3rdQuartIndex = (lData[i].size()-1);
    const double lMin      = lData[i].front();
    const double l1stQuart = lData[i][l1stQuartIndex];
    const double lMedian   = lData[i][lMedianIndex];
    const double l3rdQuart = lData[i][l3rdQuartIndex];
    const double lMax      = lData[i].back();

    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(i));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMean[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lStdDev[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMin));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l1stQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMedian));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l3rdQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMax));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  if(lData.size() <= 10) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("b", false);
    ioStreamer.insertStringContent("Float Vector Genotypes Parameters Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("table");
    ioStreamer.insertAttribute("border", "1");
    for(unsigned int i=0; i<lCovar.size(); ++i) {
      if(i==0) {
        ioStreamer.openTag("tr");
        ioStreamer.openTag("td");
        ioStreamer.closeTag(false);
        for(unsigned int j=0; j<lCovar[i].size(); ++j) {
          ioStreamer.openTag("td");
          ioStreamer.openTag("center");
          ioStreamer.openTag("b");
          ioStreamer.insertStringContent("s");
          ioStreamer.openTag("font", false);
          ioStreamer.insertAttribute("size", "-3");
          ioStreamer.insertStringContent(toString(j)+std::string("y"));
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
        }
        ioStreamer.closeTag(false);
      }
      ioStreamer.openTag("tr");
      ioStreamer.openTag("td");
      ioStreamer.openTag("center");
      ioStreamer.openTag("b");
      ioStreamer.insertStringContent("s");
      ioStreamer.openTag("font", false);
      ioStreamer.insertAttribute("size", "-3");
      ioStreamer.insertStringContent(std::string("x")+toString(i));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      for(unsigned int j=0; j<lCovar[i].size(); ++j) {
        ioStreamer.openTag("td");
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent(toString(lCovar[i][j]));
        ioStreamer.closeTag(false);
        ioStreamer.closeTag(false);
      }
      ioStreamer.closeTag(false);
    }
    ioStreamer.closeTag();
    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }
}


/*!
 *
 */
void BVDeme::writeAnalysisGenoES(PACC::XML::Streamer& ioStreamer,
                                 BVContextHTML& ioContext,
                                 std::string inPrefix,
                                 unsigned int inGenoIndex,
                                 bool inMultipleGeno) const
{
  // Header if multiple genotypes
  if(inMultipleGeno) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("h4", false);
    std::ostringstream lOSS;
    lOSS << "Analysis for genotype " << inGenoIndex;
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  // Extract simple fitness values
  std::vector< std::vector<double> > lValues;
  std::vector< std::vector<double> > lSigmas;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mGenotypes[inGenoIndex].getType() != BVGenotype::eESVector) continue;
    const std::vector< std::pair<double,double> >& lGeno =
      mPopulation[i].mGenotypes[inGenoIndex].getESVector();
    if(lGeno.size() > lValues.size()) lValues.resize(lGeno.size());
    if(lGeno.size() > lSigmas.size()) lSigmas.resize(lGeno.size());
    for(unsigned int j=0; j<lGeno.size(); ++j) {
      lValues[j].push_back(lGeno[j].first);
      lSigmas[j].push_back(lGeno[j].second);
    }
  }

  // Compute mean and standard deviation
  std::vector<double> lMeanValues, lStdDevValues;
  std::vector< std::vector<double> > lCovarValues;
  computeMeanStdDev(lValues, lMeanValues, lStdDevValues);
  if(lValues.size() <= 100) computeCovar(lValues, lMeanValues, lCovarValues);

  std::vector<double> lMeanSigmas, lStdDevSigmas;
  std::vector< std::vector<double> > lCovarSigmas;
  computeMeanStdDev(lSigmas, lMeanSigmas, lStdDevSigmas);
  if(lSigmas.size() <= 100) computeCovar(lSigmas, lMeanSigmas, lCovarSigmas);

  // Write parameters values in CSV file
  std::string lCSVFileNameValues = inPrefix;
  if(inMultipleGeno) {
    lCSVFileNameValues += "analysis_geno";
    lCSVFileNameValues += toString(inGenoIndex);
    lCSVFileNameValues += "_es_values.csv";
  }
  else lCSVFileNameValues += "analysis_geno_es_values.csv";
  std::ofstream lCSVOFSValues(std::string(ioContext.mOutDir+std::string("/")+lCSVFileNameValues).c_str());
  for(unsigned int i=0; i<lValues.size(); ++i) {
    lCSVOFSValues << "\"VALUE" << i << "\"";
    if(i == (lValues.size()-1)) lCSVOFSValues << std::endl;
    else lCSVOFSValues << ",";
  }
  for(unsigned int i=0; i<lValues.front().size(); ++i) {
    for(unsigned int j=0; j<lValues.size(); ++j) {
      lCSVOFSValues << lValues[j][i];
      if(j == (lValues.size()-1)) lCSVOFSValues << std::endl;
      else lCSVOFSValues << ",";
    }
  }
  lCSVOFSValues.close();
  std::string lCSVFileNameSigmas = inPrefix;
  if(inMultipleGeno) {
    lCSVFileNameSigmas += "analysis_geno";
    lCSVFileNameSigmas += toString(inGenoIndex);
    lCSVFileNameSigmas += "_es_sigmas.csv";
  }
  else lCSVFileNameSigmas += "analysis_geno_es_sigmas.csv";
  std::ofstream lCSVOFSSigmas(std::string(ioContext.mOutDir+std::string("/")+lCSVFileNameSigmas).c_str());
  for(unsigned int i=0; i<lSigmas.size(); ++i) {
    lCSVOFSSigmas << "\"SIGMA" << i << "\"";
    if(i == (lSigmas.size()-1)) lCSVOFSSigmas << std::endl;
    else lCSVOFSSigmas << ",";
  }
  for(unsigned int i=0; i<lSigmas.front().size(); ++i) {
    for(unsigned int j=0; j<lSigmas.size(); ++j) {
      lCSVOFSSigmas << lSigmas[j][i];
      if(j == (lSigmas.size()-1)) lCSVOFSSigmas << std::endl;
      else lCSVOFSSigmas << ",";
    }
  }
  lCSVOFSSigmas.close();

  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileNameValues.c_str());
  ioStreamer.insertStringContent("Genotypes Values");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");

  if(lValues.size() <= 100) {
    std::string lCSVCovarValues = inPrefix;
    if(inMultipleGeno) {
      lCSVCovarValues += "analysis_geno";
      lCSVCovarValues += toString(inGenoIndex);
      lCSVCovarValues += "_es_values_covar.csv";
    }
    else lCSVCovarValues += "analysis_geno_es_values_covar.csv";
    ioStreamer.insertStringContent("&nbsp;&nbsp;[");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lCSVCovarValues.c_str());
    ioStreamer.insertStringContent("Covariance Matrix of Values");
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("]");

    lCSVCovarValues = ioContext.mOutDir + std::string("/") + lCSVCovarValues;
    std::ofstream lOFSCovarValues(lCSVCovarValues.c_str());
    for(unsigned int i=0; i<lCovarValues.size(); ++i) {
      for(unsigned int j=0; j<lCovarValues[i].size(); ++j) {
        lOFSCovarValues << lCovarValues[i][j];
        if(j == (lCovarValues[i].size()-1)) lOFSCovarValues << std::endl;
        else lOFSCovarValues << ",";
      }
    }
    lOFSCovarValues.close();
  }

  ioStreamer.insertStringContent("&nbsp;&nbsp[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lCSVFileNameSigmas.c_str());
  ioStreamer.insertStringContent("Genotypes Sigmas");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  if(lSigmas.size() <= 100) {
    std::string lCSVCovarSigmas = inPrefix;
    if(inMultipleGeno) {
      lCSVCovarSigmas += "analysis_geno";
      lCSVCovarSigmas += toString(inGenoIndex);
      lCSVCovarSigmas += "_es_sigmas_covar.csv";
    }
    else lCSVCovarSigmas += "analysis_geno_es_sigmas_covar.csv";
    ioStreamer.insertStringContent("&nbsp;&nbsp;[");
    ioStreamer.openTag("a", false);
    ioStreamer.insertAttribute("href", lCSVCovarSigmas.c_str());
    ioStreamer.insertStringContent("Covariance Matrix of Sigmas");
    ioStreamer.closeTag(false);
    ioStreamer.insertStringContent("]");

    lCSVCovarSigmas = ioContext.mOutDir + std::string("/") + lCSVCovarSigmas;
    std::ofstream lOFSCovarSigmas(lCSVCovarSigmas.c_str());
    for(unsigned int i=0; i<lCovarSigmas.size(); ++i) {
      for(unsigned int j=0; j<lCovarSigmas[i].size(); ++j) {
        lOFSCovarSigmas << lCovarSigmas[i][j];
        if(j == (lCovarSigmas[i].size()-1)) lOFSCovarSigmas << std::endl;
        else lOFSCovarSigmas << ",";
      }
    }
    lOFSCovarSigmas.close();
  }
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);


  // Output statistics
  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("ES Vector Genotypes Values Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Value");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(unsigned int i=0; i<lValues.size(); ++i) {
    std::sort(lValues[i].begin(), lValues[i].end());
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lValues[i].size());
    if(l1stQuartIndex >= lValues[i].size()) l1stQuartIndex = (lValues[i].size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lValues[i].size());
    if(lMedianIndex >= lValues[i].size()) lMedianIndex = (lValues[i].size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lValues[i].size());
    if(l3rdQuartIndex >= lValues[i].size()) l3rdQuartIndex = (lValues[i].size()-1);
    const double lMin      = lValues[i].front();
    const double l1stQuart = lValues[i][l1stQuartIndex];
    const double lMedian   = lValues[i][lMedianIndex];
    const double l3rdQuart = lValues[i][l3rdQuartIndex];
    const double lMax      = lValues[i].back();

    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(i));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMeanValues[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lStdDevValues[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMin));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l1stQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMedian));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l3rdQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMax));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  ioStreamer.openTag("center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("ES Vector Genotypes Sigmas Statistics");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "1");
  ioStreamer.openTag("tr");
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Sigma");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Average");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Std Deviation");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Minimum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("1st Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Median");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("3rd Quartile");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("td");
  ioStreamer.insertAttribute("align", "center");
  ioStreamer.openTag("b", false);
  ioStreamer.insertStringContent("Maximum");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();

  for(unsigned int i=0; i<lSigmas.size(); ++i) {
    std::sort(lSigmas[i].begin(), lSigmas[i].end());
    unsigned int l1stQuartIndex = (unsigned int)std::floor(0.25*lSigmas[i].size());
    if(l1stQuartIndex >= lSigmas[i].size()) l1stQuartIndex = (lSigmas[i].size()-1);
    unsigned int lMedianIndex = (unsigned int)std::floor(0.5*lSigmas[i].size());
    if(lMedianIndex >= lSigmas[i].size()) lMedianIndex = (lSigmas[i].size()-1);
    unsigned int l3rdQuartIndex = (unsigned int)std::floor(0.75*lSigmas[i].size());
    if(l3rdQuartIndex >= lSigmas[i].size()) l3rdQuartIndex = (lSigmas[i].size()-1);
    const double lMin      = lSigmas[i].front();
    const double l1stQuart = lSigmas[i][l1stQuartIndex];
    const double lMedian   = lSigmas[i][lMedianIndex];
    const double l3rdQuart = lSigmas[i][l3rdQuartIndex];
    const double lMax      = lSigmas[i].back();

    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(i));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMeanSigmas[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lStdDevSigmas[i]));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMin));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l1stQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMedian));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(l3rdQuart));
    ioStreamer.closeTag(false);
    ioStreamer.openTag("td");
    ioStreamer.insertAttribute("align", "center");
    ioStreamer.insertStringContent(toString(lMax));
    ioStreamer.closeTag(false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  if(lValues.size() <= 10) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("b", false);
    ioStreamer.insertStringContent("ES Genotypes Values Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("table");
    ioStreamer.insertAttribute("border", "1");
    for(unsigned int i=0; i<lCovarValues.size(); ++i) {
      if(i==0) {
        ioStreamer.openTag("tr");
        ioStreamer.openTag("td");
        ioStreamer.closeTag(false);
        for(unsigned int j=0; j<lCovarValues[i].size(); ++j) {
          ioStreamer.openTag("td");
          ioStreamer.openTag("center");
          ioStreamer.openTag("b");
          ioStreamer.insertStringContent("s");
          ioStreamer.openTag("font", false);
          ioStreamer.insertAttribute("size", "-3");
          ioStreamer.insertStringContent(toString(j)+std::string("y"));
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
        }
        ioStreamer.closeTag(false);
      }
      ioStreamer.openTag("tr");
      ioStreamer.openTag("td");
      ioStreamer.openTag("center");
      ioStreamer.openTag("b");
      ioStreamer.insertStringContent("s");
      ioStreamer.openTag("font", false);
      ioStreamer.insertAttribute("size", "-3");
      ioStreamer.insertStringContent(std::string("x")+toString(i));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      for(unsigned int j=0; j<lCovarValues[i].size(); ++j) {
        ioStreamer.openTag("td");
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent(toString(lCovarValues[i][j]));
        ioStreamer.closeTag(false);
        ioStreamer.closeTag(false);
      }
      ioStreamer.closeTag(false);
    }
    ioStreamer.closeTag();
    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }

  if(lSigmas.size() <= 10) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("b", false);
    ioStreamer.insertStringContent("ES Genotypes Sigmas Covariance Matrix");
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
    ioStreamer.openTag("table");
    ioStreamer.insertAttribute("border", "1");
    for(unsigned int i=0; i<lCovarSigmas.size(); ++i) {
      if(i==0) {
        ioStreamer.openTag("tr");
        ioStreamer.openTag("td");
        ioStreamer.closeTag(false);
        for(unsigned int j=0; j<lCovarSigmas[i].size(); ++j) {
          ioStreamer.openTag("td");
          ioStreamer.openTag("center");
          ioStreamer.openTag("b");
          ioStreamer.insertStringContent("s");
          ioStreamer.openTag("font", false);
          ioStreamer.insertAttribute("size", "-3");
          ioStreamer.insertStringContent(toString(j)+std::string("y"));
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
          ioStreamer.closeTag(false);
        }
        ioStreamer.closeTag(false);
      }
      ioStreamer.openTag("tr");
      ioStreamer.openTag("td");
      ioStreamer.openTag("center");
      ioStreamer.openTag("b");
      ioStreamer.insertStringContent("s");
      ioStreamer.openTag("font", false);
      ioStreamer.insertAttribute("size", "-3");
      ioStreamer.insertStringContent(std::string("x")+toString(i));
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      ioStreamer.closeTag(false);
      for(unsigned int j=0; j<lCovarSigmas[i].size(); ++j) {
        ioStreamer.openTag("td");
        ioStreamer.openTag("center");
        ioStreamer.insertStringContent(toString(lCovarSigmas[i][j]));
        ioStreamer.closeTag(false);
        ioStreamer.closeTag(false);
      }
      ioStreamer.closeTag(false);
    }

    ioStreamer.closeTag();
    ioStreamer.closeTag();
    ioStreamer.openTag("br", false);
    ioStreamer.closeTag(false);
  }
}


/*!
 *
 */
void BVDeme::writeAnalysisGenoGPTree(PACC::XML::Streamer& ioStreamer,
                                     BVContextHTML& ioContext,
                                     std::string inPrefix,
                                     unsigned int inGeneration,
                                     unsigned int inDemeIndex,
                                     unsigned int inGenoIndex,
                                     bool inMultipleGeno) const
{ 
  // Header if multiple genotypes
  if(inMultipleGeno) {
    ioStreamer.openTag("center");
    ioStreamer.openTag("h4", false);
    std::ostringstream lOSS;
    lOSS << "Average tree for genotype " << (inGenoIndex+1);
    ioStreamer.insertStringContent(lOSS.str());
    ioStreamer.closeTag(false);
    ioStreamer.closeTag(false);
  }

  // Extract GP tree
  BVTreeAverager lAVT;
  for(unsigned int i=0; i<mPopulation.size(); ++i) {
    if(mPopulation[i].mGenotypes[inGenoIndex].getType() != BVGenotype::eGPTree) continue;
    lAVT.addXMLTree(mPopulation[i].mGenotypes[inGenoIndex].getGPTree());
  }

  // Output XHTML for the average tree
#ifdef HAVE_LIBZ
  std::string lAVTName = inPrefix + "avt.svgz";
#else  // HAVE_LIBZ
  std::string lAVTName = inPrefix + "avt.svg";
#endif // HAVE_LIBZ
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("[");
  ioStreamer.openTag("a", false);
  ioStreamer.insertAttribute("href", lAVTName);
  ioStreamer.insertStringContent("Actual Average Tree");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]&nbsp;&nbsp;[");
  ioStreamer.openTag("a", false);
  std::ostringstream lOSSAVTS;
  lOSSAVTS << "avt.html#deme" << (inDemeIndex+1) << "_geno" << (inGenoIndex+1);
  ioStreamer.insertAttribute("href", lOSSAVTS.str());
  ioStreamer.insertStringContent("All Average Trees");
  ioStreamer.closeTag(false);
  ioStreamer.insertStringContent("]");
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("object");
  ioStreamer.insertAttribute("data", lAVTName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.openTag("embed");
  ioStreamer.insertAttribute("src", lAVTName);
  ioStreamer.insertAttribute("type", "image/svg+xml");
  ioStreamer.insertAttribute("width", "600");
  ioStreamer.insertAttribute("height", "600");
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);

  // Set context value
  ioContext.mAverageTreeWritten[inDemeIndex+1][inGenoIndex+1][inGeneration] = lAVTName;
  
  // Write average tree to SVG file
  lAVTName = ioContext.mOutDir + std::string("/") + lAVTName;
#ifdef HAVE_LIBZ
  ogzstream lOFS(lAVTName.c_str());
#else  // HAVE_LIBZ
  std::ofstream lOFS(lAVTName.c_str());
#endif // HAVE_LIBZ
  lAVT.draw(lOFS);
  lOFS.close();
}


/*!
 *
 */
void BVDeme::writePopulation(const std::vector<BVIndividual>& inPopulation,
                             PACC::XML::Streamer& ioStreamer,
                             BVContextHTML& ioContext) const
{
  ioStreamer.openTag("center");
  ioStreamer.openTag("table");
  ioStreamer.insertAttribute("border", "0");
  ioStreamer.insertAttribute("width", "96%");
  for(unsigned int i=0; i<inPopulation.size(); ++i) {
    ioStreamer.openTag("tr");
    ioStreamer.openTag("td");
    if((i%2)==0) ioStreamer.insertAttribute("style", "background-color: #CCCCDD");
    else ioStreamer.insertAttribute("style", "background-color: #F5F5F5");
    inPopulation[i].write(ioStreamer, ioContext);
    ioStreamer.closeTag();
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
}

