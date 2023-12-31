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
 *  \file   beagle/GP/StatsCalcFitnessKozaOp.hpp
 *  \brief  Definition of the class StatsCalcFitnessKozaOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:05 $
 */

#ifndef Beagle_GP_StatsCalcFitnessKozaOp_hpp
#define Beagle_GP_StatsCalcFitnessKozaOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"
#include "beagle/StatsCalculateOp.hpp"

namespace Beagle {

namespace GP {

/*!
 *  \class StatsCalcFitnessKozaOp beagle/GP/StatsCalcFitnessKozaOp.hpp
 *     "beagle/GP/StatsCalcFitnessKozaOp.hpp"
 *  \brief Calculate statistics of a GP deme, for a generation, operator class.
 *  \ingroup GPF
 */
class StatsCalcFitnessKozaOp : public StatsCalculateOp {

public:

  //! StatsCalcFitnessKozaOp allocator type
  typedef AllocatorT<StatsCalcFitnessKozaOp,StatsCalculateOp::Alloc>
          Alloc;
  //! StatsCalcFitnessKozaOp handle type.
  typedef PointerT<StatsCalcFitnessKozaOp,StatsCalculateOp::Handle>
          Handle;
  //! StatsCalcFitnessKozaOp bag type.
  typedef ContainerT<StatsCalcFitnessKozaOp,StatsCalculateOp::Bag>
          Bag;

  explicit StatsCalcFitnessKozaOp(string inName="GP-StatsCalcFitnessKozaOp");
  virtual ~StatsCalcFitnessKozaOp() { }

  virtual void calculateStatsDeme(Beagle::Stats& outStats,
                                  Beagle::Deme& ioDeme,
                                  Beagle::Context& ioContext) const;

};

}

}

#endif // Beagle_GP_StatsCalcFitnessKozaOp_hpp
