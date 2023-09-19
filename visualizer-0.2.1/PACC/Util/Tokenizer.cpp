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
 * \file PACC/Util/Tokenizer.cpp
 * \brief Class methods for the input stream tokenizer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#include "Util/Tokenizer.hpp"
#include <stdexcept>

using namespace std;
using namespace PACC;

/*! \brief Parse next token of default input stream.
\return	String of next token found.

This function finds the next token in the input stream, after removing any leading white space. An empty token (string) means that end of stream was reached. Throws an std::runtime_error exception if no stream is defined.
*/
string PACC::Tokenizer::getNextToken(void)
{
   string lToken;
   if(mStream == 0) throw runtime_error("<Tokenizer::getNextToken> input stream undefined");
   getNextToken(lToken);
   return lToken;
}

/*! \brief Parse next token of default input stream.
 \return True if a valid token was found, false otherwise.

 This function finds the next token in the default input stream, after removing any leading white space. The token is returned through argument \c outToken. An empty token (string) means that end of stream was reached. Throws a std::runtime_error exception if no default stream is defined.
 */
bool PACC::Tokenizer::getNextToken(string& outToken)
{
   if(mStream == 0) throw runtime_error("<Tokenizer::getNextToken> input stream undefined");
   char lChar;
   outToken.erase(outToken.begin(), outToken.end());
   // get rid of leading white space
   while(mStream->get(lChar) && mWhiteSpace[(unsigned) lChar]) if(lChar == '\n') ++mLine;
   // get token; if eof, return empty string
   if(mStream->good())
   {
      // not eof
      if(lChar == '\n') ++mLine;
      outToken += lChar;
      if(!mSingleCharTokens[(unsigned) lChar])
      {
         // not a single char token nor string token
         while(mStream->get(lChar))
         {
            // append until next white space or single char token
            if(mWhiteSpace[(unsigned) lChar] || mSingleCharTokens[(unsigned) lChar])
            {
               mStream->putback(lChar);
               break;
            } else if(lChar == '\n') ++mLine;
            outToken += lChar;
         }
      }
   }
   return !outToken.empty();
}

//! Return single character delimiters.
string PACC::Tokenizer::getSingleCharTokens(void) const 
{
   string lSingleCharTokens;
   for(unsigned int i=0; i < 256; ++i) if(mSingleCharTokens[i]) lSingleCharTokens += (char) i;
   return lSingleCharTokens;
}

//! Return white space delimiters.
string PACC::Tokenizer::getWhiteSpace(void) const 
{
   string lWhiteSpace;
   for(unsigned int i=0; i < 256; ++i) if(mWhiteSpace[i]) lWhiteSpace += (char) i;
   return lWhiteSpace;
}

//! Set single character delimiters to string \c inTokens.
void PACC::Tokenizer::setSingleCharTokens(const string &inTokens) 
{
   memset(mSingleCharTokens, 0, sizeof(mSingleCharTokens));
   for(string::const_iterator i = inTokens.begin(); i != inTokens.end(); ++i) 
   {
      mSingleCharTokens[(unsigned)*i] = true;
   }
}

//! Set white space delimiters to string \c inWhite.
void PACC::Tokenizer::setWhiteSpace(const string &inWhite) 
{
   memset(mWhiteSpace, 0, sizeof(mWhiteSpace));
   for(string::const_iterator i =inWhite.begin(); i != inWhite.end(); ++i) 
   {
      mWhiteSpace[(unsigned)*i] = true;
   }
}
