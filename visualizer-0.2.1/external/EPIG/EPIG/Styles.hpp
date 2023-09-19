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
 * \file  EPIG/Styles.hpp
 */

#ifndef EPIG_Styles_hpp_
#define EPIG_Styles_hpp_

#include "EPIG/XMLTree/XMLTree.hpp"
#include "EPIG/Types.hpp"

namespace EPIG {

    class StyleAttribute;

    /*!
     * \brief  List of attributes representing the style of an element.
     *
     * A style object hold a list of style attributes.
     *
     * \see  StyleAttribute
     */
    class Style : public XMLTree::AttributeList {
    public:
        /*!
         * \brief  Default constructor. Make an empty style.
         */
        Style();
        /*!
         * \brief  Constructor with a style attribute.
         */
        Style(const XMLTree::Attribute &inAttribute);

        //@{
        /*!
         * \brief   Addition operator.
         * \param   inAttribute  A style attribute to add to this style.
         * \return  The style with added attributes.
         */
        Style operator + (const StyleAttribute &inAttribute) const;
        Style &operator += (const StyleAttribute &inAttribute);
        //@}
        //@{
        /*!
         * \brief   Addition operator.
         * \param   inStyle  A style attribute list to add to this style.
         * \return  The style with added attributes.
         */
        Style operator + (const Style &inStyle) const;
        Style &operator += (const Style &inStyle);
        //@}

        // Additions with other attributes are still possible
        // so allow the use of AttributeList's operators + and +=.
        using XMLTree::AttributeList::operator +;
        using XMLTree::AttributeList::operator +=;
    };

    /*!
     * \brief   Template style attribute
     *
     * This class is provided to be the basis of any attribute that can be hold
     * in a style. The only constructor in the class is protected: that means
     * that StyleAttribute cannot be used directly. You need to subclass it
     * to represent a specific attribute that you can use.
     */
    class StyleAttribute : public XMLTree::Attribute {
    public:
        /*!
         * \brief   Concatenation operator.
         * \param   inAttribute  An attribute to append to this one.
         * \return  The resulting Style.
         */
        Style operator + (const StyleAttribute &inAttribute) const;
        /*!
         * \brief   Concatenation operator.
         * \param   inStyle  A style to append after this style attribute.
         * \return  The resulting Style.
         */
        Style operator + (const Style &inStyle) const;
        
    protected:
        /*!
         * \brief  Constructor with name and value.
         * \param  inName   A name for the style attribute.
         * \param  inValue  A value for the style attribute.
         */
        StyleAttribute(const std::string &inName, 
                       const XMLTree::String &inValue);
    };
    
    /*!
     * \defgroup  styleattr  Style Attributes
     *
     * @{
     */

    /*!
     * \brief    Style attribute setting object opacity.
     *
     * This will set the opacity of the resulting image of a graphic. 
     *
     * This is different than setting FillOpacity and StrokeOpacity. For
     * example, if you set both StrokeOpacity and FillOpacity to 0.5, you will 
     * see the fill edge behind the stroke. With opacity the hole shape is
     * rendered and then made transparent.
     */
    class Opacity : public StyleAttribute {
    public:
        /*!
         * \brief  Set opacity of the graphic.
         * \param  inOpacity  A floating point value between 0 and 1.
         */
        Opacity(float inOpacity);
    private:
        static const std::string cAttributeName; //!< Attribute name
    };

    /*!
     * \brief  Style attribute specifying how to fill shapes content.
     *
     * Currently this class can only take plain color fills. It may be extended
     * in the future to support gradient or patterns.
     */
    class Fill : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with a fill color.
         * \param  inColor  A color fill the shape with.
         */
        Fill(const Color &inColor);

        /*!
         * \brief  This constant represent an empty (or transparent) fill.
         */
        static const Fill cNone;

    private:
        static const std::string cAttributeName; //!< Attribute name
    };

    /*!
     * \brief  Style attribute specifying the level of opacity of the fill.
     * \see  Fill
     */
    class FillOpacity : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with a floating point opacity value.
         * \param  inOpacity  Floating point opacity between 0 (transparent) and
         *                    1 (completely opaque).
         */
        FillOpacity(float inOpacity);

    private:
        static const std::string cAttributeName; //!< Attribute name
    };

    /*!
     * \brief  Style attribute specifying what rule to use for overlapping
     *         regions in the same shape.
     *
     * Overlapping regions can be filled according to differents rules
     * defined as constants inside this class.
     */
    class FillRule : public StyleAttribute {
    public:
        /*!
         * \brief  After counting the crossings, if the result is zero then the 
         *         point is outside the path. Otherwise, it is inside.
         *
         * This rule determines the "insideness" of a point on the canvas by 
         * drawing a ray from that point to infinity in any direction and then 
         * examining the places where a segment of the shape crosses the ray. 
         * Starting with a count of zero, add one each time a path segment 
         * crosses the ray from left to right and subtract one each time a 
         * path segment crosses the ray from right to left. After counting the 
         * crossings, if the result is zero then the point is outside the path. 
         * Otherwise, it is inside. The following drawing illustrates the 
         * non-zero rule:
         * 
         * \image html fillrule-nonzero.png
         */
        static const FillRule cNonZero;
        /*!
         * \brief  After counting the crossings, if the count is odd, the point
         *         is inside; if even, the point is outside.
         *
         * This rule determines the "insideness" of a point on the canvas by 
         * drawing a ray from that point to infinity in any direction and 
         * counting the number of path segments from the given shape that the 
         * ray crosses. If this number is odd, the point is inside; if even, 
         * the point is outside. The following drawing illustrates the even-odd 
         * rule:
         * 
         * \image html fillrule-evenodd.png
         */
        static const FillRule cEvenOdd;

    private:
        /*!
         * \brief  Constructor with fill rule value as a string.
         * \param  inFillRule  A fill rule value as a string.
         */
        FillRule(const std::string &inFillRule);
        
        static const std::string cAttributeName; //!< Attribute name
    };

    /*!
     * \brief  Style attribute specifying how to fill the stroke of a shape.
     *
     * Currently this class can only take plain color strokes. It may be 
     * extended in the future to support gradient or patterns strokes.
     */
    class Stroke : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with a stroke color.
         * \param  inColor  A color fill the stroke with.
         */
        Stroke(const Color &inColor);

        /*!
         * \brief  This constant represent an non-existant (or invisible) 
         *         stroke.
         */
        static const Stroke cNone;

    private:
        static const std::string cAttributeName;
    };

    /*!
     * \brief  Style attribute specifying opacity of the stroke.
     */
    class StrokeOpacity : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with a floating point opacity value.
         * \param  inOpacity  Floating point opacity between 0 (transparent) and
         *                    1 (completely opaque).
         */
        StrokeOpacity(float inOpacity);
    private:
        static const std::string cAttributeName; //!< Attribute name.
    };

    /*!
     * \brief  Style attribute specifying width of the stroke.
     *
     * Defautl width of as stroke is 1.
     */
    class StrokeWidth : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with a floating point value.
         * \param  inWidth  A positive floating point stroke width value.
         */
        StrokeWidth(float inWidth);
    private:
        static const std::string cAttributeName; //!< Attribute name.
    };
    
    /*!
     * \brief  Style attribute specifying how to dash the stroke.
     *
     * The dash array is a list of numbers repsesenting the lengths of each
     * part of the dashed line. The first number represent the first dash 
     * length, the next is the empty space length, the next is the second dash
     * lenght, and so on.
     *
     * \see  DashArray
     */
    class StrokeDashArray : public StyleAttribute {
    public:
        /*!
         * \brief  Construct a dash array with a string.
         * \param  inDashArray  A string with the lenght of dash and spaces
         *                      separated by spaces characters.
         *
         * Sample dash array value: "10 2.5 5"
         */
        StrokeDashArray(const std::string &inDashArray);
        /*!
         * \brief  Construct a dash array with a vector of floats.
         * \param  inDashArray  A vector of floats as the list of dashes and 
         *                      spaces.
         */
        StrokeDashArray(const std::vector<float> &inDashArray);

        /*!
         * \brief  Default dash array for a continous line.
         */
        static const StrokeDashArray cContinuous;
        /*!
         * \brief  Default dash array for a dotted line.
         * \note   This will work correctly only with stroke width of 1.
         */
        static const StrokeDashArray cDotted;
        /*!
         * \brief  Default dash array for a dashed line.
         */
        static const StrokeDashArray cDashed;
    private:
        static const std::string cAttributeName; //!< Attribute Name
    };

    /*!
     * \brief  Style attribute specifying the type of cap for the stroke ends.
     * \see  LineCap
     */
    class StrokeLineCap : public StyleAttribute {
    public:
        /*!
         * \image html linecap-butt.png
         */
        static const StrokeLineCap cButt;
        /*!
         * \image html linecap-round.png
         */
        static const StrokeLineCap cRound;
        /*!
         * \image html linecap-square.png
         */
        static const StrokeLineCap cSquare;

    private:
        /*!
         * \brief  Constructor with line cap string.
         * \param  inLineCap  A line cap style name.
         */
        StrokeLineCap(const std::string &inLineCap);
        
        static const std::string cAttributeName; //!< Attribute name.
    };

    /*!
     * \brief  Style attribyte specifying the type of join between stroke 
     *         segments.
     * \see  LineJoin
     */
    class StrokeLineJoin : public StyleAttribute {
    public:
        /*!
         * \image html linejoin-miter.png
         */
        static const StrokeLineJoin cMiter;
        /*!
         * \image html linejoin-round.png
         */
        static const StrokeLineJoin cRound;
        /*!
         * \image html linejoin-bevel.png
         */
        static const StrokeLineJoin cBevel;

    private:
        /*!
         * \brief  Constructor with line join string.
         * \param  inLineJoin  A line join style name.
         */
        StrokeLineJoin(const std::string &inLineJoin);
        
        static const std::string cAttributeName; //!< Attribute name.
    };
    
    /*!
     * \brief  Style attribyte specifying the limit on the ratio of the miter 
     *         length to the width of the stroke.
     *
     * This style attribute has no effect on strokes with line join that is not
     * set to miter. The default miter limit ratio is 4.
     *
     * \see  StrokeLineJoin, MiterLimit
     */
    class StrokeMiterLimit : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with limit ratio.
         * \param  inRatio  A maximum ratio of the miter length to the width
         *                  of the stroke.
         */
        StrokeMiterLimit(float inRatio);

    private:
        static const std::string cAttributeName; //!< Attribute name.
    };

    /*!
     * \brief  Style attribute specifying the family (or the name) of the font 
     *         used to draw text.
     */
    class FontFamily : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with family name.
         * \param  inName  The name of the family.
         * 
         * You can also optionally pass a list of families to choose from. In
         * this case the first matching family will be used. The list must be
         * of the form: "Times, Times New Roman, serif".
         */
        FontFamily(const std::string &inName);

        /*!
         * \brief  Default serif font from the viewer.
         */
        static const FontFamily cSerif;
        /*!
         * \brief  Default sans serif font from the viewer.
         */
        static const FontFamily cSansSerif;

    private:
        static const std::string cAttributeName; //!< Attribute Name
    };
    
    /*!
     * \brief  Style attribute specifying the style of text to use.
     */
    class FontStyle : public StyleAttribute {
    public:
        /*!
         * \brief  Use normal font style.
         */
        static const FontStyle cNormal;
        /*!
         * \brief  Use italic font style.
         *
         * Similar to cOblique.
         */
        static const FontStyle cItalic;
        /*!
         * \brief  Use oblique font style.
         *
         * Similar to cItalic.
         */
        static const FontStyle cOblique;

    private:
        /*!
         * \brief  Constructor with font style name.
         * \param  inName  A valid CSS style name string.
         */
        FontStyle(const std::string &inName);
        
        static const std::string cAttributeName; //!< Attribute Name
    };
    
    /*!
     * \brief  Style attribute defining the weight of a font.
     */
    class FontWeight : public StyleAttribute {
    public:
        /*!
         * \brief  Normal font weight.
         */
        static const FontWeight cNormal;
        /*!
         * \brief  Bold font weight.
         */
        static const FontWeight cBold;

    private:
        /*!
         * \brief  Constructor with weight name.
         */
        FontWeight(const std::string &inName);
        
        static const std::string cAttributeName;
    };

    /*!
     * \brief  Style attribute specifying the size of the font used to draw
     *         text.
     */
    class FontSize : public StyleAttribute {
    public:
        /*!
         * \brief  Constructor with floating point font size.
         * \param  inSize  A floating point number with the size in pixel.
         */
        FontSize(float inSize);

    private:
        static const std::string cAttributeName; //!< Attribute Name
    };

    /*!
     * \brief  Attribute specifying what part of the text to place at the anchor
     *         point.
     *
     * This attribute allow you to specify if the text anchor should be at the
     * start, the middle or the end of the text. Static member constants are
     * defined for each allowed value.
     */
    class TextAnchor : public StyleAttribute {
    public:
        /*!
         * \name  Predefined anchor attributes with allowed values
         */
        //@{
        /*!
         * \brief  Start the text at the anchor point.
         */
        static const TextAnchor cStart;
        /*!
         * \brief  Put the middle of the text at the anchor point.
         */
        static const TextAnchor cMiddle;
        /*!
         * \brief  Put the end of the text at the anchor point.
         */
        static const TextAnchor cEnd;
        //@}

    private:
        /*!
         * \brief  Anchor the text using the value given in the string.
         * \param  inAnchor  A valid SVG value for the <code>text-anchor</code>
         *                   attribute.
         */
        TextAnchor(const std::string &inAnchor);
        
        static const std::string cAttributeName; //!< Attribute name
    };

    /*!
     * \brief  Style attribute specifying the decoration to put on the text.
     *
     * Supported decorations are underline and line-through.
     */
    class TextDecoration : public StyleAttribute {
    public:
        /*!
         * \brief  Clears all text decorations.
         */
        static const TextDecoration cNone;
        /*!
         * \brief  Set an underline decoration for the text.
         */
        static const TextDecoration cUnderline;
        /*!
         * \brief  Set a line-through decoration for the text.
         */
        static const TextDecoration cLineThrough;

    private:
        TextDecoration(const std::string &inAnchor);
        
        static const std::string cAttributeName; //!< Attribute name
    };
    
    /*!
     * \name  Style Attributes Short Names
     */
    //@{
    typedef StrokeDashArray DashArray;
    typedef StrokeLineCap LineCap;
    typedef StrokeLineJoin LineJoin;
    typedef StrokeMiterLimit MiterLimit;
    //@}
    
    /*!
     * @}
     */
}

#endif
