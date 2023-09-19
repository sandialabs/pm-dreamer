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

#ifndef BVRegister_hpp
#define BVRegister_hpp

#include <map>
#include <string>
#include <iostream>
#include "XML/Iterator.hpp"
#include "XML/Streamer.hpp"
#include "BVContextHTML.hpp"

/*!
 *
 */
class BVRegister {

public:

  BVRegister() { }
  ~BVRegister() { }

  void read(PACC::XML::ConstIterator inIter);
  void write(PACC::XML::Streamer& ioStreamer, BVContextHTML& ioContext) const;
  
private:
  
  std::map<std::string,std::string> mParameters;

};

#endif // BVRegister_hpp

