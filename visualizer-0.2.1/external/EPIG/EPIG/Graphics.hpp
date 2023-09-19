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
 * \file  EPIG/Graphics.hpp
 */

#ifndef EPIG_Graphics_hpp_
#define EPIG_Graphics_hpp_

#include <stdexcept>
#include "EPIG/XMLTree/XMLTree.hpp"
#include "EPIG/Styles.hpp"
#include "EPIG/Transforms.hpp"

namespace EPIG {
    
    class BadIdentifierException;
    class IncompatibleClassException;

    /*!
     * \defgroup  graphics  Graphic classes
     */

    /*!
     * \brief  Base class for graphic elements.
     * \ingroup  graphics
     *
     * This class provide no more functionality than it's base class. It is
     * intended to be able to distinguish graphics element from others. That way
     * we can restrict at compile time what kind of elements can be inserted in
     * a Group for example.
     *
     * This class must be subclassed to be useful. You may note that the only
     * constructor is protected to enforce that.
     */
    class Graphic : protected XMLTree::Element {
    public:
        /*!
         * \brief  Add some style attributes to element.
         * \param  inStyle  A style attribute list to add to this element.
         *
         * If the same style attribute with the same name is already present, it
         * will be replaced by the new value.
         */
        void addStyle(const Style &inStyle);
        /*!
         * \brief  Set the coordinate transformation to apply.
         * \param  inTransform  A transformation to apply to this element.
         *
         * The current transformation, if any, will be replaced by the new one.
         *
         * \see transformcomp
         */
        void setTransform(const Transform &inTransform);
        /*!
         * \brief  Remove any coordinate transformation applyed to this element.
         */
        void clearTransform();
        
        /* Keep identifier accessors from XMLTree::Attribute accessibles */
        using XMLTree::Element::setIdentifier;
        using XMLTree::Element::getIdentifier;
        using XMLTree::Element::clearIdentifier;
        
        static const XMLTree::String cSVGNamespace; //!< Name for the SVG XML namespace
        static const std::string cElementName; //!< Empty name element for this graphic.
        
    protected:
        /*!
         * \brief  Constructor for a graphic element.
         * \param  inName        A tag name for the element.
         * \param  inAttributes  A list of attributes to give to this element.
         */
        Graphic(const XMLTree::String &inName, const XMLTree::AttributeList &inAttributes = XMLTree::AttributeList()) 
        : XMLTree::Element(inName, inAttributes) { }
        
        friend class Group; //!< Allowing the use of Element's << operator.

        static const std::string cXAttributeName; //!< Attribute name for x position.
        static const std::string cYAttributeName; //!< Attribute name for y position.
        static const std::string cWidthAttributeName; //!< Attribute name for width.
        static const std::string cHeightAttributeName;//!< Attribute name for height.
        static const std::string cTransformAttributeName; //!< Attribute name for transform.
    };
    
    /*!
     * \brief  Base graphic container.
     * \ingroup  graphics
     *
     * A group hold an ordered list of graphics. As for other graphics elements, 
     * you can apply a Style to the group. Any child element inside that does 
     * not have a different definition for a given style attribute will inherit
     * the one for the group.
     *
     * You can also apply a transformation to a group. The transform will apply
     * to all contained elements.
     *
     * Finally, you can give an identifier to a group. That allows the group to
     * be found when nested in other groups.
     */
    class Group : public Graphic {
    public:
        /*!
         * \brief  Default constructor.
         * \param  inStyle   Optional default style attributes to give to 
         *                   inserted graphics.
         * 
         * Give an empty group.
         */
        explicit Group(const Style &inStyle = Style());
        
        /*!
         * \brief   Insertion operator.
         * \param   inGraphic  A graphic element to insert into the group.
         * \return  A reference to the group.
         *
         * The graphic is put at the top and may cover other objects.
         */
        Group &operator << (const Graphic &inGraphic);
        
        /*!
         * \brief   Return the number of children in this group.
         * \return  The number of children of this group.
         */
        unsigned count() const;

        /*!
         * \brief  Remove all elements from the group.
         */
        void clear();
        
        //@{
        /*!
         * \brief   Get a reference to the child graphic found with identifier.
         * \param   inIdentifier  An indentifier to find the child graphic inside
         *                        of the canvas.
         * \return  A reference to the desired graphic.
         *
         * Looks recursively into the children tree to find a graphic with
         * the specified identifier.
         * 
         * Function returns a reference to an object of the type determined in 
         * template. This allow the use of this function to set class-specific
         * propreties. Here is an example:
         *
         * <code>myGroup.get&lt;Circle&gt;("id").setRadius(10);</code>
         *
         * \throw  BadIdentifierException  When a child with that identifier
         *                                 could not be found.
         * \throw  IncompatibleClassException  When the object found is 
         *                                     incompatible with requested 
         *                                     class from template parameter.
         */
        template <class graphicClass>
        graphicClass &get(const std::string &inIdentifier) 
        { return classCast<graphicClass>(getChild(inIdentifier)); }
        
        template <class graphicClass>
        const graphicClass &get(const std::string &inIdentifier) const 
        { return classCast<graphicClass>(getChild(inIdentifier)); }
        //@}
        
        //@{
        /*!
         * \brief   Get a reference to the child graphic found with index.
         * \param   inIndex  The index of the child inside this group.
         * \return  A reference to the desired graphic.
         * 
         * This returns a reference to an object of the type determined in 
         * template. This allow the use of this function to set class-specific
         * propreties. Here is an example:
         *
         * <code>myGroup.get&lt;Circle&gt;("id").setRadius(10);</code>
         *
         * \throw  BadIdentifierException  When a child with that identifier
         *                                 could not be found.
         * \throw  IncompatibleClassException  When the object found is 
         *                                     incompatible with requested 
         *                                     class from template parameter.
         */
        template <class graphicClass>
        graphicClass &get(unsigned inIndex) 
        { return classCast<graphicClass>(getChild(inIndex)); }
        
        template <class graphicClass>
        const graphicClass &get(unsigned inIndex) const 
        { return classCast<graphicClass>(getChild(inIndex)); }
        //@}
        
    protected:
        /*!
         * \brief  Utility constructor for subclass that need a custom element.
         * \param  inTagName     Name of the sublcass element.
         * \param  inAttributes  XMLTree::Attribute list to give to the element.
         * 
         * This class is only provided for sublcassers that need to keep group
         * functionalities but use a different tag name.
         * 
         * \see  Frame::Frame()
         */
        Group(const std::string &inTagName, 
              const XMLTree::AttributeList &inAttributes = XMLTree::AttributeList());

        //@{
        /*!
         * \internal
         * \brief  Callback for Group::get used to solve a Visual Studio.NET
         *         compiler bug.
         *
         * This allow for accessing Group::get from a sublcass without using
         * Group::get<gc> that crash Visual Studio.NET compiler in some obscure
         * circonstances.
         */
		template <class graphicClass>
        graphicClass &getGraphic(const std::string &inIdentifier) 
        { return get<graphicClass>(inIdentifier); }
        
        template <class graphicClass>
        graphicClass &getGraphic(unsigned inIndex) 
        { return get<graphicClass>(inIndex); }
        //@}

    private:
        //@{
        /*!
         * \brief  Check graphic class compatibliliy before returning a 
         *         reference to the node as the requested graphic class.
         * \param  inNode  Pointer to the node.
         *
         * This function calls checkElement that will throw the appropriates
         * exceptions in some circonstances.
         */
        template <class graphicClass>
		static graphicClass &classCast(XMLTree::Node *inNode) {
            if ( sizeof(graphicClass) != sizeof(XMLTree::Node) )
            { throw IncompatibleClassException("Group::classCast(): "
                "Cannot cast to a class that extends data members or use "
                "virtual functions."); }
            // ** Important **
            // Compiler will make an error below if trying to cast to a class
            // that is not a valid graphic class.
            checkElement(inNode, graphicClass::cElementName);
            return static_cast<graphicClass &>(*inNode);
        }
        template <class graphicClass>
		static const graphicClass &classCast(const XMLTree::Node *inNode) {
            if ( sizeof(graphicClass) != sizeof(XMLTree::Node) )
            { throw IncompatibleClassException("Group::classCast(): "
                "Cannot cast to a class that extends data members or use "
                "virtual functions."); }
            // ** Important **
            // Compiler will make an error below if trying to cast to a class
            // that is not a valid graphic class.
            checkElement(inNode, graphicClass::cElementName);
            return static_cast<const graphicClass &>(*inNode);
        }
        //@}
        
        /*!
         * \brief  Check the element validity.
         * \param  inNode                  Node to check.
         * \param  inRequestedElementName  Element name needed to be valid.
         *
         * This function helps classCast by throwing the right exceptions in
         * case
         *
         * \throw  BadIdentifierException  When the node is null.
         * \throw  IncompatibleClassException  When the node is not an element 
         *         or when the element name does not match the requested one.
         */
        static void checkElement(const Node *inNode, 
                                 const std::string &inRequestedElementName);
    
        static const std::string cElementName; //<! Element's tag name
    };

    /*!
     * \brief  Graphic container with a clipping rectangle.
     * \ingroup  graphics
     *
     * The frame establish a clipping rectangle for the elements it can contain.
     * Coordinates for elements inside the frame are translated to the frame
     * origin.
     */
    class Frame : public Group {
    public:
        /*!
         * \brief  Frame constructor specifying origin, size and an optional
         *         attribute list.
         * \param  inOrigin  A point to be the origin corner.
         * \param  inSize    Width and height of the frame's rectangle.
         * \param  inStyle   Optional default style attributes to give to 
         *                   inserted graphics.
         */
        Frame(const Point &inOrigin, const Size &inSize, 
              const Style &inStyle = Style());

        /*!
         * \brief   Get the origin coordinates of this frame's rectangle.
         * \return  A point with origin coordinates.
         */
        Point getOrigin() const;
        /*!
         * \brief  Set the origin coordinates of this frame's rectangle.
         * \param  inOrigin  A new origin point.
         */
        void setOrigin(const Point &inOrigin);
        /*!
         * \brief  Set the origin coordinates of this frame's rectangle.
         * \param  inX  A new origin point x coordinate.
         * \param  inY  A new origin point y coordinate.
         */
        void setOrigin(float inX, float inY);

        /*!
         * \brief   Get the size of the frame's rectangle.
         * \return  A size value corresponding to the rectangle size.
         */
        Size getSize() const;
        /*!
         * \brief  Set the size of the frame's rectangle.
         * \param  inSize  A new size value for the rectangle.
         */
        void setSize(const Size &inSize);
        /*!
         * \brief  Set the size of the frame's rectangle.
         * \param  inWidth   A new width for the rectangle.
         * \param  inHeight  A new height for the rectangle.
         */
        void setSize(float inWidth, float inHeight);
         
        static const std::string cElementName; //!< Element's tag name.
        
    private:
        friend class SVGDocument; //!< Needed for using frame as the root element.
    };
    
    /*!
     * \brief  Document holding SVG graphics.
     */
    class SVGDocument : public XMLTree::Document {
    public:
        /*!
         * \brief  Constructor from a frame.
         * \param  inTitle      A title for this document.
         * \param  inRootFrame  A root frame for this document.
         * \param  inEncoding   An optional text encoding.
         *
         * If you do not spefify a character encoding, Document use the default
         * encoding provided by Encoding::getDefault().
         *
         * By default the document will use cartesian Y-up coordinates. If 
         * you made your graphics using Y-down coordinates standard to SVG and 
         * some others graphic systems you should use
         * SVGDocument(const std::string &, const Frame &, bool, 
         *             const std::string &) and set inIsCartesian to false. 
         */
        SVGDocument(const std::string &inTitle, const Frame &inRootFrame, 
                    const std::string &inEncoding = 
                                            XMLTree::Encoding::getDefault());
                                            
        /*!
         * \brief  Constructor from a frame.
         * \param  inTitle        A title for this document.
         * \param  inRootFrame    A root frame for this document.
         * \param  inIsCartesian  Indicate if this document is using a cartesian 
         *                        coordinate system, opposed to Y-down 
         *                        coordinates standard to SVG and some other 
         *                        graphic systems.
         * \param  inEncoding   An optional text encoding.
         *
         * If you do not spefify a character encoding, Document use the default
         * encoding provided by Encoding::getDefault().
         * 
         * If this document is using a cartesian coordinate system some
         * transformations will be applyed to parts of the document to convert
         * from standard SVG Y-down coordinate system.
         */
        SVGDocument(const std::string &inTitle, const Frame &inRootFrame, 
                    bool inIsCartesian, const std::string &inEncoding 
                                        = XMLTree::Encoding::getDefault());
        
    private:
        void addTitle(const std::string &inTitle);
        void convertToCartesian();
        void unflip(XMLTree::Element &inElement);
    
        static const std::string cDocType; //!< SVG doctype.
    };
    
    /*!
     * Indicate an attempt to access a graphic inside a group using an 
     * identifier that does not exists.
     */
    class BadIdentifierException : public std::runtime_error {
    public:
        //! Create an exception with the description.
        BadIdentifierException(const char inDescription[])
        : std::runtime_error(inDescription) { }
    };
    
    /*!
     * Indicate an attempt to cast to a graphic class that is incompatible with
     * the actual one.
     */
    class IncompatibleClassException : public std::runtime_error {
    public:
        //! Create an exception with the description.
        IncompatibleClassException(const char inDescription[])
        : std::runtime_error(inDescription) { }
    };

}

#endif
