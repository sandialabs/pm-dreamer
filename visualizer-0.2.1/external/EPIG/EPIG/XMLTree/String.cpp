/*
 * EPIG - The Easy Programming Interface for Graphics
 * Copyright (C) 2003  Michel Fortin, Jean-Phillipe Lebel, Marie-Eve Tremblay
 * http://www.gel.ulaval.ca/~epig/
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*!
 * \file  EPIG/XMLTree/String.cpp
 */

#include <sstream>
#include "EPIG/XMLTree/String.hpp"

using namespace EPIG::XMLTree;

// Useful template functions for convertions
/*!
 * \internal
 * \brief   Template for creating a string from a data type.
 * \param   inData  Data to convert.
 * \return  Data expressed as a standard library string.
 */
template <typename dataType>
std::string getStringValue(dataType inData) {
    std::ostringstream lStream;
    lStream << inData;
    return lStream.str();
}
/*!
 * \internal
 * \brief   Template for creating a data type from a string.
 * \param   inString  String to convert.
 * \return  Data expressed in native format.
 */
template <typename dataType>
dataType getValue(const std::string &inString) {
    if ( inString.length() == 0 )
        throw DataFormatException("String::getTypeValue(): String is empty.");
    dataType lNumber;
    std::istringstream lStream(inString);
    if ( lStream >> lNumber ) return lNumber;
    else throw DataFormatException("String::getTypeValue(): "
                                   "String is not a numeric value.");
}


// String XML Node

const NodeType String::cNodeType = eText;

String::String() : Node(cEmpty) { }

String::String(char inChar)
: Node(new DataString(getStringValue(inChar))) { }

String::String(float inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(double inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(signed int inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(signed short inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(signed long inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(unsigned int inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(unsigned short inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(unsigned long inNumber)
: Node(new DataString(getStringValue(inNumber))) { }

String::String(const char inString[]) 
: Node(new DataString(inString)) { }

String::String(const std::string &inString) 
: Node(new DataString(inString)) { }

String::String(const String &inString1, const String &inString2) 
: Node(new DataPair(inString1, inString2)) {
    // Save memory and get better efficiency by consolidating strings that
    // are fragmented too much.
    if ( length()/fragmentation() < cMinCharPerFragment )
        consolidate(); 
}

String String::operator + (const String &inString) const {
    return String(*this, inString);
}
String &String::operator += (const String &inString) {
    *this = String(*this, inString);
    return *this;
}

bool String::operator == (const String &inString) const {
    if ( useSameDataAs(inString) ) return true;
    if ( length() != inString.length() ) return false;

    // Consolidate both strings to optimize comparaison. This is most often
    // useful because comparaisons happen usually more than once.
    consolidate();
    inString.consolidate();
    
    // See if strings are equals.
    if ( getData().getStdStringValue() 
            == inString.getData().getStdStringValue() ) {
        // If equals: free some memory by making both strings use the same data. 
        mergeWith(inString);
        return true;
    } else
        return false;
}
bool String::operator != (const String &inString) const {
    return !( *this == inString );
}
bool String::operator < (const String &inString) const {
    consolidate();
    inString.consolidate();
    return ( getData().getStdStringValue() 
                < inString.getData().getStdStringValue() );
}

unsigned String::length() const {
    return getData().length();
}

String::operator std::string () const {
    return getData().getStdStringValue();
}

signed int String::getIntValue() const { 
    return getValue<signed int>(*this);
}
signed short String::getShortValue() const {
    return getValue<signed short>(*this);
}
signed long String::getLongValue() const {
    return getValue<signed long>(*this);
}
unsigned int String::getUnsignedIntValue() const {
    return getValue<unsigned int>(*this);
}
unsigned short String::getUnsignedShortValue() const {
    return getValue<unsigned short>(*this);
}
unsigned long String::getUnsignedLongValue() const {
    return getValue<unsigned long>(*this);
}
float String::getFloatValue() const {
    return getValue<float>(*this);
}
double String::getDoubleValue() const {
    return getValue<double>(*this);
}

const String String::cEmpty = "";
const unsigned String::cMinCharPerFragment 
                                    = sizeof(DataPair) + sizeof(DataString) + 1;

void String::consolidate() const {
    if ( getData().isFragmented() )
        // Value won't change, only underlying structure, so it's okay to bypass
        // const strictness.
        const_cast<String &>(*this) = String(getData().getStdStringValue());
}

void String::mergeWith(const String &inString) const {
    // Always prefer freeing at higher address space: that 
    // should help reduce memory fragmentation.
    
    if ( &getData() > &inString.getData() )
        // Value won't change, only underlying structure, so it's okay to bypass
        // const strictness.
        const_cast<String &>(*this) = inString;
    else
        const_cast<String &>(inString) = *this;
}

unsigned String::fragmentation() const {
    return getData().fragmentation();
}

// String::DataString

String::DataString::DataString() { }

String::DataString::DataString(const char inString[]) 
: mString(inString) { }

String::DataString::DataString(const std::string &inString) 
: mString(inString) { }

unsigned String::DataString::length() const {
    return (unsigned)mString.length();
}

void String::DataString::writeText(std::ostream &inStream) const {
    inStream << mString;
}

std::string String::DataString::getStdStringValue() const {
    return mString;
}


//todo:  Improve character escaping performance. Buffering?
void String::DataString::write(std::ostream &inStream) const {
    // Escape required characters.
    for ( unsigned i = 0; i < mString.length(); i++ )
        switch ( char c = mString[i] ) {
            case '<': inStream << "&lt;";
            case '>': inStream << "&gt;";
            case '&': inStream << "&amp;";
            case '"': inStream << "&quot;";
            default: inStream << c;
        }
}

String::DataString *String::DataString::clone() const {
    return new DataString(*this);
}


// DataPair

String::DataPair::DataPair(const String &inString1, const String &inString2) 
: mLength(inString1.length() + inString2.length()), 
  mString1(inString1), 
  mString2(inString2) { }

bool String::DataPair::isFragmented() const {
    return true;
}

unsigned String::DataPair::fragmentation() const {
    return 1 + mString1.fragmentation() + mString2.fragmentation();
}

unsigned String::DataPair::length() const {
    return mLength;
}

void String::DataPair::writeText(std::ostream &inStream) const {
    mString1.getData().writeText(inStream);
    mString2.getData().writeText(inStream);
}

std::string String::DataPair::getStdStringValue() const {
    std::ostringstream lStream;
    writeText(lStream);
    return lStream.str();
}

void String::DataPair::write(std::ostream &inStream) const {
    mString1.write(inStream);
    mString2.write(inStream);
}

String::DataPair *String::DataPair::clone() const {
    return new DataPair(*this);
}

