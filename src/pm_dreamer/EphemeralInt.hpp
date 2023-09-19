/***************************************************************************
                              EphemeralInt.hpp
                             -------------------
                               W. Michael Brown

  Integer constant primitive in [-20,20]

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

/// Integer constant primitive in [-20,20]
class EphemeralInt : public EphemeralT<Double> {

public:
  typedef AllocatorT<EphemeralInt,EphemeralT<Double>::Alloc>
          Alloc;
  typedef PointerT<EphemeralInt,EphemeralT<Double>::Handle>
          Handle;
  typedef ContainerT<EphemeralInt,EphemeralT<Double>::Bag>
          Bag;

  explicit inline EphemeralInt(Double::Handle inValue=NULL, string inName="E_i");
  ~EphemeralInt() { }

  inline GP::Primitive::Handle generate(string inName, GP::Context& ioContext);

};

class VEphemeralInt : public EphemeralT<VDouble> {

public:
  typedef AllocatorT<VEphemeralInt,EphemeralT<VDouble>::Alloc>
          Alloc;
  typedef PointerT<VEphemeralInt,EphemeralT<VDouble>::Handle>
          Handle;
  typedef ContainerT<VEphemeralInt,EphemeralT<VDouble>::Bag>
          Bag;

  explicit inline VEphemeralInt(VDouble::Handle inValue=NULL, string inName="E_i");
  ~VEphemeralInt() { }

  inline GP::Primitive::Handle generate(string inName, GP::Context& ioContext);

};

}
}

Beagle::GP::EphemeralInt::EphemeralInt(Beagle::Double::Handle inValue, Beagle::string inName) :
  EphemeralT<Beagle::Double>(inValue, inName)
{ }

Beagle::GP::Primitive::Handle Beagle::GP::EphemeralInt::generate(Beagle::string inName, Beagle::GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle::Double::Handle lValue = new Beagle::Double(Beagle::Int(int(ioContext.getSystem().getRandomizer().rollUniform(-20.,20.))));
  return new EphemeralInt(lValue, inName);
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::EphemeralInt::generate(string inName, GP::Context& ioContext)");
}

Beagle::GP::VEphemeralInt::VEphemeralInt(Beagle::VDouble::Handle inValue, Beagle::string inName) :
  EphemeralT<Beagle::VDouble>(inValue, inName)
{ }

Beagle::GP::Primitive::Handle Beagle::GP::VEphemeralInt::generate(Beagle::string inName, Beagle::GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle::VDouble::Handle lValue = new Beagle::VDouble(Beagle::Int(int(ioContext.getSystem().getRandomizer().rollUniform(-20.,20.))));
  return new VEphemeralInt(lValue, inName);
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::EphemeralInt::generate(string inName, GP::Context& ioContext)");
}

#endif
