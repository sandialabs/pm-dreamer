/***************************************************************************
                              EphemeralInt.hpp
                             -------------------
                               W. Michael Brown

  Integer constant primitive in [-20,20]

 __________________________________________________________________________
    This file is part of the gp_force project
 __________________________________________________________________________

    begin                : Jun 9 2008
    copyright            : (C) 2008 by W. Michael Brown
    email                : wmbrown@sandia.gov
 ***************************************************************************/

#ifndef EphemeralInt_hpp
#define EphemeralInt_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Int.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"
#include "beagle/GP/EphemeralT.hpp"


namespace Beagle {
namespace GP {

/// Integer constant primitive in [-20,20] or [-12,-6]
class EphemeralInt : public EphemeralT<Double> {

public:
  typedef AllocatorT<EphemeralInt,EphemeralT<Double>::Alloc>
          Alloc;
  typedef PointerT<EphemeralInt,EphemeralT<Double>::Handle>
          Handle;
  typedef ContainerT<EphemeralInt,EphemeralT<Double>::Bag>
          Bag;

  explicit EphemeralInt(Double::Handle inValue=NULL, string inName="E_i");
  virtual ~EphemeralInt() { }

  virtual GP::Primitive::Handle generate(string inName, GP::Context& ioContext);

};

}
}

using namespace Beagle;
using namespace GP;

GP::EphemeralInt::EphemeralInt(Double::Handle inValue, Beagle::string inName) :
  EphemeralT<Double>(inValue, inName)
{ }

GP::Primitive::Handle GP::EphemeralInt::generate(Beagle::string inName, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  //  Double::Handle lValue = new Double(Int(ioContext.getSystem().getRandomizer().rollUniform(-20.0,20.0)));
  Double::Handle lValue = new Double(Int(ioContext.getSystem().getRandomizer().rollUniform(-13.0,5.0)));
  //  Double::Handle lValue = new Double(Int(-ioContext.getSystem().getRandomizer().rollInteger(6,12)));
  return new EphemeralInt(lValue, inName);
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::EphemeralInt::generate(string inName, GP::Context& ioContext)");
}

#endif
