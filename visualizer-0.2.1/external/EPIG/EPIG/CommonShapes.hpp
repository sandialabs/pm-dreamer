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
 * \file  EPIG/CommonShapes.hpp
 */

#ifndef EPIG_CommonShapes_hpp_
#define EPIG_CommonShapes_hpp_

#include "EPIG/Types.hpp"
#include "EPIG/Styles.hpp"
#include "EPIG/Graphics.hpp"

namespace EPIG {

    /*!
     * \brief  Graphic primitive for rectangles.
     * \ingroup  graphics
     *
     * A rectangle is a four-sided figure with two vertical sides and two
     * horizontal ones.
     */
    class Rectangle : public Graphic {
    public:
        /*!
         * \brief  Constructor with point of origin and rectangular size.
         * \param  inOrigin  A point to be the origin corner.
         * \param  inSize    Width and height of the rectangle.
         * \param  inStyle   Optional style attributes to give to the rectangle.
         */
        Rectangle(const Point &inOrigin, const Size &inSize, 
                  const Style &inStyle = Style());

        /*!
         * \brief   Get the origin coordinates of this rectangle.
         * \return  A point with origin coordinates.
         */
        Point getOrigin() const;
        /*!
         * \brief  Set the origin coordinates of this rectangle.
         * \param  inOrigin  A new origin point.
         */
        void setOrigin(const Point &inOrigin);
        /*!
         * \brief  Set the origin coordinates of this rectangle.
         * \param  inX  A new origin point x coordinate.
         * \param  inY  A new origin point y coordinate.
         */
        void setOrigin(float inX, float inY);
        
        /*!
         * \brief   Get the size of the rectangle.
         * \return  A size value corresponding to the rectangle size.
         */
        Size getSize() const;
        /*!
         * \brief  Set the size of the rectangle.
         * \param  inSize  A new size value for the rectangle.
         */
        void setSize(const Size &inSize);
        /*!
         * \brief  Set the size of the rectangle.
         * \param  inWidth   A new width for the rectangle.
         * \param  inHeight  A new height for the rectangle.
         */
        void setSize(float inWidth, float inHeight);
        
        static const XMLTree::String cElementName; //!< Element's tag name.
    };

    /*!
     * \brief  Graphic primitive for circles.
     * \ingroup  graphics
     *
     * A circle is defined by a central point (called the center) and a radius.
     * The shape is formed of the closed curve where the distance between the
     * center and each point of the curve is equal to the radius.
      */
    class Circle : public Graphic {
    public:
        /*!
         * \brief  Constructor.
         * \param  inCenter  A central point.
         * \param  inRadius  A number representing the radius.
         * \param  inStyle   Optional style attributes to give to the circle.
         */
        Circle(const Point &inCenter, float inRadius, 
               const Style &inStyle = Style());

        /*!
         * \brief   Get the center coordinates of this rectangle.
         * \return  A point with the center coordinates.
         */
        Point getCenter() const;
        /*!
         * \brief  Set the center coordinates of this rectangle.
         * \param  inCenter  A new center point.
         */
        void setCenter(const Point &inCenter);
        /*!
         * \brief  Set the center coordinates of this rectangle.
         * \param  inCX  A new origin point x coordinate.
         * \param  inCY  A new origin point y coordinate.
         */
        void setCenter(float inCX, float inCY);

        /*!
         * \brief   Get the circle's radius.
         * \return  The circle radius.
         */
        float getRadius() const;
        /*!
         * \brief  Set the circle's radius.
         * \param  inRadius  A new circle radius.
         */
        void setRadius(float inRadius);
        
        static const XMLTree::String cElementName; //!< Element's tag name.
    private:
        static const XMLTree::String cCenterXAttributeName; //!< Center X coordinate.
        static const XMLTree::String cCenterYAttributeName; //!< Center Y coordinate.
        static const XMLTree::String cRadiusAttributeName; //!< Radius attribute.
    };
    
    /*!
     * \brief  Graphic primitive for ellipses.
     * \ingroup  graphics
     *
     * The ellipse is defined by a central point (the center) and two radius
     * values: one for the x axis, the other for the y axis.
     */
    class Ellipse : public Graphic {
    public:
        /*!
         * \brief  Constructor.
         * \param  inCenter   A central point.
         * \param  inXRadius  A number representing the radius on the x axis.
         * \param  inYRadius  A number representing the radius on the y axis.
         * \param  inStyle    Optional style attributes to give to the circle.
         */
        Ellipse(const Point &inCenter, float inXRadius, float inYRadius,
                const Style &inStyle = Style());

        /*!
         * \brief   Get the center coordinates of this rectangle.
         * \return  A point with the center coordinates.
         */
        Point getCenter() const;
        /*!
         * \brief  Set the center coordinates of this rectangle.
         * \param  inCenter  A new center point.
         */
        void setCenter(const Point &inCenter);
        /*!
         * \brief  Set the center coordinates of this rectangle.
         * \param  inCX  A new origin point x coordinate.
         * \param  inCY  A new origin point y coordinate.
         */
        void setCenter(float inCX, float inCY);

        /*!
         * \brief   Get ellipse's x radius.
         * \return  The ellipse x radius.
         */
        float getXRadius() const;
        /*!
         * \brief  Set ellipse's x radius.
         * \param  inXRadius  A new ellipse x radius.
         */
        void setXRadius(float inXRadius);
        
        /*!
         * \brief   Get ellipse's y radius.
         * \return  The ellipse y radius.
         */
        float getYRadius() const;
        /*!
         * \brief  Set ellipse's y radius.
         * \param  inYRadius  A new ellipse y radius.
         */
        void setYRadius(float inYRadius);
        
        static const XMLTree::String cElementName; //!< Element's tag name.
    private:
        static const XMLTree::String cCenterXAttributeName; //!< Center X coordinate.
        static const XMLTree::String cCenterYAttributeName; //!< Center Y coordinate.
        static const XMLTree::String cXRadiusAttributeName; //!< Radius attribute name.
        static const XMLTree::String cYRadiusAttributeName; //!< Radius attribute name.
    };

    /*!
     * \brief  Graphic primitive for straight lines.
     * \ingroup  graphics
     *
     * A line link a start point to an end point. Since a line has no fillable
     * region and by default shapes do not have a stroke, you must explicitly
     * define a stroke if you want the line to be visible.
     *
     * \see Stroke
     */
    class Line : public Graphic {
    public:
        /*!
         * \brief  Constructor.
         * \param  inStart  A start point.
         * \param  inEnd    An end point.
         * \param  inStyle  Optional style attributes to give to the line.
         */
        Line(const Point &inStart, const Point &inEnd, 
             const Style &inStyle = Style());

        /*!
         * \brief   Get the start point coordinates.
         * \return  The start point coordinates.
         */
        Point getStart() const;
        /*!
         * \brief  Set the start point coordinates.
         * \param  inStart  New start point coordinates.
         */
        void setStart(const Point &inStart);
        /*!
         * \brief  Set the start point coordinates.
         * \param  inX  New start point x coordinate.
         * \param  inY  New start point y coordinate.
         */
        void setStart(float inX, float inY);

        /*!
         * \brief   Get the end point coordinates.
         * \return  The end point coordinates.
         */
        Point getEnd() const;
        /*!
         * \brief  Set the end point coordinates.
         * \param  inEnd  New end point coordinates.
         */
        void setEnd(const Point &inEnd);
        /*!
         * \brief  Set the end point coordinates.
         * \param  inX  New end point x coordinate.
         * \param  inY  New end point y coordinate.
         */
        void setEnd(float inX, float inY);

        static const XMLTree::String cElementName; //!< Element's tag name.
    private:
        static const XMLTree::String cX1AttributeName; //!< Start point x coordinate.
        static const XMLTree::String cY1AttributeName; //!< Start point y coordinate.
        static const XMLTree::String cX2AttributeName; //!< End point x coordinate.
        static const XMLTree::String cY2AttributeName; //!< End point y coordinate.
    };
    
    /*!
     * \brief  Graphic primitive for polygons
     * \ingroup  graphics
     *
     * A polygon is made of a serie of points linked together with strait lines.
     * A polygon is a closed path, so the last point is always linked with the 
     * first one.
     *
     * \see  Polyline
     */
    class Polygon : public Graphic {
    public:
        /*!
         * \brief  Default constructor.
         * \param  inStyle  Optional style attributes.
         * 
         * Construct an empty polygon. This constructor is provided so you can
         * add points later.
         */
        Polygon(const Style &inStyle = Style());
        /*!
         * \brief  Constructor with a list of points.
         * \param  inLinePath  A list of points to form the line path.
         * \param  inStyle     Optional style attributes.
         * 
         * This constructor gives a polygon with predefined points. You can
         * always add more points witht the += operator.
         */
        Polygon(const PointList &inLinePath, const Style &inStyle = Style());
        
        //@{
        /*!
         * \brief   Addition operator.
         * \param   inLinePath  A list of points to append tp the line path.
         * \return  A reference to the resulting polygon.
         *
         * The line path gets one or more new points. The closing line will be
         * between the first point and the last point added.
         */
        Polygon operator + (const PointList &inLinePath) const;
        Polygon &operator += (const PointList &inLinePath);
        //@}
        //@{
        /*!
         * \brief  Addition operator.
         * \param  inPoint  A point to append tp the line path.
         * \return  A reference to the resulting polygon.
         *
         * The line path gets one new point. The closing line will be between 
         * the first point and the new last point.
         */
        Polygon operator + (const Point &inPoint) const;
        Polygon &operator += (const Point &inPoint);
        //@}

        static const XMLTree::String cElementName; //!< Element's tag name.
    private:
        static const XMLTree::String cPointsAttributeName; //!< Point list attribute.
    };

    /*!
     * \brief  Graphic primitive for polyline
     * \ingroup  graphics
     *
     * A polyline is made of a serie of points linked together with strait 
     * lines. Polyline is not closed: so the last point is not linked with the 
     * first one.
     * 
     * \see  Polygon
     */
    class Polyline : public Graphic {
    public:
        /*!
         * \brief  Default constructor.
         * \param  inStyle  Optional style attributes.
         * 
         * Construct an empty polyline. This constructor is provided so you can
         * add points later.
         */
        Polyline(const Style &inStyle = Style());
        /*!
         * \brief  Constructor with a list of points.
         * \param  inLinePath  A list of points to form the line path.
         * \param  inStyle     Optional style attributes.
         * 
         * This constructor gives a polyline with predefined points. You can
         * always add more points witht the += operator.
         */
        Polyline(const PointList &inLinePath, const Style &inStyle = Style());

        //@{
        /*!
         * \brief   Addition operator.
         * \param   inLinePath  A list of points to append tp the line path.
         * \return  A reference to the resulting polygon.
         *
         * The line path gets one or more new points. The closing line will be
         * between the first point and the last point added.
         */
        Polyline operator + (const PointList &inLinePath) const;
        Polyline &operator += (const PointList &inLinePath);
        //@}
        //@{
        /*!
         * \brief  Addition operator.
         * \param  inPoint  A point to append tp the line path.
         * \return  A reference to the resulting polygon.
         *
         * The line path gets one new point. The closing line will be between 
         * the first point and the new last point.
         */
        Polyline operator + (const Point &inPoint) const;
        Polyline &operator += (const Point &inPoint);
        //@}

        static const XMLTree::String cElementName; //!< Element's tag name.
    private:
        static const XMLTree::String cPointsAttributeName; //!< Point list attribute.
    };

}

#endif
