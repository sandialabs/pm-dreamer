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
 * \file PACC/XML/Streamer.hpp
 * \brief Class definition for the %XML streamer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_XML_Streamer_hpp_
#define PACC_XML_Streamer_hpp_

#include <iostream>
#include <string>
#include <stack>

namespace PACC { 
   
   using namespace std;
   
   namespace XML {

   /*! \brief Simple document streamer.
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup XML

   This class is for streaming XML tags with automatic indentation.
   */
   class Streamer {
    public:
      //! Constructs object for streaming %XML tags into output stream \c inStream with indentation width \c inWidth.
      Streamer(ostream& inStream, unsigned int inWidth=2) : mStream(inStream), mWidth(inWidth), mClosed(true) {}

      void closeTag(bool inIndent=true);
      ostream& getStream(void);
      void insertAttribute(const string& inName, double inValue);
      void insertAttribute(const string& inName, string inValue);
      template<class Container>
         void insertContainer(const Container& inContainer, const string& inName="");
      void insertHeader(const string& inEncoding = "ISO-8859-1");
      void insertPrimitiveTag(double invalue, const string& inTagName="Float", const string& inAttributeName="v");
      void insertPrimitiveTag(int invalue, const string& inTagName="Integer", const string& inAttributeName="v");
      void insertPrimitiveTag(const string& invalue, const string& inTagName="String", const string& inAttributeName="v");
      void insertSpecialTag(const string& inTag);
      void insertStringContent(const string& inContent);
      void openTag(const string& inName, bool inIndent=true);
      
      Streamer& operator<<(int inValue);
      Streamer& operator<<(double inValue);
      Streamer& operator<<(const string& inValue);

      //! Insert any class \c inClass into this streamer, assuming that it defines a write function.
      template<class T> Streamer& operator<<(const T& inClass) {
         inClass.write(*this);
         return *this;
      }
      
    protected:
      ostream& mStream; //!< Output stream
      stack<string> mTags; //!< Stack of opened tags
      unsigned int mWidth; //!< Width of indentation
      bool mClosed; //!< Indicates if start tag has been closed
      
   };

   /*! \brief Inserts STL container \c inContainer using tag name \c inName.

   If no tag name is specified, the container's content is inserted into the current tag. Assumes that the container's elements can insert themselves into a streamer using operator<<.
   */
   template<class Container>
      void Streamer::insertContainer(const Container& inContainer, //!< STL container to insert.
                                     const string& inName //!< Name of tag.
                                     )
   {
      // insert optional tag name
      if(inName != "") openTag(inName);
      // iterate through container
      for(typename Container::const_iterator i = inContainer.begin(); i != inContainer.end(); ++i)
      {
         (*this) << (*i);
      }
      if(inName != "") closeTag();
   }

} // end of XML namespace

} // end of PACC namespace

#endif // PACC_XML_Streamer_hpp_
