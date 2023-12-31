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
 *  \file   EphemeralPercent.hpp
 *  \brief  Definition of the EphemeralPercent primitive used in the spambase problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:23 $
 */

#ifndef EphemeralPercent_hpp
#define EphemeralPercent_hpp

#include "beagle/GP.hpp"
#include <string>


/*!
 *  \class EphemeralPercent EphemeralPercent.hpp "EphemeralPercent.hpp"
 *  \brief Ephemeral constant of Double randomly generated in [0,100].
 *  \ingroup Spambase
 */
class EphemeralPercent : public Beagle::GP::EphemeralT<Beagle::Double> {

public:

  //! EphemeralPercent allocator type.
  typedef Beagle::AllocatorT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Alloc>
          Alloc;
  //!< EphemeralPercent handle type.
  typedef Beagle::PointerT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Handle>
          Handle;
  //!< EphemeralPercent bag type.
  typedef Beagle::ContainerT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Bag>
          Bag;

  explicit EphemeralPercent(Beagle::Double::Handle inValue=NULL);
  virtual ~EphemeralPercent() { }

  virtual Beagle::GP::Primitive::Handle generate(Beagle::string inName, Beagle::GP::Context& ioContext);

};


#endif // EphemeralPercent_hpp
