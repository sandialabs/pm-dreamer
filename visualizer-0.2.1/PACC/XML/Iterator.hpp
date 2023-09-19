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
 * \file PACC/XML/Iterator.hpp
 * \brief Class definition for the %XML node iterator.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_XML_Iterator_hpp_
#define PACC_XML_Iterator_hpp_

#include <list>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {
   
   // forward declarations
   class Node;
   
   /*! \brief %Node iterator.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
   
   A node iterator is used to iterate through child nodes, using an interface similar to STL bidirectional iterators. For example, to iterate through the childs of node \c lNode:
   \code
   Node lNode;
   ...
   for(Iterator lChild = lNode.getFirstChild(); lChild; ++lChild) 
   {
      // process each child
      ...
   }
   \endcode
   Operator* can be used to return a node reference, operator-> to return a node pointer, operator++ (both prefix and postfix) to skip to next element, and operator-- (both prefix and postfix) to skip to previous element. 
      
   A boolean casting operator is also available to test for end of sequence (see second term of for-statement above). An iterator pointing to a valid node always returns true while an invalid iterator returns false. An invalid iterator should never be dereferenced, incremented, or decremented. WARNING: no error checking is currently implemented.
   */
   class Iterator
   {
    public:
      //! Construct an empty iterator.
      Iterator(void) : mList(0) {}
      //! Construct an iterator for list \c inList using stl iterator \c inPos.
      Iterator(list<Node*>* inList, const list<Node*>::iterator& inPos) : mList(inList), mPos(inPos) {}
      
      //! Iterate to next sibling of current node (prefix version); return iterator pointing to next sibling.
      Iterator& operator++(void) {++mPos; return *this;}
      //! Iterate to next sibling of current node (postfix version); return iterator pointing to current node.
      Iterator operator++(int) {Iterator lTmp = *this; ++mPos; return lTmp;}
      //! Iterate to previous sibling of current node (prefix version); return iterator pointing to previous sibling.
      Iterator& operator--(void) {--mPos; return *this;}
      //! Iterate to previous sibling of current node (postfix version); return iterator pointing to current node.
      Iterator operator--(int) {Iterator lTmp = *this; --mPos; return lTmp;}
      //! Return reference to current node (const version).
      Node& operator*(void) const {return *(*mPos);}
      //! Return pointer to current node (const version).
      Node* operator->(void) const {return *mPos;}
      //! Return true if this iterator is the same as iterator \c inIter; false otherwise.
      bool operator==(const Iterator& inIter) const {return mList == inIter.mList && mPos == inIter.mPos;}
      //! Return true if this iterator is different from iterator \c inIter; false otherwise.
      bool operator!=(const Iterator& inIter) const {return mList != inIter.mList || mPos != inIter.mPos;}
      //! Cast Iterator into bool; return true for a valid iterator, false otherwise.
      operator bool(void) const {return mList != 0 && mPos != mList->end();}
      
    private:
      list<Node*>* mList; //!< List of childs.
      list<Node*>::iterator mPos; //!< position of current child.
      
      friend class Document;      
   };
   
   /*! \brief %Node const iterator.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
      
   A const node iterator is used to iterate through child nodes, using an interface similar to STL bidirectional const_iterators. For example, to iterate through the childs of node \c lNode:
   \code
   Node lNode;
   ...
   for(ConstIterator lChild = lNode.getFirstChild(); lChild; ++lChild)
   {
      // process each child
      ...
   }
   \endcode
   Operator* can be used to return a node const reference, operator-> to return a node const pointer, operator++ (both prefix and postfix) to skip to next element, and operator-- (both prefix and postfix) to skip to next element. A boolean casting operator is also available to test for end of sequence (see second term of for-statement above).
   */
   class ConstIterator
   {
    public:
      //! Construct empty ierator.
      ConstIterator(void) : mList(0) {}
      //! Construct an iterator for node \c inNode using stl iterator \c inPos.
      ConstIterator(const list<Node*>* inList, const list<Node*>::const_iterator& inPos) : mList(const_cast<list<Node*>*>(inList)), mPos(inPos) {}
      
      //! Iterate to next sibling of current node (prefix version); return iterator pointing to next sibling.
      ConstIterator& operator++(void) {++mPos; return *this;}
      //! Iterate to next sibling of current node (postfix version); return iterator pointing to current node.
      ConstIterator operator++(int) {ConstIterator lTmp = *this; ++mPos; return lTmp;}
      //! Iterate to previous sibling of current node (prefix version); return iterator pointing to previous sibling node.
      ConstIterator& operator--(void) {--mPos; return *this;}
      //! Iterate to previous sibling of current node (postfix version); return iterator pointing to current node.
      ConstIterator operator--(int) {ConstIterator lTmp = *this; --mPos; return lTmp;}
      //! Return reference to current node (const version).
      const Node& operator*(void) const {return *(*mPos);}
      //! Return pointer to current node (const version).
      const Node* operator->(void) const {return *mPos;}
      //! Return true if this iterator is the same as iterator \c inIter; false otherwise.
      bool operator==(const ConstIterator& inIter) const {return mList == inIter.mList && mPos == inIter.mPos;}
      //! Return true if this iterator is different from iterator \c inIter; false otherwise.
      bool operator!=(const ConstIterator& inIter) const {return mList != inIter.mList || mPos != inIter.mPos;}
      //! Cast Iterator into bool; return true for a valid iterator, false otherwise.
      operator bool(void) const {return mList != 0 && mPos != mList->end();}
      
    private:
      list<Node*>* mList; //!< List of childs.
      list<Node*>::const_iterator mPos; //!< position of current child.
      
      friend class Document;      
   };

} // end of XML namespace 

} // end of PACC namespace

#endif // PACC_XML_Iterator_hpp_
