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
 * \file  EPIG/XMLTree/Node.cpp
 */

#include <sstream>
#include "EPIG/XMLTree/Node.hpp"
#include "EPIG/XMLTree/Element.hpp"
#include "EPIG/XMLTree/String.hpp"

using namespace EPIG::XMLTree;

Node::Node(const Node &inNode) : mData(inNode.mData) { mData->retain(); }
Node::Node(const Data *inData) : mData(inData) { mData->retain(); }
Node::~Node() { mData->release(); }

void Node::write(std::ostream &inStream) const { getData().write(inStream); }

Node &Node::operator = (const Node &inNode) {
    mData = mData->replaceWith(inNode.mData);
    return *this;
}

NodeList Node::operator + (const Node &inNode) const {
    return NodeList(*this) += inNode;
}

NodeList Node::operator + (const NodeList &inNodeList) const {
    return NodeList(*this) += inNodeList;
}

NodeType Node::getNodeType() const {
    return getData().getNodeType();
}
        
std::string Node::getXMLValue() const {
    std::ostringstream lStream;
    lStream << *this;
    return lStream.str();
}

Node::Data &Node::getData() {
    // If only one node hold this data (that means it's this node), then no
    // need to make a copy. Just force the constant data to be editable.
    if ( mData->getRetainCount() == 1 )
        return const_cast<Node::Data &>(*mData);
    
    // Else make a copy to insure the data is preserved for other nodes.
    Data *lData = mData->clone();
    mData->release();
    mData = lData;
    return *lData;
}


// Node::Data

Node::Data::Data(NodeType inType) : mRetainCount(0), mNodeType(inType) { }
Node::Data::~Data() { }

void Node::Data::retain() const { mRetainCount++; }
void Node::Data::release() const {
    if ( --mRetainCount == 0 ) delete this;
}

const Node::Data *Node::Data::replaceWith(const Node::Data *inData) const {
    inData->retain();
    this->release();
    return inData;
}


// NodeList

NodeList::NodeList() { }
NodeList::NodeList(const Node &inNode) { 
    *this += inNode;
}

NodeList &NodeList::operator += (const NodeList &inNodeList) {
    insert(end(), inNodeList.begin(), inNodeList.end());
    return *this;
}

NodeList NodeList::operator + (const NodeList &inNodeList) const {
    return NodeList(*this) += inNodeList;
}

NodeList &NodeList::operator += (const Node &inNode) {
    push_back(inNode);
    return *this;
}

NodeList NodeList::operator + (const Node &inNode) const {
    return NodeList(*this) += inNode;
}

Element *NodeList::getElementById(const String &inIdentifier) {
    for ( unsigned i = 0; i < size(); i++ )
    	if ( (*this)[i].getNodeType() == Element::cNodeType ) {
            Element &lElement = static_cast<Element &>((*this)[i]);
            String lIdentifier = lElement.getIdentifier();
            if ( lIdentifier == inIdentifier && lIdentifier.length() > 0 )
                return &lElement;
            else {
                Element *lChild = lElement.getChild(inIdentifier);
                if ( lChild != 0 ) return lChild;
            }
        }
    return 0;
}
const Element *NodeList::getElementById(const String &inIdentifier) const {
    for ( unsigned i = 0; i < size(); i++ )
    	if ( (*this)[i].getNodeType() == Element::cNodeType ) {
            const Element &lElement = static_cast<const Element &>((*this)[i]);
            String lIdentifier = lElement.getIdentifier();
            if ( lIdentifier == inIdentifier && lIdentifier.length() > 0 )
                return &lElement;
            else {
                const Element *lChild = lElement.getChild(inIdentifier);
                if ( lChild != 0 ) return lChild;
            }
        }
    return 0;
}
