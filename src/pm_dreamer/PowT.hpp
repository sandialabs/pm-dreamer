/***************************************************************************
                                  PowT.hpp
                             -------------------
                               W. Michael Brown

  Power (x^y) operation

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Jun 9 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef PowT_hpp
#define PowT_hpp

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

/// Pow (x^y) primitive operation
template <class T>
class PowT : public Primitive {
 public:
  typedef AllocatorT<PowT<T>,Primitive::Alloc>
          Alloc;
  typedef PointerT<PowT<T>,Primitive::Handle>
          Handle;
  typedef ContainerT<PowT<T>,Primitive::Bag>
          Bag;

  explicit PowT(string inName="POW");
  virtual ~PowT() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

typedef PowT<Double> Pow;

}
}

template <class T>
Beagle::GP::PowT<T>::PowT(string inName) :
  Beagle::GP::Primitive(2, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

template <class T>
const std::type_info* Beagle::GP::PowT<T>::getArgType(unsigned int inN,
                                                      Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inN<2);
  return &typeid(T);
  Beagle_StackTraceEndM("const std::type_info* GP::PowT<T>::getArgType(unsigned int inN, GP::Context& ioContext) const");
}


template <class T>
const std::type_info* Beagle::GP::PowT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  return &typeid(T);
  Beagle_StackTraceEndM("const std::type_info* GP::PowT<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI

template <class T>
void Beagle::GP::PowT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  T& lResult = castObjectT<T&>(outResult);
  get1stArgument(lResult, ioContext);
  T lArg2;
  get2ndArgument(lArg2, ioContext);
  lResult=pow(lResult,castObjectT<T&>(lArg2));
  Beagle_StackTraceEndM("void GP::PowT<T>::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

#endif
