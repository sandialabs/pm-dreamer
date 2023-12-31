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
 *  \file   beagle/IsEqualPointerPredicate.hpp
 *  \brief  Definition of class IsEqualPointerPredicate.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5.2.1 $
 *  $Date: 2007/05/09 01:51:14 $
 */

#ifndef Beagle_IsEqualPointerPredicate_hpp
#define Beagle_IsEqualPointerPredicate_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"

namespace Beagle {


/*!
 *  \class IsEqualPointerPredicate beagle/IsEqualPointerPredicate.hpp
 *    "beagle/IsEqualPointerPredicate.hpp"
 *  \brief Binary predicate functor that test if two pointed Objects are equals.
 *  \ingroup OOF
 *  \ingroup Utils
 *  \par Note:
 *    The IsEqualPointerPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsEqualPointerPredicate {

public:
  IsEqualPointerPredicate() { }
  ~IsEqualPointerPredicate() { }

  /*!
   *  \brief Test if the Objects pointed are equal.
   *  \param inLeftPtr Pointer to the left object to compare.
   *  \param inRightPtr Pointer to the right object to compare.
   */
  inline bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const
  {
    Beagle_StackTraceBeginM();
    if( (!inLeftPtr) && (!inRightPtr) ) return true;
    else if( (!inLeftPtr) || (!inRightPtr) ) return false;
    return inLeftPtr->isEqual(*inRightPtr);
    Beagle_StackTraceEndM("bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const");
  }

};

}

#endif // Beagle_IsEqualPointerPredicate_hpp
