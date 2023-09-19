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
 * \file PACC/XML/Document.cpp
 * \brief Class methods for the %XML document.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.2 $
 * $Date: 2004/08/10 01:41:08 $
 */

#include "XML/Document.hpp"
#include "XML/Streamer.hpp"
#include <stdexcept>
#include <fstream>

using namespace std;
using namespace PACC;

//! Assign document \c inDocument to this document: make deep copy.
PACC::XML::Document& PACC::XML::Document::operator=(const PACC::XML::Document& inDocument) 
{
   // do not self assign!
   if(&inDocument == this) return *this;
   // delete current document roots
   eraseRoots();
   // assign all child roots of argument
   for(ConstIterator lNode = inDocument.getFirstRoot(); lNode; ++lNode)
   {
      // allocate node
      Node* lChild = new Node;
      *lChild = *lNode;
      mRoots.push_back(lChild);
   }
   // assign "no parse tag" set
   mNoParseTags = inDocument.mNoParseTags;
   return *this;
}

/*! \brief Add a new child to the node referenced by iterator \c inPos.

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node as the last child of the referenced node. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add child tag "Mytag" to the node referenced by iterator \c lPos in document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addChild(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
PACC::XML::Iterator PACC::XML::Document::addChild(const PACC::XML::Iterator& inPos, const string& inValue, NodeType inType)
{
   return attachChild(inPos, new Node(inValue, inType));
}

/*! \brief Add a new root node in this document.

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node as the last root element of the document. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add root tag "Myroot" to document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addRoot(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
PACC::XML::Iterator PACC::XML::Document::addRoot(const string& inValue, PACC::XML::NodeType inType)
{
   // allocate new node
   Node* lNode = new Node(inValue, inType);
   // insert at end of root list
   mRoots.push_back(lNode);
   return Iterator(&mRoots, --(mRoots.end()));
}

/*! \brief Add a new sibling in front of the node referenced by iterator \c inPos.

This method first allocates a new node of type \c inType and value \c inValue, and inserts this node in front of the referenced node. It then returns an iterator to the newly allocated node, which can be used to set tag attributes. For example, to add sibling tag "Mytag" to the node referenced by iterator \c lPos in document \c lDocument:
\code
Iterator lPos;
...
Iterator lChild = lDocument.addSibling(lPos, "Mytag");
lChild->setAttribute("attribute", "value");
...
\endcode
*/
PACC::XML::Iterator PACC::XML::Document::addSibling(const PACC::XML::Iterator& inPos, const string& inValue, PACC::XML::NodeType inType)
{
   return attachSibling(inPos, new Node(inValue, inType));
}

/*! \brief Attach node \c inNode as a child of the node referenced by iterator \c inPos.

This method inserts the pre-allocated node \c inNode (and its sub-tree) as the last child of the referenced node. The node pointed by \c inNode then becomes property of this document. 

WARNING: a document sub-tree needs to be detached (see Document::detach) before it can be move to another document or another branch of the same document (node are not reference counted). Otherwise, the subtree will be owned twiced, and deallocated by the first owner which is deleted. This can produce strange behaviors that are very difficult to debug. Beware!
*/
PACC::XML::Iterator PACC::XML::Document::attachChild(const PACC::XML::Iterator& inPos, PACC::XML::Node* inNode)
{
   if(!inNode) throw runtime_error("<Document::attachChild> cannot attach nul pointer!");
   // insert at the end of child list of the referenced node
   inPos->mChilds.push_back(inNode);
   return Iterator(&(inPos->mChilds), --(inPos->mChilds.end()));
}

/*! \brief Attach node \c inNode as a sibling in front of the node referenced by iterator \c inPos.

This method inserts the pre-allocated node \c inNode (and its sub-tree) as the sibling that precedes the referenced node. The node pointed by \c inNode then becomes property of this document. 

WARNING: a document sub-tree needs to be detached (see Document::detach) before it can be move to another document or another branch of the same document (nodes are not reference counted). Otherwise, the subtree will be owned twiced, and deallocated by the first owner which is deleted. This can produce strange behaviors that are very difficult to debug. Beware!
*/
PACC::XML::Iterator PACC::XML::Document::attachSibling(const PACC::XML::Iterator& inPos, PACC::XML::Node* inNode)
{
   if(!inNode) throw runtime_error("<Document::attachSibling>  cannot attach nul pointer!");
   // insert in front of the referenced node
   return Iterator(inPos.mList, inPos.mList->insert(inPos.mPos, inNode));
}

/*! \brief Detach the sub-tree rooted at the node referenced by iterator \c inPos.

Tree nodes should never be owned by multiple document objects because no reference counting is implemented. This method can be used to remove node ownership from a document so that the corresponding sub-tree can be moved from one document to the other. A detached sub-tree (node) can be re-attached to a document using methods Document::attachChild or Document::attachSibling.
*/
PACC::XML::Node* PACC::XML::Document::detach(const PACC::XML::Iterator& inPos)
{
   Node* lNode = &(*inPos);
   // erase corresponding element in child list
   inPos.mList->erase(inPos.mPos);
   // return node pointer
   return lNode;
}

/*! \brief Delete the node referenced by iterator \c inPos.

Upon return, iterator \c inPos is invalidated (should not be incremented, decremented, nor deferenced). For example, given document \c lDocument, the following code that iterates through the childs of the first root data tag is WRONG:
\code
...
Iterator lRoot = lDocument.getFirstDataTag();
// process all childs of root data tag.
for(Iterator lChild = lRoot->getFirstChild(); lChild; ++lChild) 
{
   // determine if this child should be erased
   ...
   if(should_erase) lDocument.erase(lChild);
   // lChild is now invalid!
}
\endcode
The problem is that iterator \c lChild is invalidated by the call to the erase method. It now points to a deallocated block of memory. When the for-statement proceeds with operation \c ++lChild, the result will depend on the information present at the location in memory where the erased node resided. The exact behavior of this operation is dependent on the implementation of STL. It could work most of the time on some platform, and then fail in mysterious circumstances, especially in multithreaded applications. It should definitely never be used!

One way to circumvent this restriction is to use the postfix version of the decrement operator in the method call. For example, the following code is ok:
\code
...
Iterator lRoot = lDocument.getFirstDataTag();
// process all childs of root data tag.
for(Iterator lChild = lRoot->getFirstChild(); lChild; ++lChild) 
{
   // process node information in order to determine if it should be erased
   ...
   if(should_erase) lDocument.erase(lChild--);
}
\endcode
In this example, the iterator passed to the erase method is the one that points to the correct node, but iterator \c lChild is decremented prior to the call. It thus points on the previous sibling node when the erase operation is executed. In this way, the \c ++lChild operation of the for-statement will proceed with the sibling that followed the one that was erased.

An alternative code snippet could also be:
\code
...
Iterator lChild = lRoot.getFirstChild();
while(lChild) 
{
   // process node information in order to determine if it should be erased
   ...
   if(should_erase) lDocument.erase(lChild++);
   else ++lChild;
}
\endcode
The two approaches are equivalent. 
*/
void PACC::XML::Document::erase(const PACC::XML::Iterator& inPos)
{
   // delete node
   delete &(*inPos);
   // erase corresponding element in child list
   inPos.mList->erase(inPos.mPos);
}

//! Deallocate all root elements of this document.
void PACC::XML::Document::eraseRoots(void) 
{
   // delete root nodes
   Iterator lNode = getFirstRoot();
   while(lNode) delete &(*(lNode++));
   // clear list of root node pointers
   mRoots.clear();
}

/*! \brief Return an iterator on the first data tag of this document.

An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first data tag amongs the root elements of the document.
*/
PACC::XML::Iterator PACC::XML::Document::getFirstDataTag(void)
{
   for(Iterator lTag = getFirstRoot(); lTag; ++lTag) if(lTag->getType() == eData) return lTag;
   return Iterator();
}

/*! \brief Return a const iterator on the first data tag of this document.

An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns a const iterator on the first data tag amongs the root elements of the document.
*/
PACC::XML::ConstIterator PACC::XML::Document::getFirstDataTag(void) const
{
   for(ConstIterator lTag = getFirstRoot(); lTag; ++lTag) if(lTag->getType() == eData) return lTag;
   return ConstIterator();
}

/*! \brief Return an iterator on the first root element of this document.

An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns an iterator on the first element encountered in the document (usually the <?xml version="1.0"?> tag).
*/
PACC::XML::Iterator PACC::XML::Document::getFirstRoot(void)
{
   return Iterator(&mRoots, mRoots.begin());
}

/*! \brief Return a const iterator on the first root element of this document.

An %XML document may contain three types of elements: declarative tags, special tags, and data tags. This method returns a const iterator on the first element encountered in the document (usually the <?xml version="1.0"?> tag).
*/
PACC::XML::ConstIterator PACC::XML::Document::getFirstRoot(void) const
{
   return ConstIterator(&mRoots, mRoots.begin());
}

//! Parse a document contained in the file named \c inFileName; 
void PACC::XML::Document::parse(const string& inFileName)
{
   // open file
   ifstream lStream(inFileName.c_str());
   if(!lStream.good()) throw runtime_error(string("<Document::parse> unable to open file ")+inFileName);
   // parse file content
   parse(lStream, inFileName);
}

//! Parse a document from input stream \c inStream, using string \c inName as stream name for error messages.
void PACC::XML::Document::parse(istream& inStream, const string& inName)
{
   Tokenizer lTokenizer(inStream);
   lTokenizer.setStreamName(inName);
   eraseRoots();
   Node* lNode = 0;
   // parse all elements
   while((lNode = Node::parse(lTokenizer, mNoParseTags))) mRoots.push_back(lNode);
   if(!inStream.eof()) lNode->throwError(lTokenizer, "invalid xml tag");
}

//! Serialize document into an output stream \c outStream, using indentation of \c inWidth characters.
void PACC::XML::Document::serialize(ostream& outStream, unsigned int inWidth) const
{
   Streamer lStream(outStream, inWidth);
   // retrieve first root
   ConstIterator lRoot = getFirstRoot();
   if(lRoot)
   {
      // check for xml header
      if(lRoot->getType() != eDecl) lStream.insertHeader();
      // serailize all roots
      while(lRoot) (lRoot++)->serialize(lStream, true);
   }
}

//! Add \c inTag to the list of tag names for which content should not be parsed for this document. 
void PACC::XML::Document::setNoParse(const string& inTag)
{
   mNoParseTags.insert(inTag);
}

//! Remove \c inTag from the list of tags for which content should not be parsed for this document. 
void PACC::XML::Document::unsetNoParse(const string& inTag)
{
   mNoParseTags.erase(inTag);
}
