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
 *  \file   SymGPEvalOp.cpp
 *  \brief  Implementation of the class SymGPEvalOp.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Coev.hpp"
#include "CoSymEvalOp.hpp"
#include "TrainSetEvalOp.hpp"
#include "TrainSetThread.hpp"
#include "SymGPEvalOp.hpp"
#include "SymGPThread.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace Beagle;

/*!
 *  \brief Construct the GP individual evaluation operator for the co-evolutionary
 *    symbolic regression problem.
 */
SymGPEvalOp::SymGPEvalOp() :
  CoSymEvalOp("SymGPEvalOp")
{ }


/*!
 *  \brief Make evaluation sets to evaluate fitness on SymGp.
 *  \param ioIndivBag Bag of individuals to use for evaluation.
 *  \param ioContext Evaluationary context.
 */
void SymGPEvalOp::makeSets(Individual::Bag& ioIndivBag, Context::Handle ioContext)
{
  // If last generation best individual is NULL handle, choose a random individual.
  if(mLastGenBestIndividual == NULL) {
    unsigned int lRandomIndex =
      ioContext->getSystem().getRandomizer().rollInteger(0, ioContext->getDeme().size()-1);
    GP::Individual::Alloc::Handle lIndivAlloc =
      castHandleT<GP::Individual::Alloc>(ioContext->getDeme().getTypeAlloc());
    GP::Individual::Handle lIndivToCopy =
      castHandleT<GP::Individual>(ioContext->getDeme()[lRandomIndex]);
    mLastGenBestIndividual = castHandleT<GP::Individual>(lIndivAlloc->cloneData(*lIndivToCopy));
  }

  // Eval set for GP Symbolic equation
  EvalSet lSymGPEvalSet(ioIndivBag, ioContext, 0);
  addSet(lSymGPEvalSet, false);

  // Eval set of best GP individual
  GP::Individual::Bag lBestSymGPIndiv;
  lBestSymGPIndiv.push_back(mLastGenBestIndividual);
  EvalSet lBestSymGPEvalSet(lBestSymGPIndiv, ioContext, 1);
  addSet(lBestSymGPEvalSet, true);

  // Get a copy of the best individual for next generation
  unsigned int lBestIndivIndex = 0;
  float lBestIndivIndexFits =
    castHandleT<FitnessSimple>(ioContext->getDeme()[0]->getFitness())->getValue();
  for(unsigned int i=1; i<ioContext->getDeme().size(); ++i) {
    float lFitness =
      castHandleT<FitnessSimple>(ioContext->getDeme()[i]->getFitness())->getValue();
    if(lFitness < lBestIndivIndexFits) {
      lBestIndivIndexFits = lFitness;
      lBestIndivIndex = i;
    }
  }
  GP::Individual::Handle lIndivToCopy =
    castHandleT<GP::Individual>(ioContext->getDeme()[lBestIndivIndex]);
  GP::Individual::Alloc::Handle lIndivAlloc =
    castHandleT<GP::Individual::Alloc>(ioContext->getDeme().getTypeAlloc());
  mLastGenBestIndividual = castHandleT<GP::Individual>(lIndivAlloc->cloneData(*lIndivToCopy));
}


