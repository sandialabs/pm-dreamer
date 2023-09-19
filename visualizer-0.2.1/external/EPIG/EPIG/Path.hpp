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
 * \file  EPIG/Path.hpp
 */

#ifndef EPIG_Path_hpp_
#define EPIG_Path_hpp_

#include "EPIG/Types.hpp"
#include "EPIG/Styles.hpp"
#include "EPIG/Graphics.hpp"

namespace EPIG {

    class PathData;

    /*!
     * \brief  Graphic primitive for paths.
     * \ingroup  graphics
     *
     * \see  pathcommands
     */
    class Path : public Graphic {
    public:
        /*!
         * \brief  Default constructor.
         * \param  inStyle  Optional style attributes.
         * 
         * Produce an empty path. Commands can be added later.
         */
        explicit Path(const Style &inStyle = Style());
        /*!
         * \brief  Constructor with defined path commands.
         * \param  inCommands  Data defining the shape.
         * \param  inStyle     Optional style attributes.
         *
         * This constructor gives a already defined path. You can
         * always add more commands witht the += operator.
         */
        Path(const PathData &inCommands, const Style &inStyle = Style());

        //@{
        /*!
         * \brief   Add one or more commands to the path.
         * \param   inCommands  One or more commands to add to the path.
         * \return  The resulting path.
         */
        Path operator + (const PathData &inCommands) const;
        Path &operator += (const PathData &inCommands);
        //@}

    private:
        static const XMLTree::String cElementName; //!< Element's tag name
        static const XMLTree::String cDataAttributeName; //!< Path definition data
    };

    /*!
     * \brief  Data defining a path.
     *
     * This is the base class for path commands. It is not possible to create
     * an instance of this class directly. Instead you should use one of the
     * subclasses that provide real functionalities.
     */
    class PathData : protected XMLTree::String {
    public:
        /*!
         * \brief   Concatenation operator.
         * \param   inData  Another component to concatenate to this one.
         * \return  Longer data containing both components.
         */
        PathData operator + (const PathData &inData);
        
    protected:
        friend class Path; //!< For access to XMLTree::String base class
        
        /*!
         * \brief  Default constructor with a string.
         * \param  inValue  A string value for this component.
         */
        PathData(const XMLTree::String &inValue = XMLTree::String());

        static const XMLTree::String cSpaceSeparator; //!< Space separator
        static const XMLTree::String cCommaSeparator; //!< Comma separator 
    };
    
    /*!
     * \defgroup  pathcommands  Path Commands
     *
     * @{
     */

    /*!
     * \brief  Path command for starting a new subpath at given coordinate.
     */
    class MoveTo : public PathData {
    public:
        /*!
         * \brief  Constructor with x and y coordinates.
         * \param  inX  An x coordinate for the subpath starting point.
         * \param  inY  An y coordinate for the subpath starting point.
         */
        MoveTo(float inX, float inY);
        /*!
         * \brief  Constructor with point coordinates.
         * \param  inPoint  Coordinates for the subpath starting point.
         */
        MoveTo(const Point &inPoint);

    private:
        static const XMLTree::String cCommandName; //!< Name of this path command.
    };

    /*!
     * \brief  Path command for drawing a line.
     */
    class LineTo : public PathData {
    public:
        /*!
         * \brief  Constructor with x and y coordinates.
         * \param  inX  An x coordinate for the line destination.
         * \param  inY  An y coordinate for the line destination.
         */
        LineTo(float inX, float inY);
        /*!
         * \brief  Constructor with point coordinates.
         * \param  inPoint  Coordinates for the line destination.
         */
        LineTo(const Point &inPoint);

    private:
        static const XMLTree::String cCommandName; //!< Name of this path command.
    };

    /*!
     * \brief  Path command to close the current subpath.
     *
     * This command close the subpath by drawing a straight line from the 
     * current point to current subpath's initial point.
     */
    class ClosePath : public PathData {
    public:
        /*! 
         * \brief  Default constructor.
         */
        ClosePath();

    private:
        static const XMLTree::String cCommandName; //!< Name of this path command.
    };

    /*!
     * \brief  Path command to draw a cubic Bézier curve.
     */
    class CubicCurveTo : public PathData {
    public:
        /*!
         * \brief  Constructor with two control points.
         * \param  inPoint          A destination point for the curve.
         * \param  inControlPoint1  A control point for the beginning of the 
         *                          curve.
         * \param  inControlPoint2  A control point for the end of the curve.
         */
        CubicCurveTo(const Point &inPoint, const Point &inControlPoint1, 
                     const Point &inControlPoint2);
        /*!
         * \brief  Constructor with one control point.
         * \param  inPoint         A destination point for the curve.
         * \param  inControlPoint  A control point for the end of the curve.
         *
         * The beginning control point is assumed to be the reflection of the 
         * end control point on the previous command relative to the current 
         * point. If previous command is not a CubicCurveTo, assume the end 
         * control point is coincident with the current point.
         */
        CubicCurveTo(const Point &inPoint, const Point &inControlPoint);

    private:
        //! Name of this path command.
        static const XMLTree::String cCommandName;
        //! Name of the spline version of this path command.
        static const XMLTree::String cSplineComponentName;
    };
    
    /*!
     * \brief  Path command to draw a quadratic Bézier curve.
     */
    class QuadraticCurveTo : public PathData {
    public:
        /*!
         * \brief  Constructor with one control point.
         * \param  inPoint         A destination point for the curve.
         * \param  inControlPoint  A control point for the curve.
         */
        QuadraticCurveTo(const Point &inPoint, const Point &inControlPoint);
        /*!
         * \brief  Constructor with no control point.
         * \param  inPoint  A destination point for the curve.
         *
         * The control point is assumed to be the reflection of the control 
         * point on the previous command relative to the current point. If
         * previous command is not a QuadraticCurveTo, assume the control point 
         * is coincident with the current point.
         */
        QuadraticCurveTo(const Point &inPoint);
        /*!
         * \brief  Constructor with no control point.
         * \param  inX  Destination x coordinate.
         * \param  inY  Destination y coordinate.
         *
         * The control point is assumed to be the reflection of the control 
         * point on the previous command relative to the current point. If
         * previous command is not a QuadraticCurveTo, assume the control point 
         * is coincident with the current point.
         */
        QuadraticCurveTo(float inX, float inY);
        
    private:
        //! Name of this path command.
        static const XMLTree::String cCommandName;
        //! Name of the short version of this path command.
        static const XMLTree::String cShorthandComponentName; 
    };
    
    /*!
     * \brief  Path command to draw an elliptical arc.
     *
     * Draws an elliptical arc from the current point to given coordinates. The 
     * size and orientation of the ellipse are defined by x-radius and y-radius
     * and an x-axis-rotation, which indicates how the ellipse as a whole is 
     * rotated relative to the current coordinate system. The center of the 
     * ellipse is calculated automatically to satisfy the constraints imposed 
     * by the other parameters. Large-arc and sweep contribute to the automatic 
     * calculations and help determine how the arc is drawn.
     *
     * For most situations, there are actually four different arcs (two 
     * different ellipses, each with two different arc sweeps) that satisfy 
     * these constraints. large-arc-flag and sweep-flag indicate which one of 
     * the four arcs are drawn, as follows:
     *
     * <ul>
     * <li> Of the four candidate arc sweeps, two will represent an arc sweep of 
     *      greater than or equal to 180 degrees (the "large-arc"), and two will 
     *      represent an arc sweep of less than or equal to 180 degrees (the 
     *      "small-arc"). If large-arc is true, then one of the two larger
     *      arc sweeps will be chosen; otherwise, if large-arc is false, one 
     *      of the smaller arc sweeps will be chosen,
     *
	 * <li> If sweep is true, then the arc will be drawn in a 
     *      "positive-angle" direction (i.e., the ellipse formula 
     *      x=cx+rx*cos(theta) and y=cy+ry*sin(theta) is evaluated such that 
     *      theta starts at an angle corresponding to the current point and 
     *      increases positively until the arc reaches (x,y)). A value of false 
     *      causes the arc to be drawn in a "negative-angle" direction (i.e., 
     *      theta starts at an angle value corresponding to the current point 
     *      and decreases until the arc reaches (x,y)).
     * </ul>
     */
    class EllipticalArcTo : public PathData {
    public:
        /*!
         * \brief  Constructor.
         * \param  inPoint          A destination point for the curve.
         * \param  inXRadius        An x radius of the ellipse.
         * \param  inYRadius        An y radius of the ellipse.
         * \param  inXAxisRotation  A angle of rotation for the ellipse.
         * \param  inLargeArc       True for the path to follow the large arc, 
         *                          false follow the smaller one.
         * \param  inSweep          True to turn by a positive angle, false
         *                          for a negative one.
         */
        EllipticalArcTo(const Point &inPoint, float inXRadius, float inYRadius, 
                        float inXAxisRotation, bool inLargeArc, bool inSweep);

    private:
        static const XMLTree::String cCommandName; //!< Name of this path command.
        static const XMLTree::String cLogicalTrue; //!< Logical true value.
        static const XMLTree::String cLogicalFalse; //!< Logical false value.
    };

    /*!
     * \brief  Short name for a cubic Bézier curve path command.
     */
    typedef CubicCurveTo CurveTo;
    
    /*!
     * @}
     */
}

#endif
