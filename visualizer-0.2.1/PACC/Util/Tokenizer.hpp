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
* \file PACC/Util/Tokenizer.hpp
 * \brief Class definition for the input stream tokenizer.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2004/08/08 01:26:27 $
 */

#ifndef PACC_Tokenizer_hpp_
#define PACC_Tokenizer_hpp_

#include <iostream>
#include <string>

namespace PACC {
   
   using namespace std;

   /*! \brief Input stream tokenizer
   \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
   \ingroup Util
   \ingroup XML

   This class is for tokenizing an input stream. Tokens are either strings of characters seperated by white space, or special single-characters. By default, there are no special single-character tokens, and white space characters are spaces, tabs, cariage returns, and line-feeds. But these can be changed using the Tokenizer::setSingleCharTokens and Tokenizer::setWhiteSpace functions, respectively. For instance, if "a" and "d" are single character tokens, then string "abcd" will be tokenized in "a"+"bc" + "d".
   */
   class Tokenizer {
    public:
      //! Construct a default tokenizer with no input stream.
      Tokenizer(void) : mLine(1), mStream(0) {setWhiteSpace(" \t\n\r"); setSingleCharTokens("");}
      //! Construct a tokenizer with default input stream \c inStream.
      Tokenizer(istream& inStream) : mLine(1), mStream(&inStream) {setWhiteSpace(" \t\n\r"); setSingleCharTokens("");}

      string getNextToken(void);
      bool getNextToken(string& outToken);

      //! Return current line number.
      unsigned int getLineNumber(void) {return mLine;}
      //! Return pointer to the default input stream.
      istream* getStreamPtr(void) {return mStream;}
      //! Return name of default input stream.
      const string& getStreamName(void) const {return mName;}
      string getSingleCharTokens(void) const;
      string getWhiteSpace(void) const;
      //! Set current line number.
      void setLineNumber(unsigned int inLine) {mLine = inLine;}
      //! Set default input stream to \c inStream.
      void setStream(istream& inStream) {mStream = &inStream;}
      //! Set name of default input stream to \c inName.
      void setStreamName(const string& inName) {mName = inName;}
      void setSingleCharTokens(const string &inTokens);
      void setWhiteSpace(const string &inWhite);

    protected:
      unsigned int mLine; //!< Current line number.
      string mName; //!< Name of default input stream.
      istream* mStream; //!< Default input stream.
      char mWhiteSpace[256]; //!< White space delimiters.
      char mSingleCharTokens[256]; //!< Single character tokens.

   };

   //! Convert string \c inString to integer value (Warning: no error checking).
   inline int convertStringToInteger(const string& inString) {return atoi(inString.c_str());}
   //! Convert string \c inString to float value (Warning: no error checking).
   inline double convertStringToFloat(const string& inString) {return atof(inString.c_str());}

} // end of PACC namespace

#endif // PACC_Tokenizer_hpp_
