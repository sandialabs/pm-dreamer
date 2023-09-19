/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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

/*!
 *  \file   beagle/GP/InitHalfOp.hpp
 *  \brief  Definition of the class GP::InitHalfOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.2 $
 *  $Date: 2007/05/11 21:32:36 $
 */

#ifndef Beagle_GP_InitHalfOp_hpp
#define Beagle_GP_InitHalfOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/GP/InitFullOp.hpp"
#include "beagle/GP/InitGrowOp.hpp"


namespace Beagle {
namespace GP {

/*!
 *  \class InitHalfOp beagle/GP/InitHalfOp.hpp "beagle/GP/InitHalfOp.hpp"
 *  \brief GP trees "half-and-half" initialization operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPPop
 */
class InitHalfOp : public GP::InitializationOp {

public:

  //! GP::InitHalfOp allocator type.
  typedef AllocatorT<InitHalfOp,InitializationOp::Alloc>
          Alloc;
  //! GP::InitHalfOp handle type.
  typedef PointerT<InitHalfOp,InitializationOp::Handle>
          Handle;
  //! GP::InitGrowOp bag type.
  typedef ContainerT<InitHalfOp,InitializationOp::Bag>
          Bag;

  explicit InitHalfOp(std::string inReproProbaName="ec.repro.prob",
                      std::string inName="GP-InitHalfOp");
  virtual ~InitHalfOp() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual unsigned int initTree(GP::Tree &outTree, 
                                unsigned int inMinDepth,
                                unsigned int inMaxDepth,
                                GP::Context &ioContext) const;

protected:

  GP::InitFullOp mInitFullOp;  //!< Full initialization operator.
  GP::InitGrowOp mInitGrowOp;  //!< Grow initialization operator.

};

}
}

#endif // Beagle_GP_InitHalfOp_hpp
