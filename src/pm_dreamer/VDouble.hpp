/***************************************************************************
                                  VDouble.hpp
                             -------------------
                               W. Michael Brown

  Vector of Double

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

#ifndef VDouble_hpp
#define VDouble_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/GP.hpp"
#include <valarray>
#include <cassert>

namespace Beagle {

inline std::valarray<double> vec_to_array(const std::vector<Double> &vec) {
  std::valarray<double> array(vec.size());
  for (int i=0; i<vec.size(); i++) array[i]=vec[i];
  return array;
}
  
inline std::valarray<int> vec_to_array(const std::vector<int> &vec) {
  std::valarray<int> array(vec.size());
  for (int i=0; i<vec.size(); i++) array[i]=vec[i];
  return array;
}

class DArray {
 public:
  DArray() : terms(NULL) {}
  DArray(const double& in) : dvec(in,1), terms(NULL) {}
  DArray(const std::valarray<double> in) : dvec(in), terms(NULL) {}
  DArray(const std::vector<double> in) : dvec(in.size()), terms(NULL)
    { for (int i=0; i<in.size(); i++) dvec[i]=in[i]; }
  DArray(const DArray &in) : lookup(in.lookup) {
    if (in.terms!=NULL)
      terms=new std::vector<double>(*(in.terms));
    else
      terms=NULL;
    dvec.resize(in.dvec.size());
    dvec=in.dvec;
  }
  
  ~DArray() { if (terms!=NULL) { delete terms; } }

  inline double & operator[](int i) { return dvec[i]; }
  inline const double & operator[](int i) const { return dvec[i]; }
  inline int size() const { return dvec.size(); }
  template <class T> DArray & operator=(const T& in) { dvec=in; return *this;}
  bool operator==(const double value) { return dvec[0]==value; }
  template <class T> DArray & operator==(const T& in) { return dvec[0]==in[0]; }
  bool operator<(const double value) { return dvec[0]<value; }
  bool operator<(const DArray & value) { return dvec[0]<value[0]; }
  bool operator<(const std::valarray<double> & value) { return dvec[0]<value[0]; }

  DArray & operator=(const DArray &two) {
    lookup=two.lookup;
    if (terms!=NULL) { delete terms; terms=NULL; }
    if (two.terms!=NULL)
      terms=new std::vector<double>(*(two.terms));
    if (dvec.size()!=two.dvec.size())
      dvec.resize(two.dvec.size());
    dvec=two.dvec;
    return *this; 
  }
  DArray & operator=(const std::valarray<double> &two) {
    assert(terms==NULL);
    if (dvec.size()!=two.size())
      dvec.resize(two.size()); 
    dvec=two;
    return *this;
  }
  
  inline void set_terms(const std::vector<double> &_terms,
                 const std::vector<int> *_lookup) {
    if (terms!=NULL) delete terms;
    terms=new std::vector<double>(_terms);
    lookup=_lookup;
    int nV=lookup->size();
    dvec.resize(nV);
    for (int i=0; i<nV; i++)
      dvec[i]=terms->at(lookup->at(i));
  }
                    
  inline void update_dvec() { 
    int nV=lookup->size();
    for (int i=0; i<nV; i++)
      dvec[i]=terms->at(lookup->at(i));
  } 

  inline double slice_sum(const int s, int N) 
    { double a=0.0; N+=s; for (int i=s; i<N; i++) a+=dvec[i]; return a; }

  std::valarray<double> dvec;
  std::vector<double> *terms;
  const std::vector<int> *lookup;
};

inline std::ostream& operator<<(std::ostream& out, const DArray &p) {
  int oldp=out.precision(); 
  out.precision(16); 
  if (p.terms==NULL)
    out << p[0];
  else {
    int nt=p.terms->size()-1;
    for (int i=0; i<nt; i++)
      out << p.terms->at(i) << " ";
    out << p.terms->back();
  }
  out.precision(oldp); 
  return out;
}

inline std::istream& operator>>(std::istream& in, DArray &p) {
  if (p.terms!=NULL) {
    for (int i=0; i<p.terms->size(); i++)
      in >> p.terms->at(i); 
    int nV=p.lookup->size();
    for (int i=0; i<nV; i++)
      p.dvec[i]=p.terms->at(p.lookup->at(i));
  } else {
    double token;
    in >> token;
    p=token;
  }
  
  return in;
}

class VDouble : public WrapperT<DArray> {
 public:
  typedef AllocatorT< VDouble, Object::Alloc > Alloc;
  typedef PointerT< VDouble, Object::Handle > Handle;
  typedef ContainerT< VDouble, Object::Bag > Bag;

  VDouble() {}
  VDouble(const DArray& in) :
    WrapperT<DArray>(in) {}
  VDouble(const Double& in) :
    WrapperT<DArray>(DArray(in.getWrappedValue())) {}
  VDouble(const std::vector<double> terms, const std::vector<int> *lookup)
    { getWrappedValue().set_terms(terms,lookup); }    
  template <class T> VDouble(const T& in) : 
    WrapperT<DArray>(DArray(in)) {}
  ~VDouble() {}
  inline int size() const { return getWrappedValue().size(); }
  inline double& operator[](const int i) { return getWrappedValue()[i]; }
  inline const double& operator[](const int i) const { return getWrappedValue()[i]; }
  inline double sum() const { return getWrappedValue().dvec.sum(); }
  inline double max() const { return getWrappedValue().dvec.max(); }
  inline double min() const { return getWrappedValue().dvec.min(); }
  inline void resize(const int N) { getWrappedValue().dvec.resize(N); }
  inline double slice_sum(const int s, int N) 
    { return getWrappedValue().slice_sum(s,N); }

  operator double() { return getWrappedValue().dvec[0]; }
  
  inline double term(const int i) { return getWrappedValue().terms->at(i); }
  inline void set_term(const int i, const double v)
    { getWrappedValue().terms->at(i)=v; }
  inline void update_dvec() { getWrappedValue().update_dvec(); } 
  inline int num_terms() { return getWrappedValue().terms->size(); }
};

/*!
 *  \brief Evaluate absolute value of a Beagle::VDouble.
 *  \param inValue VDouble to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Adapt
 */
template <>
inline VDouble absolute(const VDouble& inValue)
{
  Beagle_StackTraceBeginM();
  return VDouble(std::abs(inValue.getWrappedValue().dvec));
  Beagle_StackTraceEndM("VDouble absolute(const VDouble& inValue)");
}

}

/*!
 *  \brief  Test whether an double is less than another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::VDouble.
 */
inline bool operator<(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.isLess(inRightVDouble);
  Beagle_StackTraceEndM("bool operator<(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief  Test whether an double is less than, or equal to another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::VDouble.
 */
inline bool operator<=(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return ( inLeftVDouble.isLess(inRightVDouble) || inLeftVDouble.isEqual(inRightVDouble) );
  Beagle_StackTraceEndM("bool operator<=(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief  Test whether an double is more than another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::VDouble.
 */
inline bool operator>(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inRightVDouble.isLess(inLeftVDouble);
  Beagle_StackTraceEndM("bool operator>(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief  Test whether an double is more than, or equal to another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::VDouble.
 */
inline bool operator>=(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return ( inRightVDouble.isLess(inLeftVDouble) || inLeftVDouble.isEqual(inRightVDouble) );
  Beagle_StackTraceEndM("bool operator>=(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief  Test whether an double is equal to another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::VDouble.
 */
inline bool operator==(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.isEqual(inRightVDouble);
  Beagle_StackTraceEndM("bool operator==(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief  Test whether an double is not equal to another.
 *  \param  inLeftVDouble  Left double compared.
 *  \param  inRightVDouble Right double compared.
 *  \return True if left double is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::VDouble.
 */
inline bool operator!=(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return ( inLeftVDouble.isEqual(inRightVDouble) == false);
  Beagle_StackTraceEndM("bool operator!=(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Compare equality of a VDouble with a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator==(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() == inRightVDouble;
  Beagle_StackTraceEndM("bool operator==(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Compare inequality of a VDouble with a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator!=(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() != inRightVDouble;
  Beagle_StackTraceEndM("bool operator!=(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Test if a VDouble is < than a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator<(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() < inRightVDouble;
  Beagle_StackTraceEndM("ool operator<(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Test if a VDouble is <= than a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator<=(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() <= inRightVDouble;
  Beagle_StackTraceEndM("bool operator<=(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Test if a VDouble is > than a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator>(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() > inRightVDouble;
  Beagle_StackTraceEndM("bool operator>(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Test if a VDouble is >= than a double.
 *  \param inLeftVDouble Left value to compare.
 *  \param inRightVDouble Right value to compare.
 *  \return Result of the comparison.
 
inline
bool operator>=(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue() >= inRightVDouble;
  Beagle_StackTraceEndM("bool operator>=(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Add two VDouble.
 *  \param inLeftVDouble Left value to add.
 *  \param inRightVDouble Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::VDouble operator+(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  const std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (l.size()==1)
    return(l[0]+r);
  else if (r.size()==1)
    return l+r[0];
  else
    return l+r;
  Beagle_StackTraceEndM("VDouble operator+(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Add a VDouble with a double.
 *  \param inLeftVDouble Left value to add.
 *  \param inRightVDouble Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::VDouble operator+(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue().dvec + inRightVDouble;
  Beagle_StackTraceEndM("VDouble operator+(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Add a VDouble to a VDouble.
 *  \param inLeftVDouble Left value to which the right one is added.
 *  \param inRightVDouble Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::VDouble operator+=(Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (r.size()==1)
    l+=r[0];
  else if (l.size()==1) {
    double t=l[0];
    l.resize(r.size());
    l=r+t;
  } else
    l+=r;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator+=(VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Add a double to a VDouble.
 *  \param inLeftVDouble Left value to which the right one is added.
 *  \param inRightVDouble Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::VDouble operator+=(Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  inLeftVDouble.getWrappedValue().dvec += inRightVDouble;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator+=(VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Subtract two VDouble.
 *  \param inLeftVDouble Left value to subtract.
 *  \param inRightVDouble Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::VDouble operator-(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  const std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (l.size()==1)
    return(l[0]-r);
  else if (r.size()==1)
    return l-r[0];
  else
    return l-r;
  Beagle_StackTraceEndM("VDouble operator-(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Subtract a double from a VDouble.
 *  \param inLeftVDouble Left value to subtract.
 *  \param inRightVDouble Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::VDouble operator-(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue().dvec - inRightVDouble;
  Beagle_StackTraceEndM("VDouble operator-(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Subtract a VDouble from a VDouble.
 *  \param inLeftVDouble Left value from which the right double is subtracted.
 *  \param inRightVDouble Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::VDouble operator-=(Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (r.size()==1)
    l-=r[0];
  else if (l.size()==1) {
    double t=l[0];
    l.resize(r.size());
    l=t-r;
  } else
    l-=r;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator-=(VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Subtract a double from a VDouble.
 *  \param inLeftVDouble Left value from which the right double is subtracted.
 *  \param inRightVDouble Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::VDouble operator-=(Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  inLeftVDouble.getWrappedValue().dvec -= inRightVDouble;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator-=(VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Multiply two VDouble.
 *  \param inLeftVDouble Left value to multiply.
 *  \param inRightVDouble Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::VDouble operator*(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  const std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (l.size()==1)
    return(l[0]*r);
  else if (r.size()==1)
    return l*r[0];
  else
    return l*r;
  Beagle_StackTraceEndM("VDouble operator*(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Multiply a VDouble with a double.
 *  \param inLeftVDouble Left value to multiply.
 *  \param inRightVDouble Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::VDouble operator*(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue().dvec * inRightVDouble;
  Beagle_StackTraceEndM("VDouble operator*(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Multiply a VDouble with a VDouble.
 *  \param inLeftVDouble Left value to which the right double is multiplied.
 *  \param inRightVDouble Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::VDouble operator*=(Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (r.size()==1)
    l*=r[0];
  else if (l.size()==1) {
    double t=l[0];
    l.resize(r.size());
    l=r*t;
  } else
    l*=r;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator*=(VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Multiply a VDouble with a double.
 *  \param inLeftVDouble Left value from which the right double is multiplied.
 *  \param inRightVDouble Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::VDouble operator*=(Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  inLeftVDouble.getWrappedValue().dvec *= inRightVDouble;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator*=(VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Divide two VDouble.
 *  \param inLeftVDouble Left value to divide.
 *  \param inRightVDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::VDouble operator/(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  const std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (l.size()==1)
    return(l[0]/r);
  else if (r.size()==1)
    return l/r[0];
  else
    return l/r;
  Beagle_StackTraceEndM("VDouble operator/(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Divide a VDouble with a double.
 *  \param inLeftVDouble Left value to divide.
 *  \param inRightVDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::VDouble operator/(const Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return inLeftVDouble.getWrappedValue().dvec / inRightVDouble;
  Beagle_StackTraceEndM("VDouble operator/(const VDouble& inLeftVDouble, double inRightVDouble)");
}


/*!
 *  \brief Divide a VDouble with a VDouble.
 *  \param inLeftVDouble Left value to which the right double is divided.
 *  \param inRightVDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::VDouble operator/=(Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (r.size()==1)
    l/=r[0];
  else if (l.size()==1) {
    double t=l[0];
    l.resize(r.size());
    l=t/r;
  } else
    l/=r;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator/=(VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}


/*!
 *  \brief Divide a VDouble with a double.
 *  \param inLeftVDouble Left value from which the right double is divided.
 *  \param inRightVDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::VDouble operator/=(Beagle::VDouble& inLeftVDouble, double inRightVDouble)
{
  Beagle_StackTraceBeginM();
  inLeftVDouble.getWrappedValue().dvec /= inRightVDouble;
  return inLeftVDouble;
  Beagle_StackTraceEndM("VDouble operator/=(VDouble& inLeftVDouble, double inRightVDouble)");
}

inline
Beagle::VDouble pow(const Beagle::VDouble& inLeftVDouble, const Beagle::VDouble& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  const std::valarray<double> &l=inLeftVDouble.getWrappedValue().dvec;
  const std::valarray<double> &r=inRightVDouble.getWrappedValue().dvec;
  if (l.size()==1)
    return pow(l[0],r);
  else if (r.size()==1)
    return pow(l,r[0]);
  else
    return pow(l,r);
  Beagle_StackTraceEndM("VDouble pow(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}

inline
Beagle::VDouble exp(const Beagle::VDouble& in)
{
  Beagle_StackTraceBeginM();
  return exp(in.getWrappedValue().dvec);
  Beagle_StackTraceEndM("VDouble pow(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}

inline
Beagle::VDouble cos(const Beagle::VDouble& in)
{
  Beagle_StackTraceBeginM();
  return cos(in.getWrappedValue().dvec);
  Beagle_StackTraceEndM("VDouble pow(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}

inline
Beagle::VDouble sin(const Beagle::VDouble& in)
{
  Beagle_StackTraceBeginM();
  return sin(in.getWrappedValue().dvec);
  Beagle_StackTraceEndM("VDouble pow(const VDouble& inLeftVDouble, const VDouble& inRightVDouble)");
}

inline
Beagle::Double pow(const Beagle::Double& inLeftVDouble, const Beagle::Double& inRightVDouble)
{
  Beagle_StackTraceBeginM();
  return pow(inLeftVDouble.getWrappedValue(),inRightVDouble.getWrappedValue());
  Beagle_StackTraceEndM("Double pow(const Double& inLeftVDouble, const Double& inRightVDouble)");
}

#endif
