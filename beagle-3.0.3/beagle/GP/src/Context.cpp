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
 *  \file   beagle/GP/src/Context.cpp
 *  \brief  Source code of class GP::Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision:
 *  $Date: 2007/05/09 01:51:06 $
 */


#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Default constructor of GP evolutionary context.
 */
GP::Context::Context() : 
  mAllowedExecutionTime(0.0),
  mNodesExecutionCount(0), 
  mAllowedNodesExecution(UINT_MAX) 
{ }

