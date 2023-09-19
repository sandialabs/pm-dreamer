/***************************************************************************
                                  VDivide.hpp
                             -------------------
                               W. Michael Brown

  Protected division for vector of double

 __________________________________________________________________________
    This file is part of the PM-Dreamer project
 __________________________________________________________________________

    begin                : Aug 7 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef VDivide_hpp
#define VDivide_hpp

#include <cmath>
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

class VDivide : public Primitive {
public:
  typedef AllocatorT<VDivide,Primitive::Alloc>
          Alloc;
  typedef PointerT<VDivide,Primitive::Handle>
          Handle;
  typedef ContainerT<VDivide,Primitive::Bag>
          Bag;

  explicit VDivide(string inName="DIV");
  virtual ~VDivide() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

}
}

Beagle::GP::VDivide::VDivide(string inName) :
  Beagle::GP::Primitive(2, inName)
{ }

#ifdef BEAGLE_HAVE_RTTI

const std::type_info* Beagle::GP::VDivide::getArgType(unsigned int inN,
                                                      Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inN<2);
  return &typeid(VDouble);
  Beagle_StackTraceEndM("const std::type_info* GP::VDivide<T>::getArgType(unsigned int inN, GP::Context& ioContext) const");
}

const std::type_info* Beagle::GP::VDivide::getReturnType(Beagle::GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  return &typeid(VDouble);
  Beagle_StackTraceEndM("const std::type_info* GP::VDivide<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI

void Beagle::GP::VDivide::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  VDouble& lResult = castObjectT<VDouble&>(outResult);
  VDouble lArg2;
  get1stArgument(lResult, ioContext);
  get2ndArgument(lArg2, ioContext);
  DArray& lR=lResult.getWrappedValue();
  DArray& lA2=lArg2.getWrappedValue();
  if (lA2.size()==1)
    if((lA2[0] < 0.001) && (-0.001 < lA2[0])) {
      lR.dvec = std::valarray<double>(1.0,lR.size());
      return;
    }
  lResult/=lArg2;
  for (int i=0; i<lA2.size(); i++)
    if((lA2[i] < 0.001) && (-0.001 < lA2[i])) 
      lR[i] = 1.0;
  Beagle_StackTraceEndM("void GP::VDivide<T>::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

#endif
