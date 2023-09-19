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
 * \file PACC/XML/Streamer.cpp
 * \brief Class methods for the %XML streamer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval

 * $Revision: 1.2 $
 * $Date: 2004/08/11 19:31:55 $
 */

#include "XML/Streamer.hpp"
#include <stdexcept>

using namespace std;
using namespace PACC;

//! Close the last opened tag. Indents end tag if \c inIndent is true (default).
void PACC::XML::Streamer::closeTag(bool inIndent)
{
   if(mTags.empty()) throw runtime_error("<Writer::closeTag> No tag to close!");
   if(!mClosed) {
     mStream << "/>";
   }
   else
   {
      // indentation depends on the number of tags currently on the stack.
      if(inIndent) mStream << endl;
      unsigned int lIndent = mWidth*(mTags.size()-1);
      if(inIndent && lIndent) mStream << string(lIndent, ' ');
      mStream << "</" << mTags.top() << ">";
   }
   mClosed = true;
   mTags.pop();
}

//! Return output stream of streamer.
ostream& PACC::XML::Streamer::getStream(void)
{
   return mStream;
}

//! Insert an attribute with name \c inName and float value \c inValue into current start tag.
void PACC::XML::Streamer::insertAttribute(const string& inName, double inValue)
{
   mStream << " " << inName << "=\"" << inValue << "\"";
}

//! Insert an attribute with name \c inName and string value \c inValue into current start tag.
void PACC::XML::Streamer::insertAttribute(const string& inName, string inValue)
{
   mStream << " " << inName << "=\"" << inValue << "\"";
}

/*! \brief Insert a valid xml declarative tag with encoding \c inEncoding (default is ISO-8859-1).

The header tag as the form <?xml version="1.0" encoding="ISO-8859-1"?>.
*/
void PACC::XML::Streamer::insertHeader(const string& inEncoding)
{
   mStream << "<?xml version=\"1.0\"";
   if(!inEncoding.empty()) mStream << " encoding=\"" << inEncoding << "\"";
   mStream << "?>" << endl;
}

/*! \brief Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and floating point value \c inValue.

If tag name is omitted, default name "Float" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void PACC::XML::Streamer::insertPrimitiveTag(double inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag(false);
}

/*! \brief Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and integer value \c inValue.

If tag name is omitted, default name "Integer" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void PACC::XML::Streamer::insertPrimitiveTag(int inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag(false);
}

/*! \brief Insert a primitive tag with tag name \c inTagName, attribute name \c inAttName, and string value \c inValue.

If tag name is omitted, default name "String" is used. If attribute name is omitted, default name "v" is used. The format for this primitive tag is thus <inTagName inAttName="inValue"/>.
*/
void PACC::XML::Streamer::insertPrimitiveTag(const string& inValue, const string& inTagName, const string& inAttName)
{
   openTag(inTagName);
   insertAttribute(inAttName, inValue);
   closeTag(false);
}

//! Insert special tag \c inTag.
void PACC::XML::Streamer::insertSpecialTag(const string& inTag)
{
   if(!mClosed) {
      mStream << ">";
      mClosed = true;
   }
   // indentation depends on the number of tags currently on the stack.
   unsigned int lIndent = mWidth*mTags.size();
   if(lIndent) mStream << endl << string(lIndent, ' ');
   mStream << "<" << inTag << ">" << endl;
}

//! Insert string \c inContent as content of current tag.
void PACC::XML::Streamer::insertStringContent(const string& inContent)
{
   if(!mClosed) {
      mStream << ">";
      mClosed = true;
   }
   mStream << inContent;
}

//! Open new start tag using tag name \c inName, and increase indentation level.
void PACC::XML::Streamer::openTag(const string& inName, bool inIndent)
{
   if(!mClosed)
   {
      mStream << ">";
      mClosed = true;
   }
   // indentation depends on the number of tags currently on the stack.
   unsigned int lIndent = mWidth*mTags.size();
   if(inIndent && lIndent) mStream << endl << string(lIndent, ' ');
   mStream << "<" << inName;
   mTags.push(inName);
   mClosed = false;
}

//! Insert a float tag with value \c inValue into this streamer.
PACC::XML::Streamer& PACC::XML::Streamer::operator<<(double inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}

//! Insert an integer tag with value \c inValue into this streamer.
PACC::XML::Streamer& PACC::XML::Streamer::operator<<(int inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}

//! Insert a string tag with value \c inValue into this streamer.
PACC::XML::Streamer& PACC::XML::Streamer::operator<<(const string& inValue)
{
   insertPrimitiveTag(inValue);
   return *this;
}
