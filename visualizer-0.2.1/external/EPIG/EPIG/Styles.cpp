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
 * \file  EPIG/Styles.cpp
 */

#include "EPIG/Styles.hpp"

using namespace EPIG;
using namespace EPIG::XMLTree;


// Style

Style::Style() { }

Style::Style(const Attribute &inAttribute) : AttributeList(inAttribute) { }

Style Style::operator + (const StyleAttribute &inAttribute) const {
    return Style(*this) += inAttribute;
}

Style Style::operator + (const Style &inStyle) const {
    return Style(*this) += inStyle;
}

Style &Style::operator += (const StyleAttribute &inAttribute) {
    AttributeList::operator += (inAttribute);
    return *this;
}

Style &Style::operator += (const Style &inStyle) {
    AttributeList::operator += (inStyle);
    return *this;
}


// Style-attribute

Style StyleAttribute::operator + (const StyleAttribute &inAttribute) const {
    return Style(*this) += inAttribute;
}

Style StyleAttribute::operator + (const Style &inStyle) const {
    return Style(*this) += inStyle;
}

StyleAttribute::StyleAttribute(const std::string &inName, const String &inValue) 
: Attribute(inName, inValue) { }


// Opacity style attribute

const std::string Opacity::cAttributeName = "opacity";

Opacity::Opacity(float inOpacity) 
: StyleAttribute(cAttributeName, String(inOpacity)) { }

// Fill style attribute

const std::string Fill::cAttributeName = "fill";

Fill::Fill(const Color &inColor) 
: StyleAttribute(cAttributeName, inColor) { }

const Fill Fill::cNone(Color("none"));


// Fill-Opacity style attribute

const std::string FillOpacity::cAttributeName = "fill-opacity";

FillOpacity::FillOpacity(float inOpacity) 
: StyleAttribute(cAttributeName, String(inOpacity)) { }


// Fill-Rule style attribute

const std::string FillRule::cAttributeName = "fill-rule";

FillRule::FillRule(const std::string &inFillRule) 
: StyleAttribute(cAttributeName, inFillRule) { }

const FillRule FillRule::cNonZero("nonzero");
const FillRule FillRule::cEvenOdd("evenodd");

// Stroke

const std::string Stroke::cAttributeName = "stroke";

Stroke::Stroke(const Color &inColor) 
: StyleAttribute(cAttributeName, inColor) { }

const Stroke Stroke::cNone(Color("none"));


// Stroke-Opacity

const std::string StrokeOpacity::cAttributeName = "stroke-opacity";

StrokeOpacity::StrokeOpacity(float inOpacity) 
: StyleAttribute(cAttributeName, String(inOpacity)) { }


// Stroke-Width

const std::string StrokeWidth::cAttributeName = "stroke-width";

StrokeWidth::StrokeWidth(float inWidth) 
: StyleAttribute(cAttributeName, String(inWidth)) { }


// Stroke-DashArray

const std::string StrokeDashArray::cAttributeName = "stroke-dasharray";

StrokeDashArray::StrokeDashArray(const std::string &inDashArray) 
: StyleAttribute(cAttributeName, inDashArray) { }

StrokeDashArray::StrokeDashArray(const std::vector<float> &inDashArray) 
: StyleAttribute(cAttributeName, String()) {
    String lNumbers;
    for ( unsigned i = 0; i < inDashArray.size(); i++ )
        lNumbers += String(inDashArray[i]) + " ";
    setValue(lNumbers);
}

const StrokeDashArray StrokeDashArray::cContinuous("none");
const StrokeDashArray StrokeDashArray::cDotted("1");
const StrokeDashArray StrokeDashArray::cDashed("3");


// Stroke-LineCap

const std::string StrokeLineCap::cAttributeName = "stroke-linecap";

StrokeLineCap::StrokeLineCap(const std::string &inLineCap) 
: StyleAttribute(cAttributeName, inLineCap) { }

const StrokeLineCap StrokeLineCap::cButt("butt");
const StrokeLineCap StrokeLineCap::cRound("round");
const StrokeLineCap StrokeLineCap::cSquare("square");


// Stroke-LineJoin

const std::string StrokeLineJoin::cAttributeName = "stroke-linejoin";

StrokeLineJoin::StrokeLineJoin(const std::string &inLineJoin) 
: StyleAttribute(cAttributeName, inLineJoin) { }

const StrokeLineJoin StrokeLineJoin::cMiter("miter");
const StrokeLineJoin StrokeLineJoin::cRound("round");
const StrokeLineJoin StrokeLineJoin::cBevel("bevel");


// Stroke-MiterLimit

const std::string StrokeMiterLimit::cAttributeName = "stroke-miterlimit";

StrokeMiterLimit::StrokeMiterLimit(float inRatio) 
: StyleAttribute(cAttributeName, String(inRatio)) { }


// Font Family Style Attribute

const std::string FontFamily::cAttributeName = "font-family";

FontFamily::FontFamily(const std::string &inName) 
: StyleAttribute(cAttributeName, inName) { }

const FontFamily FontFamily::cSerif("serif");
const FontFamily FontFamily::cSansSerif("sans-serif");


// Font Size Style Attribute

const std::string FontSize::cAttributeName = "font-size";

FontSize::FontSize(float inSize) 
: StyleAttribute(cAttributeName, String(inSize)) { }


// Font Style Style Attribute

const std::string FontStyle::cAttributeName = "font-style";

FontStyle::FontStyle(const std::string &inName) 
: StyleAttribute(cAttributeName, inName) { }

const FontStyle FontStyle::cNormal("normal");
const FontStyle FontStyle::cItalic("italic");
const FontStyle FontStyle::cOblique("oblique");


// Font Weight Style Attribute

const std::string FontWeight::cAttributeName = "font-weight";

FontWeight::FontWeight(const std::string &inName) 
: StyleAttribute(cAttributeName, inName) { }

const FontWeight FontWeight::cNormal("normal");
const FontWeight FontWeight::cBold("bold");


// Text Anchor Style Attribute

const std::string TextAnchor::cAttributeName = "text-anchor";

TextAnchor::TextAnchor(const std::string &inAnchor) 
: StyleAttribute(cAttributeName, inAnchor) { }

const TextAnchor TextAnchor::cStart("start");
const TextAnchor TextAnchor::cMiddle("middle");
const TextAnchor TextAnchor::cEnd("end");


// Text Decoration Style Attribute

const std::string TextDecoration::cAttributeName = "text-decoration";

TextDecoration::TextDecoration(const std::string &inAnchor) 
: StyleAttribute(cAttributeName, inAnchor) { }

const TextDecoration TextDecoration::cNone("none");
const TextDecoration TextDecoration::cUnderline("underline");
const TextDecoration TextDecoration::cLineThrough("line-through");
