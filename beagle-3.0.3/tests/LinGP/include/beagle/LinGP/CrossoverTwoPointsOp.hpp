/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/CrossoverTwoPointsOp.hpp
 *  \brief  Definition of the class LinGP::CrossoverTwoPointsOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#ifndef Beagle_LinGP_CrossoverTwoPointsOp_hpp
#define Beagle_LinGP_CrossoverTwoPointsOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverTwoPointsOpT.hpp"
#include "beagle/LinGP/Program.hpp"

namespace Beagle {
namespace LinGP {

/*!
 *  \class CrossoverTwoPointsOp beagle/LinGP/CrossoverTwoPointsOp.hpp
 *    "beagle/LinGP/CrossoverTwoPointsOp.hpp"
 *  \brief Linear GP two points crossover operator class.
 *  \ingroup LinGPF
 */
class CrossoverTwoPointsOp : public GA::CrossoverTwoPointsOpT<Program> {

public:

  //! LinGP::CrossoverTwoPointsOp allocator type.
  typedef AllocatorT<CrossoverTwoPointsOp,GA::CrossoverTwoPointsOpT<Program>::Alloc>
          Alloc;
  //! LinGP::CrossoverTwoPointsOp handle type.
  typedef PointerT<CrossoverTwoPointsOp,GA::CrossoverTwoPointsOpT<Program>::Handle>
          Handle;
  //! LinGP::CrossoverTwoPointsOp bag type.
  typedef ContainerT<CrossoverTwoPointsOp,GA::CrossoverTwoPointsOpT<Program>::Bag>
          Bag;

  /*!
   *  \brief Construct linear GP two points crossover operator.
   *  \param inMatingPbName Name of the mating probability parameter used in the register.
   *  \param inName Name of the linear GP two points crossover operator.
   */
  explicit CrossoverTwoPointsOp(Beagle::string inMatingPbName="lingp.cx2p.prob",
                                Beagle::string inName="LinGP-CrossoverTwoPointsOp") :
    GA::CrossoverTwoPointsOpT<Program>(inMatingPbName, inName)
  { }

  virtual ~CrossoverTwoPointsOp() { }

};

}
}

#endif // Beagle_LinGP_CrossoverTwoPointsOp_hpp
