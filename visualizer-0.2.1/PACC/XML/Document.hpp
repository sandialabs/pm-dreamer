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
 * \file PACC/XML/Document.hpp
 * \brief Class definition for the %XML document.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_XML_Document_hpp_
#define PACC_XML_Document_hpp_

#include "XML/Node.hpp"
#include <queue>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {
   
   /*! \brief %Document container and parser.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
   
   This class is for parsing and manipulating xml documents. It respects the basic %XML syntax, except for the following items:
   -# It does not deal with character encodings;
   -# It does not support DTD;
   -# Tag attributes must be enclosed by double quotes;
   .
   In other words, it can read most data coded in %XML, but cannot validate its syntax, nor translate its character encodings (this translation must be done by the application).
   
   The document is internaly represented as a list (forest) of m-ary rooted trees. These trees can represent different type of elements:
   -# Declarative tags of the form "<? ... ?>" (XML::eDecl)
   -# Special tags of the form "<! ... >" (XML::eSpecial)
   -# Regular data tags (XML::eData)
   -# Simple strings (XML::eString)
   . 
   Tree nodes are derived from a map of attribute/value string pairs and contain a list of child node pointers. An Iterator class is defined in order to iterate through these nodes. Iterators wrap pointers to node elements of the document tree, with an interface similar to STL bidirectional iterators. For example:
   \code
   Document lDocument;
   lDocument.parse("Myfile.xml");
   Iterator lTag;
   // search the root elements of the document for the first data tag
   for(lTag = lDocument.getFirstRoot(); lTag; ++lTag) if(lTag->getType() != eData) break;
   ...
   \endcode
   can be used to iterate over the root elements of document \c lDocument in order to locate its first root data tag. The same result can be achieved using helper method Document::getFirstDataTag. The type of a node can be retrieved using method Node::getType.

   A data tag can contain embedded child tags which can be iterated over just like any other node. For instance, to iterate over the childs of the root data tag of document \c lDocument:
   \code
   Iterator lRoot = lDocument.getFirstDataTag();
   for(Iterator lChild = lRoot->getFirstChild(); lChild; ++lChild) 
   {
      // process each child
      ...
   }
   \endcode

   A document can also be constructed from scratch, or modified, using methods Document::addChild, Document::addRoot, Document::addSibling, Document::attachChild, Document::attachSibling, Document::detach, Document::erase, and Document::eraseRoots. It can be serialized into a stream using method Document::serialize, and parsed from a stream using Document::parse. To search for a given tag name over the nodes of a specific data element, one may use an instance of the Finder class.
   */
   class Document
   {
    public:   
      //! Constructs an empty document.
      Document(void) {}
      //! Copy constructs a document.
      Document(const Document& inDocument) {*this = inDocument;}      
      //! Delete document.
      ~Document(void) {eraseRoots();}
      
      Document& operator=(const Document& inDocument);
      
      Iterator addChild(const Iterator& inPos, const string& inValue, NodeType inType=eData);
      Iterator addRoot(const string& inTag, NodeType inType=eData);
      Iterator addSibling(const Iterator& inPos, const string& inValue, NodeType inType=eData);
      Iterator attachChild(const Iterator& inPos, Node* inNode);
      Iterator attachSibling(const Iterator& inPos, Node* inNode);
      Node* detach(const Iterator& inPos);
      void erase(const Iterator& inPos);
      void eraseRoots(void);
      Iterator getFirstDataTag(void);
      ConstIterator getFirstDataTag(void) const;
      Iterator getFirstRoot(void);
      ConstIterator getFirstRoot(void) const;
      void parse(const string& inFileName);
      void parse(istream& inStream, const string& inName="");
      void serialize(ostream& outStream, unsigned int inWidth=2) const;
      void setNoParse(const string& inTag);
      void unsetNoParse(const string& inTag);
   
    protected:
      list<Node*> mRoots; //!< List of root elements
      set<string> mNoParseTags; //!< Tag names for which content should not be parsed

   };

} // end of XML namespace

} // end of PACC namespace

#endif // PACC_XML_Document_hpp_
