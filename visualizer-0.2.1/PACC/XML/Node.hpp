/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2003 by Marc Parizeau
 *  http://manitou.gel.ulaval.ca/~parizeau/PACC
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 * \file PACC/XML/Node.hpp
 * \brief Class definition for the %XML parse tree node.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_XML_Node_hpp_
#define PACC_XML_Node_hpp_

#include "Util/Tokenizer.hpp"
#include "XML/Attribute.hpp"
#include "XML/Iterator.hpp"
#include <map>
#include <set>
#include <string>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {
   
   /*! \brief Types of parse tree node.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
   */
   enum NodeType {
      eDecl, //!< Declarative tag ("<? ... ?>").
      eSpecial, //!< Special tag ("<! ... >").
      eData, //!< Regular data tag.
      eString //!< Simple string.
   };
   
   // forward declarations
   class Streamer;
   
   /*! \brief %Node of the document tree.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML

   A tree node can represent either %XML data tags or simple string content. A data tag has a name which can be retrieved and set using methods Node::getValue and Node::setValue, respectivelly. A node is also derived from a map of attribute name/value pairs that can be fetched and set using methods Node::getAttribute and Node::setAttribute. Tag content, including any embedded tags, is represented by child nodes. A Node can parse itself from a stream Tokenizer. The n-ary sub-tree structure is constructed using an STL list of node pointers. Any parse error throws an runtime_error exception. Method Node::getFirstChild is used to retrieve an Iterator on the first child of this node. Finally, a node can serialize itself into an %XML Streamer.
   */
   class Node : public AttributeList {

public:
      //! Construct empty node of default type eData.
      Node(void) : mType(eData) {}
      //! Construct node of type \c inType (default NodeType::eTag) with value \c inValue.
      Node(const string& inValue, NodeType inType=eData) : mType(inType) {(*this)[""] = inValue;}
      //! Construct node of type \c edata with value \c inValue and attribute list \c inAttrList.
      Node(const string& inValue, const AttributeList& inAttrList) : AttributeList(inAttrList) {(*this)[""] = inValue;}
      //! Copy constructor: make deep copy of node \c inNode.
      Node(const Node& inNode) {*this = inNode;}
      //! Delete node.
      ~Node(void) {eraseChilds();}
      
      Node& operator=(const Node&);
      
      void eraseChilds(void);

      //! Return number of child nodes.
      unsigned int getChildCount(void) const {return (unsigned int)mChilds.size();}
      //! Return an iterator pointing to the first child of this node.
      Iterator getFirstChild(void) {return Iterator(&mChilds, mChilds.begin());}
      //! Return const iterator pointing to the first child of this node.
      ConstIterator getFirstChild(void) const {return ConstIterator(&mChilds, mChilds.begin());}
      
      //! Returns the node type (see enum NodeType).
      const NodeType getType(void) const {return mType;}
      //! Returns the node value (tag name for nodes of type NodeType::eTag).
      const string getValue(void) const {return getAttribute("");}         
      void serialize(Streamer& outStream, bool inIndent=true) const;
      //! Set the node type (see enum NodeType).
      void setType(NodeType inType) {mType = inType;}         
      //! set node value (tag name for nodes of type NodeType::eTag).
      void setValue(const string& inValue) {setAttribute("", inValue);}         
      
    protected:
      NodeType mType; //!< Type of node.
      list<Node*> mChilds; //!< List of child node pointers.
   
      static Node* parse(Tokenizer& inTokenizer, const set<string>& inNoParseTags);
      void parseAttributeList(Tokenizer& inTokenizer, string& outToken);
      void parseContent(Tokenizer& inTokenizer, const set<string>& inNoParseTags);
      void parseStartTag(Tokenizer& inTokenizer, string& outToken);
      void readContentAsString(Tokenizer& inTokenizer);
      void throwError(Tokenizer& inTokenizer, const string& inMessage) const;
      
      friend class Document;
   };

} // end of XML namespace 

} // end of PACC namespace

#endif // PACC_XML_Node_hpp_
