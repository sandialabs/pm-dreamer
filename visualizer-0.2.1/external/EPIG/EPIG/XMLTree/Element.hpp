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
 * \file  EPIG/XMLTree/Element.hpp
 */

#ifndef EPIG_XMLTree_Element_hpp_
#define EPIG_XMLTree_Element_hpp_

#include "EPIG/XMLTree/Node.hpp"
#include "EPIG/XMLTree/String.hpp"
#include "EPIG/XMLTree/Attribute.hpp"

namespace EPIG {
namespace XMLTree {

    /*!
     * \brief  A node representing a XML element (a tag).
     *
     * An XML document always can be represented as a tree of elements. Elements
     * contains a list of attributes and a list of nodes, called the content,
     * that can contain text or other elements.
     */
    class Element : public Node {
    public:
        /*!
         * \brief  Constructor with name and optional attribute list.
         * \param  inName        A string with the name for the element.
         * \param  inAttributes  A list of attributes to give to this element.
         */
        Element(const std::string &inName, 
                const AttributeList &inAttributes = AttributeList());

        /*!
         * \brief   Get the name of this element.
         * \return  A string with the name of this element.
         */
        std::string getName() const;
        
        /*!
         * \brief   Get id attribute value for this element.
         * \return  A string with the identifier for this element.
         */
        String getIdentifier() const;
        /*!
         * \brief  Set the value of the id attribute for this element.
         * \param  inIdentifier  A string with the new identifier.
         */
        void setIdentifier(const String &inIdentifier);
        /*!
         * \brief  Remove the id attribute for this element.
         */
        void clearIdentifier();

        /*!
         * \brief   Check presence of an attribute.
         * \param   inName  The name of the attribute.
         * \return  True if the attribute is defined, false if not defined.
         */
        bool hasAttribute(const std::string &inName) const;
        /*!
         * \brief   Get the value of an attribute.
         * \param   inName  The name of the attribute.
         * \return  The attribute value as a string.
         */
        String getAttribute(const std::string &inName) const;
        /*!
         * \brief   Set the value of an attribute.
         * \param   inName   The name of the attribute to set.
         * \param   inValue  The value to give to the attribute.
         */
        void setAttribute(const std::string &inName, const String &inValue);
        /*!
         * \brief  Set the value of multiple attributes.
         * \param  inAttributes  A list of attributes to set.
         */
        void setAttributes(const AttributeList &inAttributes);
        /*!
         * \brief   Append a string at the end of the value of an attribute.
         * \param   inName      The name of the attribute to set.
         * \param   inAddition  The string to put at the end of the value.
         */
        void appendToAttribute(const std::string &inName, const String &inAddition);
        /*!
         * \brief  Remove an attribute definition.
         * \param  inName  The name of the attribute.
         *
         * Does nothing in the case the attribute is not present.
         */
        void removeAttribute(const std::string &inName);

        /*!
         * \brief   Append an element to the content.
         * \param   inNode  A node to append.
         * \return  A reference to this element.
         *
         * The node is appended at the end of the content of this element.
         */
        Element &operator << (const Node &inNode);

        /*!
         * \brief   Get the content of this element.
         * \return  A list of node with the content.
         */
        NodeList getContent() const;
        /*!
         * \brief   Get attributes attached.
         * \return  List of the attributes.
         */
        AttributeList getAttributes() const;

        /*!
         * \brief  Replace the content.
         * \param  inContent  A list of node to replace the content with.
         */
        void setContent(const NodeList &inContent);
        
        /*!
         * \brief   Get the number of children nodes of this element.
         * \return  The count of children nodes.
         */
        unsigned getChildCount() const;
        
        //@{
        /*!
         * \brief   Get a child element by identifier.
         * \param   inIdentifier  Identifier string of the child.
         * \return  A pointer to the found element, or 0 if the element was not
         *          found.
         */
        Element *getChild(const String &inIdentifier);
        const Element *getChild(const String &inIdentifier) const;
        //@}
        //@{
        /*!
         * \brief   Get a child node by index.
         * \param   inIndex  Index of the child in this element.
         * \return  A pointer to the node at index.
         * \throw   std::out_of_range  If inIndex is an invalid index.
         */
        Node *getChild(unsigned inIndex);
        const Node *getChild(unsigned inIndex) const;
        //@}
        
        static const NodeType cNodeType; //!< Node type for elements.
        
        //! Attribute name for specifying namespaces.
        static const std::string cXMLNamespaceAttributeName;

    protected:
        class AbstractData;
        
        /*!
         * \brief  Construct an element from new abstract data.
         * \param  inData  Element data provided by the subclass.
         *
         * This constructor is provided to allow subclasses to use a custom
         * data storage class.
         */
        Element(AbstractData *inData);

        const AbstractData &getData() const
        { return (const AbstractData &)Node::getData(); }
        
        AbstractData &getData()
        { return (AbstractData &)Node::getData(); }
        
    private:
        class Data;
        
        //! Attribute name for id.
        static const std::string cIdentifierAttributeName; 
    };

    /*!
     * \internal
     * \brief  Class holding element's associated data in a node.
     */
    class Element::AbstractData : public Node::Data {
    public:
        AbstractData() : Node::Data(eElement) { }
        
        /*!
         * \brief   Get the name of this element.
         * \return  A string with the name of this element.
         */
        virtual std::string getName() const = 0;

        /*!
         * \brief   Check presence of an attribute.
         * \param   inName  The name of the attribute.
         * \return  True if the attribute is defined, false if not defined.
         */
        virtual bool hasAttribute(const std::string &inName) const = 0;
        /*!
         * \brief   Get the value of an attribute.
         * \param   inName  The name of the attribute.
         * \return  The attribute value as a string.
         */
        virtual String getAttribute(const std::string &inName) const = 0;
        /*!
         * \brief   Set the value of an attribute.
         * \param   inName   The name of the attribute to set.
         * \param   inValue  The value to give to the attribute.
         */
        virtual void setAttribute(const std::string &inName, 
                                  const String &inValue) = 0;
        /*!
         * \brief  Set the value of multiple attributes.
         * \param  inAttributes  A list of attributes to set.
         */
        virtual void setAttributes(const AttributeList &inAttributes) = 0;
        /*!
         * \brief   Append a string at the end of the value of an attribute.
         * \param   inName      The name of the attribute to set.
         * \param   inAddition  The string to put at the end of the value.
         */
        virtual void appendToAttribute(const std::string &inName, 
                                       const String &inAddition) = 0;
        /*!
         * \brief  Remove an attribute definition.
         * \param  inName  The name of the attribute.
         *
         * Does nothing in the case the attribute is not present.
         */
        virtual void removeAttribute(const std::string &inName) = 0;

        /*!
         * \brief   Append an element to the content.
         * \param   inNode  A node to append.
         *
         * The node is appended at the end of the content of this element.
         */
        virtual void append(const Node &inNode) = 0;

        /*!
         * \brief   Get the content of this element.
         * \return  A list of node with the content.
         */
        virtual NodeList getContent() const = 0;
        /*!
         * \brief   Get attributes attached.
         * \return  List of the attributes.
         */
        virtual AttributeList getAttributes() const = 0;

        /*!
         * \brief  Replace the content.
         * \param  inContent  A list of node to replace the content with.
         */
        virtual void setContent(const NodeList &inContent) = 0;
        
        virtual unsigned getChildCount() const = 0;
        
        virtual Element *getChild(const String &inIdentifier) = 0;
        virtual const Element *getChild(const String &inIdentifier) const = 0;
        
        virtual Node *getChild(unsigned inIndex) = 0;
        virtual const Node *getChild(unsigned inIndex) const = 0;
    };
    
    /*!
     * \internal
     */
    class Element::Data : public Element::AbstractData {
    public:
        Data(const std::string &inName, const AttributeList &inAttributes);

        virtual std::string getName() const;

        virtual bool hasAttribute(const std::string &inName) const;
        virtual String getAttribute(const std::string &inName) const;
        virtual void setAttribute(const std::string &inName,
                                  const String &inValue);
        virtual void setAttributes(const AttributeList &inAttributes);
        virtual void appendToAttribute(const std::string &inName, 
                                       const String &inAddition);
        virtual void removeAttribute(const std::string &inName);
        virtual void append(const Node &inNode);
        virtual NodeList getContent() const;
        virtual AttributeList getAttributes() const;

        virtual void setContent(const NodeList &inContent);
        
        virtual unsigned getChildCount() const;
        
        virtual Element *getChild(const String &inIdentifier);
        virtual const Element *getChild(const String &inIdentifier) const;
        
        virtual Node *getChild(unsigned inIndex);
        virtual const Node *getChild(unsigned inIndex) const;
        
        // Actual definitions of abstract Node method.
        virtual void write(std::ostream &inStream) const;
        virtual Data *clone() const;

    private:
        std::string mName; //!< The tag name
        AttributeList mAttributes; //!< List of attributes
        NodeList mContent; //!< Content
    };

} // namespace XMLTree
} // namespace EPIG

#endif
