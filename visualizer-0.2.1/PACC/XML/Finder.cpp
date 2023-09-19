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
 * \file PACC/XML/Finder.cpp
 * \brief Class methods for the %XML tag finder.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#include "XML/Finder.hpp"

using namespace std;
using namespace PACC;

/*! \brief Return the first instance of data tag \c inTag in the sub-tree rooted by this finder.

The search always starts with the root of the sub-tree and proceeds downward in level order, by first searching within the childs of the tree root, from first to last, before proceeding recursively with the childs of the childs (i.e. first the childs, then the grandchilds, then the grand-grandchilds, etc.). Only the regular data tag are considered in this search. For example, to search the first root tag of document \c lDocument, looking for a data tag named "Mytag":
\code
Finder lFinder(lDocument.getFirstDataTag())
// find the first instance
Iterator lTag = lFinder.find("Mytag");
if(lTag)
{
   // process this instance of data tag "Mytag"
   ...
}
\endcode
After processing a found tag, the search may be continued for the next tag using method Finder::findNext. 
*/
PACC::XML::Iterator PACC::XML::Finder::find(const string& inTag)
{
   // clear the search queue
   while(!mSearchQueue.empty()) mSearchQueue.pop();
   // push the root node onto the search queue 
   mSearchQueue.push(*this);
   // return first instance of requested tag
   mTag = inTag;
   return findNext();
}

/*! \brief Return the next instance of data tag \c inTag in the sub-tree rooted by this finder.

The search proceeds in level order (see Document::findTag), from the point where the last tag was found. Here is an example where we search for all instances of tag "Mytag" in the sub-tree rooted by the first data tag of document \c lDocument:
\code
Finder lFinder(lDocument.getFirstDataTag())
// find all instances of tag
for(Iterator lTag = lDocument.find("Mytag"); lTag; lTag = lDocument.findNext())
{
   // process each instance
   ...
}
\endcode
*/
PACC::XML::Iterator PACC::XML::Finder::findNext(void)
{
   // search in level order (in the order of the tree levels)
   while(!mSearchQueue.empty())
   {
      // get next node
      Iterator lNode = mSearchQueue.front();
      mSearchQueue.pop();
      // push all its children onto the search queue
      for(Iterator lChild = lNode->getFirstChild(); lChild; ++lChild) mSearchQueue.push(lChild);
      // return node if it matches the requested tag
      if(lNode->getType() == eData && lNode->getValue() == mTag) return lNode;
   }
   // return null iterator if not found
   return Iterator();
}


/*! \brief Return the first instance of data tag \c inTag in the sub-tree rooted by this finder.

The search always starts with the root of the sub-tree and proceeds downward in level order, by first searching within the childs of the tree root, from first to last, before proceeding recursively with the childs of the childs (i.e. first the childs, then the grandchilds, then the grand-grandchilds, etc.). Only the regular data tag are considered in this search. For example, to search the first root tag of document \c lDocument, looking for a data tag named "Mytag":
\code
Finder lFinder(lDocument.getFirstDataTag())
// find the first instance
Iterator lTag = lFinder.find("Mytag");
if(lTag)
{
   // process this instance of data tag "Mytag"
   ...
}
\endcode
After processing a found tag, the search may be continued for the next tag using method Finder::findNext 
*/
PACC::XML::ConstIterator PACC::XML::ConstFinder::find(const string& inTag)
{
   // clear the search queue
   while(!mSearchQueue.empty()) mSearchQueue.pop();
   // push the root node onto the search queue 
   mSearchQueue.push(*this);
   // return first instance of requested tag
   mTag = inTag;
   return findNext();
}

/*! \brief Return the next instance of data tag \c inTag in the sub-tree rooted by this finder.

The search proceeds in level order (see Document::findTag), from the point where the last tag was found. Here is an example where we search for all instances of tag "Mytag" in the sub-tree rooted by the first data tag of document \c lDocument:
\code
Finder lFinder(lDocument.getFirstDataTag())
// find all instances of tag
for(Iterator lTag = lDocument.find("Mytag"); lTag; lTag = lDocument.findNext())
{
   // process each instance
   ...
}
\endcode
*/
PACC::XML::ConstIterator PACC::XML::ConstFinder::findNext(void)
{
   // search in level order (in the order of the tree levels)
   while(!mSearchQueue.empty())
   {
      // get next node
      ConstIterator lNode = mSearchQueue.front();
      mSearchQueue.pop();
      // push all its children onto the search queue
      for(ConstIterator lChild = lNode->getFirstChild(); lChild; ++lChild) mSearchQueue.push(lChild);
      // return node if it matches the requested tag
      if(lNode->getType() == eData && lNode->getValue() == mTag) return lNode;
   }
   // return null iterator if not found
   return ConstIterator();
}
