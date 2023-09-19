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
 * \file  EPIG/XMLTree/Attribute.cpp
 */

#include "EPIG/XMLTree/Attribute.hpp"

using namespace EPIG::XMLTree;


// Attribute

AttributeList Attribute::operator + (const Attribute &inAttribute) const {
    return AttributeList(*this) += inAttribute;
}
AttributeList Attribute::operator + (const AttributeList &inAttributeList) const {
    return AttributeList(*this) += inAttributeList;
}


// Attribute List

AttributeList::AttributeList() { }
AttributeList::AttributeList(const Attribute &inAttribute) {
    *this += inAttribute;
}

AttributeList AttributeList::operator + (const Attribute &inAttribute) const {
    return AttributeList(*this) += inAttribute;
}
AttributeList AttributeList::operator + (const AttributeList &inAttributeList) const {
    return AttributeList(*this) += inAttributeList;
}
AttributeList &AttributeList::operator += (const AttributeList &inAttributeList) {
    insert(inAttributeList.begin(), inAttributeList.end());
    return *this;
}
AttributeList &AttributeList::operator += (const Attribute &inAttribute) {
    insert(inAttribute);
    return *this;
}

String AttributeList::get(const std::string &inName) const {
    const_iterator i = find(inName);
    if ( i == end() ) return String();
    else return i->second;
}

bool AttributeList::defined(const std::string &inName) const {
    return find(inName) != end();
}

void AttributeList::remove(const std::string &inName) {
    erase(inName);
}
