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
 *  \file   beagle/src/BadCastException.cpp
 *  \brief  Implementation of class BadCastException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new BadCastException using data given.
 *  \param inMessage    Message related to the exception.
 *  \param inFileName   File name where the exception is detected (throwed).
 *  \param inLineNumber Line number where the exception is detected (throwed).
 */
BadCastException::BadCastException(Beagle::string inMessage,
                                   Beagle::string inFileName,
                                   unsigned int   inLineNumber) :
  TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* BadCastException::getExceptionName() const throw()
{
  return "Beagle::BadCastException";
}

