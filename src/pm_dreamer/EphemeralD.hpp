/***************************************************************************
                               EphemeralD.hpp
                             -------------------
                               W. Michael Brown

  Double constant primitive in [-20,20]

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

#ifndef EphemeralD_hpp
#define EphemeralD_hpp

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
#include "VDouble.hpp"


namespace Beagle {
namespace GP {

/// Integer constant primitive in [-20,20]
class EphemeralD : public EphemeralT<Double> {

public:
  typedef AllocatorT<EphemeralD,EphemeralT<Double>::Alloc>
          Alloc;
  typedef PointerT<EphemeralD,EphemeralT<Double>::Handle>
          Handle;
  typedef ContainerT<EphemeralD,EphemeralT<Double>::Bag>
          Bag;

  explicit inline EphemeralD(Double::Handle inValue=NULL, string inName="E_d");
  virtual ~EphemeralD() { }

  virtual inline GP::Primitive::Handle generate(string inName, GP::Context& ioContext);

};

/// Floating point constant primitive in [-20,20]
class VEphemeralD : public EphemeralT<VDouble> {

public:
  typedef AllocatorT<VEphemeralD,EphemeralT<VDouble>::Alloc>
          Alloc;
  typedef PointerT<VEphemeralD,EphemeralT<VDouble>::Handle>
          Handle;
  typedef ContainerT<VEphemeralD,EphemeralT<VDouble>::Bag>
          Bag;

  explicit inline VEphemeralD(VDouble::Handle inValue=NULL, string inName="E_d");
  inline VEphemeralD(const int nterms, const std::vector<int> *lookup, 
                     VDouble::Handle inValue=NULL, string inName="E_p");
  inline VEphemeralD(const VEphemeralD &in, VDouble::Handle inValue=NULL);
  virtual ~VEphemeralD() { }

  virtual inline GP::Primitive::Handle generate(string inName, GP::Context& ioContext);
  
  inline void set_term(const int i, const double v) 
    { castHandleT<VDouble>(mValue)->set_term(i,v); }
  inline void update_dvec() 
    { castHandleT<VDouble>(mValue)->update_dvec(); }
  inline int num_terms() { return _nterms; }
  
  int _nterms;
  const std::vector<int> * _lookup;
};

}
}

using namespace Beagle;
using namespace GP;

GP::EphemeralD::EphemeralD(Double::Handle inValue, Beagle::string inName) :
  EphemeralT<Double>(inValue, inName)
{ }

GP::Primitive::Handle GP::EphemeralD::generate(Beagle::string inName, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Double::Handle lValue = new Double(ioContext.getSystem().getRandomizer().rollUniform(-20.,20.));
  return new EphemeralD(lValue, inName);
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::EphemeralD::generate(string inName, GP::Context& ioContext)");
}


GP::VEphemeralD::VEphemeralD(VDouble::Handle inValue, Beagle::string inName) :
  EphemeralT<VDouble>(inValue, inName), _nterms(1)
{ }

GP::VEphemeralD::VEphemeralD(const int nterms, const std::vector<int> *lookup, 
                             VDouble::Handle inValue, std::string inName) :
  EphemeralT<VDouble>(inValue, inName), _nterms(nterms), _lookup(lookup) {
}

GP::VEphemeralD::VEphemeralD(const VEphemeralD &in, VDouble::Handle inValue) :
  EphemeralT<VDouble>(inValue, in.getName()), _nterms(in._nterms), 
  _lookup(in._lookup) {
}

GP::Primitive::Handle GP::VEphemeralD::generate(Beagle::string inName, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if (_nterms==1) {
    VDouble::Handle lValue = new VDouble(ioContext.getSystem().getRandomizer().rollUniform(-20.,20.));
    return new VEphemeralD(lValue, inName);
  } else {
    std::vector<double> terms;
    for (int i=0; i<_nterms; i++)
      terms.push_back(ioContext.getSystem().getRandomizer().
                      rollUniform(-20.,20.));
    VDouble::Handle lValue = new VDouble(terms,_lookup);
    return new VEphemeralD(_nterms, _lookup, lValue, inName);
  }
  Beagle_StackTraceEndM("GP::Primitive::Handle GP::EphemeralD::generate(string inName, GP::Context& ioContext)");
}

#endif

