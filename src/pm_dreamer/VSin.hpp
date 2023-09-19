/***************************************************************************
                                   VSin.hpp
                             -------------------
                               W. Michael Brown

  Sine for vector of double

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

#ifndef VSin_hpp
#define VSin_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "VDouble.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle {
namespace GP {

class VSin : public Primitive {
 public:
  typedef AllocatorT<VSin,Primitive::Alloc>
          Alloc;
  typedef PointerT<VSin,Primitive::Handle>
          Handle;
  typedef ContainerT<VSin,Primitive::Bag>
          Bag;

  explicit VSin(string inName="SIN");
  virtual ~VSin() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

}
}

using namespace Beagle;

GP::VSin::VSin(Beagle::string inName) :
  Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

const std::type_info* GP::VSin::getArgType(unsigned int inN, GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inN<1);
  return &typeid(VDouble);
  Beagle_StackTraceEndM("const std::type_info* GP::VSin::getArgType(unsigned int inN, GP::Context& ioContext) const");
}


const std::type_info* GP::VSin::getReturnType(GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  return &typeid(VDouble);
  Beagle_StackTraceEndM("const std::type_info* GP::VSin::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI

void GP::VSin::execute(GP::Datum& outResult, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  VDouble& lResult = castObjectT<VDouble&>(outResult);
  get1stArgument(lResult, ioContext);
  lResult.getWrappedValue().dvec = sin(lResult.getWrappedValue().dvec);
  Beagle_StackTraceEndM("void GP::VSin::execute(GP::Datum& outResult, GP::Context& ioContext)");
}

#endif
