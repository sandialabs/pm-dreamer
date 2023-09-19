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
 * \file  EPIG/Text.cpp
 */

#include "EPIG/Text.hpp"

using namespace EPIG;
using namespace EPIG::XMLTree;


// Text Graphic Attribute

const String Text::cElementName = "text";
Text::Text(const String &inString, const Point &inAnchor, const Style inStyle)
: Graphic(cElementName,
          Attribute(cXAttributeName, String(inAnchor.x)) +
          Attribute(cYAttributeName, String(inAnchor.y)) +
          inStyle) {
    setContent(inString);
}

Point Text::getAnchor() const {
    return Point(getAttribute(cXAttributeName).getFloatValue(),
                 getAttribute(cYAttributeName).getFloatValue());
}
void Text::setAnchor(const Point &inAnchor) {
    setAttribute(cXAttributeName, String(inAnchor.x));
    setAttribute(cYAttributeName, String(inAnchor.y));
}
void Text::setAnchor(float inX, float inY) {
    setAttribute(cXAttributeName, String(inX));
    setAttribute(cYAttributeName, String(inY));
}

String Text::getText() const {
    NodeList lNodeList = getContent();
    // Normalement le contenu est un
    if ( lNodeList.size() == 1 ) {
        return (String &)lNodeList[0];
    } else {
        String lText;
        for ( unsigned i = 0; i < lNodeList.size(); i++ )
            lText += (String &)lNodeList[i];
        return lText;
    }
}
void Text::setText(const String &inString) {
    setContent(inString);
}
