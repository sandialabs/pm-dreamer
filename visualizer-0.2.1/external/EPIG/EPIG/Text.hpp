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
 * \file  EPIG/Text.hpp
 */

#ifndef EPIG_Text_hpp_
#define EPIG_Text_hpp_

#include "EPIG/Types.hpp"
#include "EPIG/Styles.hpp"
#include "EPIG/Graphics.hpp"

namespace EPIG {

    /*!
     * \brief  Graphic primitive for text.
     *
     * A text object is used to display text in a graphic. Each character of the
     * enclosed text is represented by a glyph on with standard stroke and fill
     * attributes apply. For example you can put a stroke on the text to make
     * an outline, and you can change the color of the character using
     * Fill::Color.
     *
     * There is also some special attributes that can be applyed to text to
     * control the appearence of the glyphs, like the font family, size, and
     * style. See Font for details.
     *
     * The text is placed on screen around an anchor point. How text is aligned
     * on the anchor depends of the TextAnchor and TextBaseline attributes.
     */
    class Text : public Graphic {
    public:
        /*!
         * \brief  Constructor specifying the text string and the anchor point.
         * \param  inString  A text string to display.
         * \param  inAnchor  The anchor point to display the text at.
         * \param  inStyle   Optional style attributes to give to the text.
         */
        Text(const XMLTree::String &inString, const Point &inAnchor, 
             const Style inStyle = Style());

        /*!
         * \brief   Get the anchor point for this text.
         * \return  A point with the position of the anchor.
         */
        Point getAnchor() const;
        /*!
         * \brief  Set the anchor point for this text.
         * \param  inAnchor  A new anchor point.
         */
        void setAnchor(const Point &inAnchor);
        /*!
         * \brief  Set the anchor point for this text.
         * \param  inX  An x coordinate of a new anchor point.
         * \param  inY  An y coordinate of a new anchor point.
         */
        void setAnchor(float inX, float inY);

        /*!
         * \brief   Get the text content of this element.
         * \return  A string with the text for this element.
         */
        XMLTree::String getText() const;
        /*!
         * \brief  Set the text content of this element.
         * \param  inString  A new string to replace the old one.
         */
        void setText(const XMLTree::String &inString);

        static const XMLTree::String cElementName; //!< Element's tag name
    };

}

#endif
