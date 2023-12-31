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
 *  \file   KnapsackEvalOp.hpp
 *  \brief  Definition of the type KnapsackEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

/*!
 *  \defgroup Knapsack Multiobjective 0/1 Knapsack Example
 *  \brief Multiobjective 0/1 Knapsack (knapsack): Multiobjective GA example
 *
 *  \par Objective
 *  Find a combination of objects to put in a knapsack with a minimum weight but a maximum value.
 *
 *  \par Representation
 *  Bit strings made of 24 bits, where the ith bit designate whether the ith object is putted in the
 *  bag or not. The value and weight of each objects is randomly generated at each runs. 
 *
 *  \par Fitness
 *  First objective is the maximization of the knapsack's objects value. The second objective
 *  is the maximization of (maximum knapsack weight - objects weight), that is the minimization of
 *  the weight.
 *
 */
 
#ifndef KnapsackEvalOp_hpp
#define KnapsackEvalOp_hpp

#include "beagle/GA.hpp"

#include <vector>


/*!
 *  \class KnapsackEvalOp KnapsackEvalOp.hpp "KnapsackEvalOp.hpp"
 *  \brief The individual evaluation class operator for the Knapsack problem.
 *  \ingroup Knapsack
 */
class KnapsackEvalOp : public Beagle::EvaluationOp {

public:

  //! KnapsackEvalOp allocator type.
  typedef Beagle::AllocatorT<KnapsackEvalOp,Beagle::EvaluationOp::Alloc>
          Alloc;
  //!< KnapsackEvalOp handle type.
  typedef Beagle::PointerT<KnapsackEvalOp,Beagle::EvaluationOp::Handle>
          Handle;
  //!< KnapsackEvalOp bag type.
  typedef Beagle::ContainerT<KnapsackEvalOp,Beagle::EvaluationOp::Bag>
          Bag;

  explicit KnapsackEvalOp();

  virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
                                           Beagle::Context& ioContext);
  virtual void initialize(Beagle::System& ioSystem);
  virtual void postInit(Beagle::System& ioSystem);

protected:
  Beagle::Vector::Handle mObjectValues;    //!< Knapsack objects value.
  Beagle::Vector::Handle mObjectWeights;   //!< Knapsack objects weight.
  Beagle::UInt::Handle   mIntVectorSize;   //!< Individual integer vectors size.
  float                  mSumWeight;       //!< Sum of objects weight that can be in knapsack.

};

#endif // KnapsackEvalOp_hpp
