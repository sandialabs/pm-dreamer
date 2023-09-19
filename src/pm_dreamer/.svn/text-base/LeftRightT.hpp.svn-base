/***************************************************************************
                                LeftRightT.hpp
                             -------------------
                               W. Michael Brown

  Primitive for separate evaluation and storage of left and right subtrees

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : July 17 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/
 
/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef LeftRightT_hpp
#define LeftRightT_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle {
namespace GP {

template <class T>
class LeftRightT : public Primitive {
 public:
  typedef AllocatorT<LeftRightT<T>,Primitive::Alloc>
          Alloc;
  typedef PointerT<LeftRightT<T>,Primitive::Handle>
          Handle;
  typedef ContainerT<LeftRightT<T>,Primitive::Bag>
          Bag;

  explicit LeftRightT(string inName="LeftRight", unsigned num_args=2);
  virtual ~LeftRightT() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  // Return the result from the left subtree
  void execute(GP::Datum& outResult, GP::Context& ioContext);
  // Return the result from the right subtree
  void getValue(Object &outValue);
  // True if more than 1 arg in expression tree
  bool haveValue() const;
  void setValue(const Object& inValue);
 protected:
  unsigned eval_side; // 0 to evaluate left tree; // 1 to evaluate right tree
};

typedef LeftRightT<Double> LeftRight;

}
}

template <class T>
Beagle::GP::LeftRightT<T>::LeftRightT(string inName, unsigned num_args) :
  Beagle::GP::Primitive(num_args, inName), eval_side(0)
{ }

#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive LeftRightT.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 1.
 */
template <class T>
const std::type_info* Beagle::GP::LeftRightT<T>::getArgType(unsigned int inN,
                                                      Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inN<this->getNumberArguments());
  return &typeid(T);
  Beagle_StackTraceEndM("const std::type_info* GP::LeftRightT<T>::getArgType(unsigned int inN, GP::Context& ioContext) const");
}


/*!
 *  \brief  Return the tag of the type of data return by primitive LeftRightT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::LeftRightT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  return &typeid(T);
  Beagle_StackTraceEndM("const std::type_info* GP::LeftRightT<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Return the evaluation of the left subtree
 *  \param outResult Result
 *  \param ioContext Evolutionary context.
 */
template <class T>
void Beagle::GP::LeftRightT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  T& lResult = castObjectT<T&>(outResult);
  
  if (eval_side>0)
    get2ndArgument(lResult, ioContext); 
  else if (this->getNumberArguments()==0)
    lResult=T(0);
  else
    get1stArgument(lResult, ioContext);

  Beagle_StackTraceEndM("void GP::LeftRightT<T>::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

template <class T>
bool Beagle::GP::LeftRightT<T>::haveValue() const
{
  return false;
}

template <class T>
void Beagle::GP::LeftRightT<T>::getValue(Object &outValue)
{
  castObjectT<UInt&>(outValue)=eval_side;
}

template <class T>
void Beagle::GP::LeftRightT<T>::setValue(const Object &inValue)
{
  eval_side=castObjectT<const UInt&>(inValue);
}

#endif
