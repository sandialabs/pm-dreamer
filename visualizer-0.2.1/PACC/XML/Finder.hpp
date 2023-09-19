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
 * \file PACC/XML/Finder.hpp
 * \brief Class definition for the %XML tag finder.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_XML_Finder_hpp_
#define PACC_XML_Finder_hpp_

#include "XML/Node.hpp"
#include <queue>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {
   
   /*! 
   \brief %Finder for data tags.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
   
   This class is for finding data tags in a document sub-tree.
   */
   class Finder : private Iterator
   {
    public:   
      //! Construct a finder for the sub-tree rooted on \c inRoot.
      Finder(const Iterator& inRoot) : Iterator(inRoot) {}
      
      Iterator find(const string& inTag);
      Iterator findNext(void);
   
    protected:
      string mTag; //!< Last found tag name.
      queue<Iterator> mSearchQueue; //!< Search queue for level order find   
   };
   
   /*! 
   \brief Const finder for data tags.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML
      
   This class is for finding data tags in a document sub-tree.
   */
   class ConstFinder : private ConstIterator
   {
    public:   
      //! Construct a const finder for the sub-tree rooted on \c inRoot.
      ConstFinder(const ConstIterator& inRoot) : ConstIterator(inRoot) {}
      
      ConstIterator find(const string& inTag);
      ConstIterator findNext(void);
      
    protected:
      string mTag; //!< Last found tag name.
      queue<ConstIterator> mSearchQueue; //!< Search queue for level order find   
   };
   
} // end of namespace XML

} // end of namespace PACC

#endif // PACC_XML_Finder_hpp_
