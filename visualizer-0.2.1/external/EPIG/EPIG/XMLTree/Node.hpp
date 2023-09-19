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
 * \file  EPIG/XMLTree/Node.hpp
 */

#ifndef EPIG_XMLTree_Node_hpp_
#define EPIG_XMLTree_Node_hpp_

#include <iostream>
#include <vector>
#include <map>

namespace EPIG {
namespace XMLTree {

    class String;
    class Element;
    class NodeList;

    /*!
     * \brief  Data type for the node.
     */
    enum NodeType { eElement = 1, eAttribute, eText, eCDataSection, 
                    eEntity, eEntityReference, eProcessingInstruction, eComment, 
                    eDocument, eDocumentType, eDocumentFragment, eNotation };

    /*!
     * \brief  Common base type for content of an XML Document.
     *
     * This class provide basic functions for XML Node elements. A node keep a
     * smart pointer to a reference counted Node::Data class so that if you
     * assign a Node superclass (like Element or String) to a Node object (with
     * the = assignement operator), any data stored in the Data object is not
     * lost. This is critical when a Element is reduced to a Node (in a
     * NodeList for example).
     *
     * Node reference counted Data won't be duplicated when you make a copy of
     * a node. It will be however before you change the contend of the data
     * (by getting a non-const pointer) in order to preserve the illusion that
     * a copy of a Node is realy a different copy. This is very memory
     * efficient since it duplicate the data only if it is to be changed.
     */
    class Node {
    public:
        /*!
         * \brief  Copy constructor for a node.
         * \param  inNode  A node to copy.
         *
         * Node will use the same data pointer as inNode.
         */
        Node(const Node &inNode);
        /*!
         * \brief  Destruct the current node.
         *
         * The destructor will delete data from memory when needed.
         */
        ~Node();
        
        /*!
         * \brief  Write the node to the stream.
         * \param  inStream  A stream to write the note to.
         * \note   You may prefer to use the << operator.
         */
        void write(std::ostream &inStream) const;

        /*!
         * \brief  Assign a new node to this one.
         * \param  inNode  A node to take data from.
         *
         * Discard current data and use the data of inNode.
         */
        Node &operator = (const Node &inNode);
        
        /*!
         * \brief   Create a list from the two nodes.
         * \param   inNode  Another node to put in the list.
         * \return  A list made of two nodes.
         */
        NodeList operator + (const Node &inNode) const;
        /*!
         * \brief   Create a list from by appending more nodes.
         * \param   inNodeList  Node list to put after this node.
         * \return  The resulting list.
         */
        NodeList operator + (const NodeList &inNodeList) const;
        
        /*!
         * \brief   Get the node type.
         * \return  A node type.
         */
        NodeType getNodeType() const;
        
        /*!
         * \brief   Get this node as an XML string.
         * \return  A string with this node serialized in XML.
         */
        std::string getXMLValue() const;
        
#ifndef __MSVC__
    // Visual Studio.NET has some problems seeing a protected member class
    // from a protected inherited sublcass so we have to keep it public for the
    // MS Visual C++ compiler. On other compilers, let Node::Data be protected.
    protected:
#endif
        class Data;

	protected:
        /*!
         * \brief  Constructor with a data object.
         * \param  inData  The data to keep in this node.
         *
         * This constructor is provided to allow subclasses to use their own
         * data storage class.
         */
        Node(const Data *inData);

        //@{
        /*!
         * \brief   Return a reference to the data of the node.
         * \return  A reference to the node's data.
         *
         * This is useful for subclass that want to access the stored data.
         */
        const Data &getData() const { return *mData; }
        Data &getData();
        //@}

        /*!
         * \brief   Tell if the data is common between this node and another.
         * \param   inNode  A node to check.
         * \return  True if the data is the same object, false if not.
         */
        bool useSameDataAs(const Node &inNode) const
        { return mData == inNode.mData; }

    private:
        const Data *mData; //!< Pointer to data element
    };

    /*!
     * \internal
     * \brief  Abstract data holder for a node.
     *
     * This class provide the basis of the efficient Node data storage
     * mechanism. Node::Data maintain a retain count that keep track of the data
     * usage. When all node release the data, the retain count fall to zero and
     * the data delete itself from memory.
     *
     * The retain count is initaly set to one when creating the data. Each time
     * a new node reference the data, it call the retain() method wich increace
     * the retain count. When a node no longer reference the data, it need to
     * release() it.
     */
    class Node::Data {
    public:
        /*!
         * \brief  Virtual destructor for this data.
         *
         * Needed because of virtual functions in the class.
         */
        virtual ~Data();

        /*!
         * \brief  Write the node to the stream.
         * \param  inStream  A stream to write the note to.
         */
        virtual void write(std::ostream &inStream) const = 0;
        /*!
         * \brief   Provide a copy of this data.
         * \return  The exact same data with a retain count of one.
         */
        virtual Data *clone() const = 0;

        /*!
         * \brief   Tell the count of node that are using this data object.
         * \return  An integer greather than zero.
         */
        unsigned int getRetainCount() const { return mRetainCount; }
        
        /*!
         * \brief   Get the type of this data.
         */
        NodeType getNodeType() const { return mNodeType; }

    protected:
        /*!
         * \brief  Default constructor.
         * \param  inType  A node type represented by this data.
         *
         * Initialize the retain count to 1.
         */
        Data(NodeType inType);
        
        /*!
         * \brief   Get rid of this data and return the new data.
         * \param   inData  The new data to replace this one.
         * \return  A data object using the ndw data.
         * 
         * Used by nodes when operator = is called. Node replace actual data 
         * pointer by the one returned. Provided to allow special behaviour 
         * to take place when copying.
         *
         * \note    You should not override this unless you realy want to affect
         *          the copy behaviour of your node. Use with caution.
         */
        virtual const Data *replaceWith(const Data *inData) const;

    private:
        friend class Node;
        /*!
         * \brief  Increace the retain count by one.
         */
        void retain() const;
        /*!
         * \brief  Decreace the retain count by one.
         *
         * If the retain count reach zero, then the object is deleted as it is
         * no longer claimed by any node.
         */
        void release() const;
        
    private:
        mutable unsigned mRetainCount; //!< The retain count for this data
        const NodeType mNodeType;
        
        Data &operator = (const Data &inData);
    };

    /*!
     * \brief  A list of node.
     * 
     * This class represent an ordered list of node. It is implemented as a
     * std::vector and inherit all it's members.
     * NodeList define special behaviour allowing to add attributes to
     * it using the + operator.
     */
    class NodeList : public std::vector<Node> {
    public:
        /*!
         * \brief  Default constructor.
         *
         * Make an empty list of nodes.
         */
        NodeList();
        /*!
         * \brief  Constructor for a list of one node.
         * \param  inNode  A node to put in the list.
         */
        NodeList(const Node &inNode);

        //@{
        /*!
         * \brief   Concatenation operator
         * \param   inNodeList  A list of node to append to this list.
         * \return  The list with new nodes appended to it.
         */
        NodeList &operator += (const NodeList &inNodeList);
        NodeList operator + (const NodeList &inNodeList) const;
        //@}
        
        //@{
        /*!
         * \brief   Concatenation operator
         * \param   inNode  A node to append to this list.
         * \return  The list with the new node appended to it.
         */
        NodeList &operator += (const Node &inNode);
        NodeList operator + (const Node &inNode) const;
        //@}
        
        //@{
        /*!
         * \brief   Find an element in the list by identifier.
         * \param   inIdentifier  Identifier string of the element.
         * \return  A pointer to the found element, or 0 if the element was not
         *          found.
         *
         * This method search recursively into subelements.
         */
        Element *getElementById(const String &inIdentifier);
        const Element *getElementById(const String &inIdentifier) const;
        //@}
    };

    /*!
     * \brief   Output a node to an output stream.
     * \param   inStream  A stream to output the node to.
     * \param   inNode    A node to output to the stream.
     * \return  A reference to the stream.
     *
     * This method simply call the Node::write().
     */
    inline std::ostream &operator << (std::ostream &inStream, 
                                      const Node &inNode) {
        inNode.write(inStream);
        return inStream;
    }

} // namespace XMLTree
} // namespace EPIG

#endif
