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
 *  \file   beagle/macros.hpp
 *  \brief  Contains some general Beagle macros and global functions.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.2 $
 *  $Date: 2007/05/11 21:49:00 $
 */

/*!
 *  \defgroup Utils Utilities
 *  \ingroup OOF
 *  \brief Miscelaneous utility classes and functions, part of the Object Oriented Foundations.
 */

#ifndef Beagle_macros_hpp
#define Beagle_macros_hpp

#include <cmath>

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "beagle/config.hpp"

#ifdef BEAGLE_HAVE_NUMERIC_LIMITS
#include <limits>
#endif // BEAGLE_HAVE_NUMERIC_LIMITS


// An example on how to change the STL container allocator used.
// #include <ext/new_allocator.h>
// #define BEAGLE_STLALLOCATOR __gnu_cxx::new_allocator


/*
 *  Define a spare bool type if it is not defined by the compiler.
 */
#ifndef BEAGLE_HAVE_BOOL
enum boolvals {false=0, true=1};
typedef enum boolvals bool;
#endif // BEAGLE_HAVE_BOOL


/*
 *  Define an dumb explicit keyword if it is not defined by the compiler.
 */
#ifndef BEAGLE_HAVE_EXPLICIT
#define explicit
#endif // BEAGLE_HAVE_EXPLICIT


/*!
 *  \def    Beagle_MinM(X1,X2)
 *  \brief  Calculate the minimum between two values.
 *  \param  X1 First value to compare.
 *  \param  X2 Second value to compare.
 *  \return Minimum of the two values.
 *  \deprecated Use inline function minOf instead.
 */
#define Beagle_MinM(X1,X2) ( ((X1)<(X2)) ? (X1) : (X2) )


/*!
 *  \def    Beagle_MaxM(X1,X2)
 *  \brief  Calculate the maximum between two values.
 *  \param  X1 First value to compare.
 *  \param  X2 Second value to compare.
 *  \return Maximum of the two values.
 *  \deprecated Use inline function maxOf instead.
 */
#define Beagle_MaxM(X1,X2) ( ((X1)>(X2)) ? (X1) : (X2) )

/*!
 *  \def    Beagle_Pow2M(X1,X2)
 *  \brief  Calculate the square of a value.
 *  \param  X Value to compute the square.
 *  \return Square of value.
 *  \deprecated Use inline function pow2Of instead.
 */
#define Beagle_Pow2M(X) ( (X)*(X) )


namespace Beagle {


/*!
 *  \def BEAGLE_STLALLOCATOR
 *  \brief STL allocator used to instanciate STL container elements.
 *
 *  The STL allocator type can be redefined by defining the preprocessor flag
 *  BEAGLE_STLALLOCATOR to the proper STL allocator.
 */
#ifndef BEAGLE_STLALLOCATOR
#define BEAGLE_STLALLOCATOR std::allocator
#endif // BEAGLE_STLALLOCATOR


/*!
 *  \brief Typedef of the correct STL string to use.
 *
 *  By default, if the the STL allocator is used unchanged (std::allocator), this type is a
 *  synonym of the string type.
 */
typedef std::basic_string< char, std::char_traits<char>, BEAGLE_STLALLOCATOR<char> > string;


/*!
 *  \brief Test if a floating-point number is in a not-a-number state.
 *  \param inValue Value to evaluate not-a-number state.
 *  \return True if the floating-point value is not-a-number, otherwise false.
 *  \ingroup Utils
 */
template <class T>
inline bool isNaN(const T& inValue)
{
  return ((inValue == inValue) == false);
}


/*!
 *  \brief Test if a floating-point number is infinite.
 *  \param inValue Value to evaluate finiteness.
 *  \return True if the floating-point value is equal to infinity, otherwise false.
 *  \ingroup Utils
 */
template <class T>
inline bool isInfinity(const T& inValue)
{
#ifdef BEAGLE_HAVE_NUMERIC_LIMITS
  if(std::numeric_limits<T>::has_infinity) {
    return ((inValue == std::numeric_limits<T>::infinity()) ||
            (inValue == -std::numeric_limits<T>::infinity()));
  }
#endif // BEAGLE_HAVE_NUMERIC_LIMITS
  T lZero(0.0);
  T lInfinity(1.0 / lZero);
  return ((inValue == lInfinity) || (inValue == -lInfinity));
}


/*!
 *  \brief Test if a floating-point number is finite, that is not NaN or infinite.
 *  \param inValue Value to evaluate finiteness.
 *  \return True if the floating-point value is finite, otherwise false.
 *  \ingroup Utils
 */
template <class T>
inline bool isFinite(const T& inValue)
{
  return ((isNaN<T>(inValue) == false) && (isInfinity(inValue) == false));
}


/*!
 *  \brief Evaluate the maximum of two values.
 *  \param inValue1 First value from which we want the maximum.
 *  \param inValue2 Second value from which we want the maximum.
 *  \return Maximum value between inValue1 and inValue2.
 *  \ingroup Utils
 */
template <class T>
inline T maxOf(const T& inValue1, const T& inValue2)
{
  if(isNaN(inValue1)) return inValue2;
  else if(isNaN(inValue2)) return inValue1;
  return ((inValue2 < inValue1) ? inValue1 : inValue2);
}


/*!
 *  \brief Evaluate the minimum of two values.
 *  \param inValue1 First value from which we want the minimum.
 *  \param inValue2 Second value from which we want the minimum.
 *  \return Minimum value between inValue1 and inValue2.
 *  \ingroup Utils
 */
template <class T>
inline T minOf(const T& inValue1, const T& inValue2)
{
  if(isNaN(inValue1)) return inValue2;
  else if(isNaN(inValue2)) return inValue1;
  return ((inValue1 < inValue2) ? inValue1 : inValue2);
}


/*!
 *  \brief Evaluate square of a number (X^2).
 *  \param inValue Value to evaluate the square.
 *  \return Square of the value.
 *  \ingroup Utils
 */
template <class T>
inline T pow2Of(const T& inValue)
{
  return (inValue * inValue);
}


/*!
 *  \brief Evaluate absolute value of a number.
 *  \param inValue Value to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Utils
 */
template <class T>
inline T absolute(const T& inValue)
{
  return (inValue<0) ? -inValue : inValue;
}


/*!
 *  \brief Evaluate absolute value of a double.
 *  \param inValue Double to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Utils
 */
template <>
inline double absolute(const double& inValue)
{
  return std::fabs(inValue);
}


/*!
 *  \brief Evaluate absolute value of a float.
 *  \param inValue Float to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Utils
 */
template <>
inline float absolute(const float& inValue)
{
  return std::fabs(inValue);
}


/*!
 *  \brief Evaluate absolute value of a long integer.
 *  \param inValue Long integer to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Utils
 */
template <>
inline long absolute(const long& inValue)
{
  return std::labs(inValue);
}


/*!
 *  \brief Evaluate absolute value of a int.
 *  \param inValue Integer to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Utils
 */
template <>
inline int absolute(const int& inValue)
{
  return std::abs(inValue);
}


/*!
 *  \brief Round double to the nearest integer, rounding half-way cases away from 0.
 *  \param inValue Value to round
 *  \return Rounded values to nearest integer.
 *  \ingroup Utils
 */
inline double round(double inValue)
{
  return (inValue<0.0) ? std::ceil(inValue-0.5) : std::floor(inValue+0.5);
}


/*!
 *  \brief  Convert an integer into a string.
 *  \param  inInteger Integer to convert into a string.
 *  \return string containing conversion of the integer.
 *  \ingroup Utils
 */
inline string int2str(long inInteger)
{
  std::ostringstream lStringOS;
  lStringOS << inInteger;
  return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a string into a integer.
 *  \param  inString String to convert into an integer.
 *  \return Long containing the conversion of the string.
 *  \ingroup Utils
 */
inline long str2int(const string& inString)
{
  std::string lStr(inString.c_str());
  std::istringstream lStringIS(lStr);
  long lInteger;
  lStringIS >> lInteger;
  return lInteger;
}


/*!
 *  \brief  Convert an unsigned integer into a string.
 *  \param  inInteger Integer to convert into a string.
 *  \return string containing conversion of the integer.
 *  \ingroup Utils
 */
inline string uint2str(unsigned long inInteger)
{
  std::ostringstream lStringOS;
  lStringOS << inInteger;
  return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a string into an unsigned integer.
 *  \param  inString String to convert into an integer.
 *  \return Unsigned long containing the conversion of the string.
 *  \ingroup Utils
 */
inline unsigned long str2uint(const string& inString)
{
  std::istringstream lStringIS(inString.c_str());
  unsigned long lInteger;
  lStringIS >> lInteger;
  return lInteger;
}


/*!
 *  \brief  Convert a double into a string.
 *  \param  inDouble Double to convert into a string.
 *  \return string containing conversion of the double.
 *  \ingroup Utils
 */
inline string dbl2str(double inDouble)
{
  std::ostringstream lStringOS;
  if(isNaN(inDouble)) lStringOS << "nan";
  else if(isInfinity(inDouble)) {
    if(inDouble < 0.0) lStringOS << "-inf";
    else lStringOS << "inf";
  }
  else lStringOS << inDouble;
  return lStringOS.str().c_str();
}


/*!
 *  \brief  Convert a double into a string.
 *  \param  inDouble Double to convert into a string.
 *  \param  inWidth Width of the written double.
 *  \return string containing conversion of the double.
 *  \ingroup Utils
 */
inline string dbl2str(double inDouble, unsigned int inWidth)
{
  using namespace std;
  ostringstream lStringOS;
  if(inWidth > 1) lStringOS.precision(inWidth-1);
  lStringOS.setf(ostringstream::left);
  lStringOS.width(inWidth);
  if(isNaN(inDouble)) lStringOS << "nan" << std::flush;
  else if(isInfinity(inDouble)) {
    if(inDouble < 0.0) lStringOS << "-inf" << std::flush;
    else lStringOS << "inf" << std::flush;
  }
  else lStringOS << inDouble << std::flush;
  string lString = lStringOS.str().c_str();
  if(lString.size() > inWidth) {
    string::size_type lPosE = lString.find('e', 0);
    if(lPosE < lString.size()) {
      string lStringE(lString, lPosE, lString.size());
      if(lStringE.size() < inWidth)
        lString.replace((inWidth-lStringE.size()), inWidth, lStringE);
    }
  }
  if(lString.size() != inWidth) lString.resize(inWidth, ' ');
  return lString;
}


/*!
 *  \brief  Convert a string into a double.
 *  \param  inString String to convert into a double.
 *  \return Double containing the conversion of the string.
 *  \ingroup Utils
 */
inline double str2dbl(const string& inString)
{
  if(inString == "nan") {
    double lZero(0.0);
    return (lZero / lZero);
  }
  else if(inString == "inf") {
#ifdef BEAGLE_HAVE_NUMERIC_LIMITS
    if(std::numeric_limits<double>::has_infinity) {
      return std::numeric_limits<double>::infinity();
    }
#endif // BEAGLE_HAVE_NUMERIC_LIMITS
    double lZero(0.0);
    return (1.0 / lZero);
  }
  else if(inString == "-inf") {
#ifdef BEAGLE_HAVE_NUMERIC_LIMITS
    if(std::numeric_limits<double>::has_infinity) {
      return -std::numeric_limits<double>::infinity();
    }
#endif // BEAGLE_HAVE_NUMERIC_LIMITS
    double lZero(0.0);
    return (-1.0 / lZero);
  }

  std::istringstream lStringIS(inString.c_str());
  double lDouble;
  lStringIS >> lDouble;
  return lDouble;
}


/*!
 *  \brief Convert a integer to its ordinal form , i.e. 1 -> 1st, 22 -> 22nd.
 *  \param inNumber Integer to convert.
 *  \return Ordinal form of the number.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  \ingroup Utils
 */
inline string uint2ordinal(unsigned int inNumber)
{
  // Write number to string
  std::ostringstream outStream;
  outStream << inNumber;
  std::string lString = outStream.str();

  // Insert commas for easier reading
  if(inNumber>9999) {
    unsigned int lCount=0;
    for(unsigned int i=lString.length(); i>1; --i) {
      ++lCount;
      if(lCount==3) {
        lString.insert(i-1,",");
        lCount=0;
      }
    }
  }

  // Choose appropriate suffix
  std::string lSuffix = "th";
  switch(inNumber % 10) {
    case 1: {lSuffix = "st"; break;}
    case 2: {lSuffix = "nd"; break;}
    case 3: {lSuffix = "rd"; break;}
  }
  switch(inNumber % 100) {
    case 11:
    case 12:
    case 13: {lSuffix = "th"; break;}
  }
  lString.append(lSuffix);

  return lString;
}


/*!
 *  \brief Wrap a string to fit a given line witdh.
 *  \param ioString String to wrap.
 *  \param inLineWidth Line width of the converted string.
 *  \ingroup Utils
 */
inline void wrapString(string& ioString, unsigned int inLineWidth=80)
{
  // Can't do a line width of zero.  (Setting to zero equates to turning off this algorithm.)
  if(inLineWidth==0) return;

  bool lBreakPointValid = false;
  bool lNewLine = true;
  unsigned int lBestBreakPoint = 0;
  unsigned int lFixed = 0;

  // Loop through all the characters in the string
  for (unsigned int i=0; i<ioString.size(); ++i) {

    // Find next character if this is a new line
    if(lNewLine) {
      // Find the next non-whitespace character to start the line.
      string::size_type lNextChar = ioString.find_first_not_of(" \t", i);
      if(lNextChar == string::npos) {
        // The rest of the string is just whitespace. Dump it.
        ioString.erase(i-1);
        return;
      }
      // Swallow one newline.
      if(ioString[lNextChar]=='\n') {
        ++lNextChar;
      }
      // Remove the whitespace.
      if(lNextChar > i) {
        ioString.erase(i,lNextChar-i);
      }
      lNewLine=false;
    }

    // Check if i has gone over line width (this shouldn't happen).
    if(i > lFixed+inLineWidth) return;

    // Check if i is at line width
    if(i == lFixed+inLineWidth) {
      if(!lBreakPointValid) {
        // Breaking in the middle of a word.
        ioString.insert(i,"\n");
        lFixed = i+1;
        i = lFixed-1; // because at the continue i will be incremented
        lBreakPointValid = false;
        lNewLine = true;
        continue;
      }
      else {
        // Breakpoint valid
        if(ioString[lBestBreakPoint]==' ' || ioString[lBestBreakPoint]=='\t') {
          ioString[lBestBreakPoint] = '\n';
          lFixed = lBestBreakPoint+1;
          i = lFixed-1;
          lBreakPointValid = false;
          lNewLine = true;
          continue;
        }
      }
    }

    // Check if current char is a newline
    const char lCurrentChar = ioString[i];
    if(lCurrentChar == '\n') {
      lFixed = i+1;
      lBreakPointValid = false;
      lNewLine=true;
      continue;
    }

    // Look for whitespace to set breakpoints
    if(lCurrentChar==' ' || lCurrentChar=='\t') {
      lBestBreakPoint = i;
      lBreakPointValid = true;
    }
  }
}

}


#endif // Beagle_macros_hpp
