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
 * \file   EPIG/Types.hpp
 * \brief  Data types commonly used.
 */

#ifndef EPIG_Types_hpp_
#define EPIG_Types_hpp_

#include "EPIG/XMLTree/String.hpp"
#include <cmath>
#include <vector>

namespace EPIG {

    /*!
     * \defgroup  datatype  Data types
     *
     * @{
     */

    /*!
     * \brief  Basic SVG color string.
     *
     * Colors must be represented by strings when passed in an XML attribute.
     * For this reason, Color base class is XMLTree::String.
     */
    class Color : public XMLTree::String {
    public:
        /*!
         * \brief  Constructor with color string.
         * \param  inColorString  A string representing an" SVG color.
         * 
         * Here are the valid" SVG colors formats:
         *
         * <table>
         * <tr><th> Format          </th><th> Description             </th></tr>
         * <tr><td> #rgb            </td><td> Three-digit RGB notation</td></tr>
         * <tr><td> #rrggbb         </td><td> Six-digit RGB notation  </td></tr>
         * <tr><td> rgb(R, G, B)    </td>
         *           <td> Functional notation (integer from 0 to 255) </td></tr>
         * <tr><td> rgb(R%, G%, B%) </td>
         *                         <td> Functional notation (percent) </td></tr>
         * <tr><td> &lt;colorname>  </td><td> Keyword                 </td></tr>
         * </table>
         *
         * If you want to use a color name keyword, you should use constants 
         * defined in this class instead. If you need to use direct numerical 
         * values, you should use the RGBColor or Gray subclass to construct
         * the appropriate string.
         *
         * \note   No string format validation is performed to ensure that it
         *         conform to SVG specifications. Make sure of what you write.
         */
        explicit Color(const std::string &inColorString) 
        : XMLTree::String(inColorString) { }
        
        /*!
         * \name  Color Constants Keywords
         * 
         * Borowed from 
         * <a href="http://www.w3.org/TR/SVG/types.html#ColorKeywords">SVG color
         * keywords specifications</a>. Theses predefined colors should make 
         * your life easier.
         */
        //@{
        static const Color cAliceBlue;      //!< "aliceblue" SVG color
        static const Color cAntiqueWhite;   //!< "antiquewhite" SVG color
        static const Color cAqua;           //!< "aqua" SVG color
        static const Color cAquaMarine;     //!< "aquamarine" SVG color
        static const Color cAzure;          //!< "azure" SVG color
        static const Color cBeige;          //!< "beige" SVG color
        static const Color cBisque;         //!< "bisque" SVG color
        static const Color cBlack;          //!< "black" SVG color
        static const Color cBlacheDalmond;  //!< "blanchedalmond" SVG color
        static const Color cBlue;           //!< "blue" SVG color
        static const Color cBlueViolet;     //!< "blueviolet" SVG color
        static const Color cBrown;          //!< "brown" SVG color
        static const Color cBurlyWood;      //!< "burlywood" SVG color
        static const Color cCadetBlue;      //!< "cadetblue" SVG color
        static const Color cChartreuse;     //!< "chartreuse" SVG color
        static const Color cChocolate;      //!< "chocolate" SVG color
        static const Color cCoral;          //!< "coral" SVG color
        static const Color cCornFlowerBlue; //!< "cornflowerblue" SVG color
        static const Color cCornSilk;       //!< "cornsilk" SVG color
        static const Color cCrimson;        //!< "crimson" SVG color
        static const Color cCyan;           //!< "cyan" SVG color
        static const Color cDarkBlue;       //!< "darkblue" SVG color
        static const Color cDarkCyan;       //!< "darkcyan" SVG color
        static const Color cDarkGoldenRod;  //!< "darkgoldenrod" SVG color
        static const Color cDarkGray;       //!< "darkgray" SVG color
        static const Color cDarkGreen;      //!< "darkgreen" SVG color
        static const Color cDarkGrey;       //!< "darkgrey" SVG color
        static const Color cDarkKhaki;      //!< "darkkhaki" SVG color
        static const Color cDarkMagenta;    //!< "darkmagenta" SVG color
        static const Color cDarkOliveGreen; //!< "darkolivegreen" SVG color
        static const Color cDarkOrange;     //!< "darkorange" SVG color
        static const Color cDarkOrchid;     //!< "darkorchid" SVG color
        static const Color cDarkRed;        //!< "darkred" SVG color
        static const Color cDarkSalmon;     //!< "darksalmon" SVG color
        static const Color cDarkSeaGreen;   //!< "darkseagreen" SVG color
        static const Color cDarkSlateBlue;  //!< "darkslateblue" SVG color
        static const Color cDarkSlateGray;  //!< "darkslategray" SVG color
        static const Color cDarkSlateGrey;  //!< "darkslategrey" SVG color
        static const Color cDarkTurquoise;  //!< "darkturquoise" SVG color
        static const Color cDarkViolet;     //!< "darkviolet" SVG color
        static const Color cDeepPink;       //!< "deeppink" SVG color
        static const Color cDeepSkyBlue;    //!< "deepskyblue" SVG color
        static const Color cDimGray;        //!< "dimgray" SVG color
        static const Color cDimGrey;        //!< "dimgrey" SVG color
        static const Color cDodgerBlue;     //!< "dodgerblue" SVG color
        static const Color cFireBrick;      //!< "firebrick" SVG color
        static const Color cFloralWhite;    //!< "floralwhite" SVG color
        static const Color cForestGreen;    //!< "forestgreen" SVG color
        static const Color cFuchsia;        //!< "fuchsia" SVG color
        static const Color cGainsboro;      //!< "gainsboro" SVG color
        static const Color cGostWhite;      //!< "ghostwhite" SVG color
        static const Color cGold;           //!< "gold" SVG color
        static const Color cGoldenRod;      //!< "goldenrod" SVG color
        static const Color cGray;           //!< "gray" SVG color
        static const Color cGrey;           //!< "grey" SVG color
        static const Color cGreen;          //!< "green" SVG color
        static const Color cGreenYellow;    //!< "greenyellow" SVG color
        static const Color cHoneyDew;       //!< "honeydew" SVG color
        static const Color cHotPink;        //!< "hotpink" SVG color
        static const Color cIndianRed;      //!< "indianred" SVG color
        static const Color cIndigo;         //!< "indigo" SVG color
        static const Color cIvory;          //!< "ivory" SVG color
        static const Color cKhaki;          //!< "khaki" SVG color
        static const Color cLavender;       //!< "lavender" SVG color
        static const Color cLavenderBlush;  //!< "lavenderblush" SVG color
        static const Color cLawnGreen;      //!< "lawngreen" SVG color
        static const Color cLemonChiffon;   //!< "lemonchiffon" SVG color
        static const Color cLightBlue;      //!< "lightblue" SVG color
        static const Color cLightCoral;     //!< "lightcoral" SVG color
        static const Color cLightCyan;      //!< "lightcyan" SVG color
        static const Color cLightGolderRodYellow; //!< "lightgoldenrodyellow" SVG color
        static const Color cLightGray;      //!< "lightgray" SVG color
        static const Color cLightGreen;     //!< "lightgreen" SVG color
        static const Color cLightGrey;      //!< "lightgrey" SVG color
        static const Color cLigntPink;      //!< "lightpink" SVG color
        static const Color cLightSalmon;    //!< "lightsalmon" SVG color
        static const Color cLightSeaGreen;  //!< "lightseagreen" SVG color
        static const Color cLightSkyBlue;   //!< "lightskyblue" SVG color
        static const Color cLightSlateGray; //!< "lightslategray" SVG color
        static const Color cLightSlateGrey; //!< "lightslategrey" SVG color
        static const Color cLightSteelBlue; //!< "lightsteelblue" SVG color
        static const Color cLightYellow;    //!< "lightyellow" SVG color
        static const Color cLime;           //!< "lime" SVG color
        static const Color cLimeGreen;      //!< "limegreen" SVG color
        static const Color cLinen;          //!< "linen" SVG color
        static const Color cMagenta;        //!< "magenta" SVG color
        static const Color cMaroon;         //!< "maroon" SVG color
        static const Color cMediumAquaMarine; //!< "mediumaquamarine" SVG color
        static const Color cMediumBlue;       //!< "mediumblue" SVG color
        static const Color cMediumOrchid;     //!< "mediumorchid" SVG color
        static const Color cMediumPurple;     //!< "mediumpurple" SVG color
        static const Color cMeidumSeaGreen;   //!< "mediumseagreen" SVG color
        static const Color cMediumSlateBlue;  //!< "mediumslateblue" SVG color
        static const Color cMediumSpringGreen;//!< "mediumspringgreen" SVG color
        static const Color cMediumTurquoise;  //!< "mediumturquoise" SVG color
        static const Color cMediumVioletRed;  //!< "mediumvioletred" SVG color
        static const Color cModNightBlue;   //!< "midnightblue" SVG color
        static const Color cMintCream;      //!< "mintcream" SVG color
        static const Color cMistyRose;      //!< "mistyrose" SVG color
        static const Color cMoccasin;       //!< "moccasin" SVG color
        static const Color cNavajoWhite;    //!< "navajowhite" SVG color
        static const Color cNavy;           //!< "navy" SVG color
        static const Color cOldLace;        //!< "oldlace" SVG color
        static const Color cOlive;          //!< "olive" SVG color
        static const Color cOliveDrab;      //!< "olivedrab" SVG color
        static const Color cOrange;         //!< "orange" SVG color
        static const Color cOrangered;      //!< "orangered" SVG color
        static const Color cOrchid;         //!< "orchid" SVG color
        static const Color cPaleGoldenRod;  //!< "palegoldenrod" SVG color
        static const Color cPaleGreen;      //!< "palegreen" SVG color
        static const Color cPaleTurquoise;  //!< "paleturquoise" SVG color
        static const Color cPaleVioletRed;  //!< "palevioletred" SVG color
        static const Color cPapayaWhip;     //!< "papayawhip" SVG color
        static const Color cPeachPuff;      //!< "peachpuff" SVG color
        static const Color cPeru;           //!< "peru" SVG color
        static const Color cPink;           //!< "pink" SVG color
        static const Color cPlum;           //!< "plum" SVG color
        static const Color cPowderBlue;     //!< "powderblue" SVG color
        static const Color cPurple;         //!< "purple" SVG color
        static const Color cRed;            //!< "red" SVG color
        static const Color cRosyBrown;      //!< "rosybrown" SVG color
        static const Color cRoyalBlue;      //!< "royalblue" SVG color
        static const Color cSaddleBrown;    //!< "saddlebrown" SVG color
        static const Color cSalmon;         //!< "salmon" SVG color
        static const Color cSandyBrown;     //!< "sandybrown" SVG color
        static const Color cSeaGreen;       //!< "seagreen" SVG color
        static const Color cSeaShell;       //!< "seashell" SVG color
        static const Color cSienna;         //!< "sienna" SVG color
        static const Color cSilver;         //!< "silver" SVG color
        static const Color cSkyBlue;        //!< "skyblue" SVG color
        static const Color cSlateBlue;      //!< "slateblue" SVG color
        static const Color cSlateGray;      //!< "slategray" SVG color
        static const Color cSlateGrey;      //!< "slategrey" SVG color
        static const Color cSnow;           //!< "snow" SVG color
        static const Color cSpringGreen;    //!< "springgreen" SVG color
        static const Color cSteelBlue;      //!< "steelblue" SVG color
        static const Color cTan;            //!< "tan" SVG color
        static const Color cTeal;           //!< "teal" SVG color
        static const Color cThistle;        //!< "thistle" SVG color
        static const Color cTomato;         //!< "tomato" SVG color
        static const Color cTurquoise;      //!< "turquoise" SVG color
        static const Color cViolet;         //!< "violet" SVG color
        static const Color cWheat;          //!< "wheat" SVG color
        static const Color cWhite;          //!< "white" SVG color
        static const Color cWhiteSmoke;     //!< "whitesmoke" SVG color
        static const Color cYellow;         //!< "yellow" SVG color
        static const Color cYellowGreen;    //!< "yellowgreen" SVG color
		static const Color cNone;			//!< not a color
        //@}
    };

    /*!
     * \brief  Color defined with red, green, and blue values.
     */
    class RGBColor : public Color {
    public:
        /*!
         * \brief  Construct a color using red, green, and blue values.
         * \param  inRed    An amount of red for this color, between 0 to 1.
         * \param  inGreen  An amount of green for this color, between 0 to 1.
         * \param  inBlue   An amount of blue for this color, between 0 to 1.
         */
        RGBColor(float inRed, float inGreen, float inBlue);
    };

    /*!
     * \brief  Color as a shade of gray.
     */
    class Gray : public RGBColor {
    public:
        /*!
         * \brief  Construct a shade of gray using white intencity value.
         * \param  inIntencity  Intencity of white, between 0 to 1.
         */
        Gray(float inIntencity) 
        : RGBColor(inIntencity, inIntencity, inIntencity) { }
    };
    
    class PointList;

    /*!
     * \brief  Represent a point on a plane.
     * \note   Paramenters <code>x</code> et <code>y</code> are public to allow
     *         keeping a simple notation without "get" or "set" functions.
     */
    class Point {
    public:
        float x; //!< Coordinate on the ordinate axis.
        float y; //!< Coordinate on the abscissa axis.

        /*!
         * \brief  Angle of the point make from origin with the X axis.
         */
        float angle() const { return std::atan(y/x); }
        
        /*!
         * \brief  Distance from the origin.
         */
        float distance() const { return std::sqrt(x*x + y*y); }

        /*!
         * \brief  Rotate the point around the origin.
         */
        void rotate(float angle) {
            using namespace std;
            *this = Point(x*cos(angle)-y*sin(angle), x*sin(angle)+y*cos(angle));
        }

        /*!
         * \brief  Default point constructor giving a point with origin
         *         coordinates.
         */
        Point() : x(0), y(0) { }

        /*!
         * \brief  Point constructor with explicit coordinates. 
         */
        Point(float inX, float inY) : x(inX), y(inY) { }

        /*!
         * \brief  Translate the point.
         * \param  inDX  A distance on the x axis.
         * \param  inDY  A distance on the y axis.
         */
        void moveBy(float inDX, float inDY) {
            x += inDX;
            y += inDY;
        }
        /*!
         * \brief  Scale the point around origin.
         * \param  inFactor  A factor to multiply the points components by.
         */
        void scaleBy(float inFactor) {
            x *= inFactor;
            y *= inFactor;
        }

        /*!
         * \brief   Give a string representation of this point.
         * \return  A string representation of this point.
         */
        XMLTree::String getStringValue() const;

        /*!
         * \brief   Append points to this one forming a new PointList.
         * \param   inPoints  A list of point to append after this one.
         * \return  The resulting list of points.
         */
        PointList operator + (const PointList &inPoints) const;
        
        /*!
         * \brief   Append a point to this one forming a PointList.
         * \param   inPoint  A point to append after this one.
         * \return  The resulting list of points.
         */
        PointList operator + (const Point &inPoint) const;
    };

    /*!
     * \brief  List of points.
     * 
     * This class is a standard vector of point (std::vector<Point>) with some
     * utility functions and + and += operators to facilitate the concatenation
     * of point lists.
     */
    class PointList : public std::vector<Point> {
    public:
        /*!
         * \brief  Default constructor providing an empty point list.
         */
        PointList();
        /*!
         * \brief  Construct a point list with a single point.
         * \param  inPoint  A point to put in the point list.
         */
        PointList(const Point inPoint);
        //@{
        /*!
         * \brief   Append another point list after this one.
         * \param   inPoints  Points to append after this point list.
         * \return  The resulting point list.
         */
        PointList operator + (const PointList &inPoints) const;
        PointList &operator += (const PointList &inPoints);
        //@}
        //@{
        /*!
         * \brief   Append another point at the end of this list.
         * \param   inPoint  Point to append after this list.
         * \return  The resulting point list.
         */
        PointList operator + (const Point &inPoint) const;
        PointList &operator += (const Point &inPoint);
        //@}

        /*!
         * \brief   Give a string representation of this point list.
         * \return  A string representation of this point list.
         */
        XMLTree::String getStringValue() const;
    };
    
    /*!
     * \brief  Represent a rectangular size on a plane.
     * \note   Paramenters <code>width</code> and <code>height</code> are public
     *         to allow keeping a simple notation without "get" or "set"
     *         functions.
     */
    class Size {
    public:
        float width; //!< Horizontal size component.
        float height; //!< Vertical size component.

        /*!
         * \brief  Length of the diagonal.
         */
        float diagonal() const {
            return std::sqrt(width*width + height*height);
        }

        /*!
         * \brief  Default size constructor giving a size with zero components.
         */
        Size() : width(0), height(0) { }

        /*!
         * \brief  Size constructor with explicit components.
         */
        Size(float inWidth, float inHeight) 
        : width(inWidth), height(inHeight) { }

        /*!
         * \brief   Give a string representation of this size.
         * \return  A string representation of this size.
         */
        XMLTree::String getStringValue() const;
        
        /*!
         * \brief  Scale the size by a factor.
         * \param  inFactor  A factor to multiply the size components by.
         */
        void scaleBy(float inFactor) {
            width *= inFactor;
            height *= inFactor;
        }
    };

    /*!
     * @}
     */

}

#endif
