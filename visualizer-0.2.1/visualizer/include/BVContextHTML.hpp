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

#ifndef BVContextHTML_hpp
#define BVContextHTML_hpp

#include <ostream>
#include <map>
#include <set>
#include <string>
#include "XML/Streamer.hpp"

/*!
 *
 */
class BVContextHTML {

public:

  typedef std::map< std::string,std::set<std::string> >
          StatsMap;
  typedef std::map<unsigned int,std::map<unsigned int,std::map<unsigned int,std::string> > >
          AVTMap;

  enum State {eInactive, eActive, eInProcess};

  BVContextHTML(std::string inOutDir=".",
                std::string inBaseURL="http://beagle.gel.ulaval.ca/visualizer");
  ~BVContextHTML() { }

  void writeFooter(PACC::XML::Streamer& ioStreamer) const;
  void writeHeader(PACC::XML::Streamer& ioStreamer) const;

  BVContextHTML::State  mLogsState;
  BVContextHTML::State  mStatsState;
  BVContextHTML::State  mVivariumState;
  BVContextHTML::State  mEvolverState;
  BVContextHTML::State  mRegisterState;
  BVContextHTML::State  mSeedsState;
  std::string           mOutDir;
  std::string           mBaseURL;
  std::string           mDate;

  StatsMap              mMeasuresRead;
  StatsMap              mItemsRead;
  AVTMap                mAverageTreeWritten;
    
};

#endif // BVContextHTML_hpp
