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
 * \file  EPIG/Transforms.hpp
 */

#ifndef EPIG_Transforms_hpp_
#define EPIG_Transforms_hpp_

#include "EPIG/XMLTree/XMLTree.hpp"
#include "EPIG/Types.hpp"

namespace EPIG {

    /*!
     * \brief  Coordinate tranform to apply to a graphic.
     * \ingroup  datatype
     */
    class Transform : public XMLTree::String {
    public:
        //@{
        /*!
         * \brief   Concatenation operator.
         * \param   inTransform  A transform to put after this one.
         * \return  The combined transofmation.
         */
        Transform operator + (const Transform &inTransform) const;
        Transform &operator += (const Transform &inTransform);
        //@}

    protected:
        /*!
         * \brief  Construct a transform from name and value.
         * \param  inName   The SVG name of the transformation.
         * \param  inValue  The value string for parameters of this transform.
         */
        Transform(const XMLTree::String &inName, const XMLTree::String &inValue);
        
        //! Separator for between arguments.
        static const XMLTree::String cArgumentSeparator;
        
    private:
        static XMLTree::String makeTransformString(const XMLTree::String &inName, 
                                               const XMLTree::String &inValue);
    };
    
    /*!
     * \defgroup  transformcomp  Transform Components
     *
     * @{
     */

    /*!
     * \brief  Rotation coordinate tranform.
     */
    class Rotate : public Transform {
    public:
        /*!
         * \brief  Construct a rotation with an angle from origin.
         * \param  inAngle  An angle in radians.
         */
        Rotate(float inAngle);
        /*!
         * \brief  Construct a rotation with an angle from specified point.
         * \param  inAngle  An angle in radians.
         * \param  inX      Rotation point x coordinate.
         * \param  inY      Rotation point y coordinate.
         */
        Rotate(float inAngle, float inX, float inY);
        /*!
         * \brief  Construct a rotation with an angle from specified point.
         * \param  inAngle  An angle in radians.
         * \param  inPoint  Rotation point.
         */
        Rotate(float inAngle, const Point &inPoint);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };

    /*!
     * \brief  Translation coordinate tranform.
     */
    class Translate : public Transform {
    public:
        /*!
         * \brief  Constructor with x and y translation.
         * \param  inX  A distance on the x axis.
         * \param  inY  A distance on the y axis.
         */
        Translate(float inX, float inY);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };

    /*!
     * \brief  Scaling coordinate tranform.
     */
    class Scale : public Transform {
    public:
        /*!
         * \brief  Constructor with different scale for x and y axis.
         * \param  inX  A scaling factor on the x axis.
         * \param  inY  A scaling factor on the y axis.
         */
        Scale(float inX, float inY);
        /*!
         * \brief  Constructor with proportional scaling.
         * \param  inScale A scaling factor for both x and y axis.
         */
        Scale(float inScale);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };

    /*!
     * \brief  Skewing on x axis coordinate tranform.
     */
    class SkewX : public Transform {
    public:
        /*!
         * \brief  Constructor with a skew angle.
         * \param  inAngle  An angle to skew the x axis.
         */
        SkewX(float inAngle);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };

    /*!
     * \brief  Skewing on y axis coordinate tranform.
     */
    class SkewY : public Transform {
    public:
        /*!
         * \brief  Constructor with a skew angle.
         * \param  inAngle  An angle to skew the y axis.
         */
        SkewY(float inAngle);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };

    /*!
     * \brief  Applies a matrix transformation.
     */
    class MatrixTransform : public Transform {
    public:
        /*!
         * \brief  Construct a matrix transform from the six upper values of 
         *         the matrix.
         * \param  inA  .
         * \param  inB  .
         * \param  inC  .
         * \param  inD  .
         * \param  inE  .
         * \param  inF  .
         *
         * Transformation matrix is of the form:
         *
         * <code>[a b c]<br>
         *       [d e f]<br>
         *       [0 0 1]</code>
         */
        MatrixTransform(float inA, float inB, float inC, 
                        float inD, float inE, float inF);
    private:
        static const XMLTree::String cTransformName; //!< Transformation name
    };
    
    /*!
     * @}
     */

}

#endif
