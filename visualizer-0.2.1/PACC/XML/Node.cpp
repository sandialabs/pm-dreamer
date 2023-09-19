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
 * \file PACC/XML/Node.cpp
 * \brief Class methods for the %XML parse tree node.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.3 $
 * $Date: 2004/08/12 20:28:49 $
 */

#include "XML/Node.hpp"
#include "XML/Streamer.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace PACC;

//! Assign operator: make deep copy of the hierarchy rooted with node \c inNode.
PACC::XML::Node& PACC::XML::Node::operator=(const Node& inNode)
{
   // do not self assign!
   if(&inNode == this) return *this;
   // delete node childs
   eraseChilds();
   // assign type and attributes
   mType = inNode.mType;
   map<string,string>::operator=(inNode);
   // assign all childs
   for(ConstIterator lNode = inNode.getFirstChild(); lNode; ++lNode)
   {
      // allocate node
      Node* lChild = new Node;
      *lChild = *lNode;
      mChilds.push_back(lChild);
   }
   return *this;
}

//! Deallocate all child nodes of this document.
void PACC::XML::Node::eraseChilds(void) 
{
   // delete child nodes
   Iterator lNode = getFirstChild();
   while(lNode) delete &(*(lNode++));
   // clear list of child node pointers
   mChilds.clear();
}

//! Parse next tag using stream tokenizer \c inTokenizer; return root of corresponding sub-tree. Any tag name defined in \c inNoParseTags will be treated as if its content is a string token (content will not be parsed).
PACC::XML::Node* PACC::XML::Node::parse(PACC::Tokenizer& inTokenizer, const set<string>& inNoParseTags)
{
   Node* lNode = 0;
   // look for start tag
   string lToken;
   inTokenizer.setWhiteSpace("");
   inTokenizer.setSingleCharTokens("<");
   if(!inTokenizer.getNextToken(lToken)) return false;
   if(lToken.find_first_not_of(" \t\n\r") == string::npos) 
   {
      // flush any leading white space
      if(!inTokenizer.getNextToken(lToken)) return 0;
   }
   if(lToken == "<") 
   {
      // check for end tag
      char lChar;
      inTokenizer.getStreamPtr()->get(lChar);
      if(lChar == '/') {
         // found end tag; 
         return 0;
      }
      inTokenizer.getStreamPtr()->putback(lChar);
      // found start tag
      lNode = new Node;
      lNode->parseStartTag(inTokenizer, lToken);
      if(lToken == "/") 
      {
         // found empty tag; next token must be '>'
         inTokenizer.setWhiteSpace("");
         inTokenizer.setSingleCharTokens(">");
         if(!inTokenizer.getNextToken(lToken)) 
            lNode->throwError(inTokenizer, "unexpected eof");
         if(lToken != ">") 
            lNode->throwError(inTokenizer, "invalid start tag");
      } 
      else if(lNode->getType() == eData) 
      {
         // either parse or read tag content
         if(inNoParseTags.find((*lNode)[""]) != inNoParseTags.end()) 
            lNode->readContentAsString(inTokenizer);
         else 
            lNode->parseContent(inTokenizer, inNoParseTags);
         // next token must be '>'
         inTokenizer.setWhiteSpace(" \t\n\r");
         inTokenizer.setSingleCharTokens(">");
         if(!inTokenizer.getNextToken(lToken)) 
            lNode->throwError(inTokenizer, "unexpected eof");
         if(lToken != ">") 
            lNode->throwError(inTokenizer, "invalid end tag");
      }
      // else node must be eSpecial
   } 
   else 
   {
      // found a simple string node
      lNode = new Node;
      lNode->mType = eString;
      (*lNode)[""] = lToken;
   }
   return lNode;
}

//! Parse the attribute list using stream tokenizer \c inTokenizer; return ending token through argument \c outToken.
void PACC::XML::Node::parseAttributeList(PACC::Tokenizer& inTokenizer, string& outToken)
{
   inTokenizer.setWhiteSpace(" \t\n\r");
   inTokenizer.setSingleCharTokens("=/>");
   // next token should be an attribute name
   if(!inTokenizer.getNextToken(outToken)) 
      throwError(inTokenizer, "unexpected eof");
   // parse all attributes
   while(outToken != "/" && outToken != ">")
   {
      // ok, found an attribute name!
      string lName = outToken;
      // next token should be '='
      inTokenizer.setSingleCharTokens("=");
      if(!inTokenizer.getNextToken(outToken)) 
         throwError(inTokenizer, "unexpected eof");
      if(outToken != "=") 
         throwError(inTokenizer, "invalid attribute");
      inTokenizer.setSingleCharTokens("\"");
      // next token must be '"'
      if(!inTokenizer.getNextToken(outToken)) 
         throwError(inTokenizer, "unexpected eof");
      if(outToken != "\"") 
         throwError(inTokenizer, "invalid attribute value");
      // next token is either a value or '"'
      inTokenizer.setWhiteSpace("");
      inTokenizer.setSingleCharTokens("\"");
      if(!inTokenizer.getNextToken(outToken)) 
         throwError(inTokenizer, "unexpected eof");
      if(outToken == "\"") {
         // value is empty string
         (*this)[lName] = "";
      } else {
         (*this)[lName] = outToken;
         inTokenizer.getNextToken(outToken);
         if(outToken != "\"") 
            throwError(inTokenizer, "unexpected eof");
      }
      inTokenizer.setWhiteSpace(" \t\n\r");
      inTokenizer.setSingleCharTokens("=/>");
      if(!inTokenizer.getNextToken(outToken)) 
         throwError(inTokenizer, "unexpected eof");
   }
}

//! Parse in stream tokenizer \c inTokenizer the content of this node (current data tag). Any tag name defined in \c inNoParseTags will be treated as if its content is a string token (content will not be parsed).
void PACC::XML::Node::parseContent(PACC::Tokenizer& inTokenizer, const set<string>& inNoParseTags)
{
   Node* lChild;
   // parse all child
   while((lChild = parse(inTokenizer, inNoParseTags))) mChilds.push_back(lChild);
   // test for valid end tag
   inTokenizer.setWhiteSpace("");
   inTokenizer.setSingleCharTokens(" \t\n\r>");
   string lToken;
   if(!inTokenizer.getNextToken(lToken)) 
      throwError(inTokenizer, "unexpected eof");
   if(lToken != (*this)[""]) 
      throwError(inTokenizer, "invalid end tag");
}

//! Parse in stream tokenizer \c inTokenizer the start tag of this node, assuming that token "<" has already been read; return ending token through argument \c outToken.
void PACC::XML::Node::parseStartTag(PACC::Tokenizer& inTokenizer, string& outToken)
{
   // parse tag name
   inTokenizer.setWhiteSpace("");
   inTokenizer.setSingleCharTokens(" \t\n\r/>");
   if(!inTokenizer.getNextToken(outToken)) 
      throwError(inTokenizer, "unexpected eof");
   if(outToken.find_first_of(" \t\n\r/>") != string::npos) 
      throwError(inTokenizer, "invalid start tag name");
   switch(outToken[0]) {
      case '?': case '!':
         if(outToken[0] == '?') mType = eDecl;
         else mType = eSpecial;
         // parse special tag
         (*this)[""] = outToken;
         inTokenizer.setSingleCharTokens(">");
         if(!inTokenizer.getNextToken(outToken)) 
            throwError(inTokenizer, "unexpected eof");
            if(outToken != ">") {
               (*this)[""] += outToken;
               if(!inTokenizer.getNextToken(outToken)) 
                  throwError(inTokenizer, "unexpected eof");
            }
               break;
      default:
         // token is regular tag name
         mType = eData;
         (*this)[""] = outToken;
         parseAttributeList(inTokenizer, outToken);
   }
}

//! Read in stream tokenizer \c inTokenizer the content of this node (current tag) as a single string (do not parse content).
void PACC::XML::Node::readContentAsString(PACC::Tokenizer& inTokenizer)
{
   // create child node
   Node* lChild = new Node;
   mChilds.push_back(lChild);
   lChild->setType(eString);
   string& lValue = (*lChild)[""];
   // parse until end tag
   inTokenizer.setWhiteSpace("");
   inTokenizer.setSingleCharTokens("</>");
   string lToken;
   int lTags = 1;
   while(lTags != 0)
   {
      // check every start tag
      if(!inTokenizer.getNextToken(lToken)) 
         throwError(inTokenizer, string("unexpected eof"));
      if(lToken == "<") 
      {
         if(!inTokenizer.getNextToken(lToken)) 
            throwError(inTokenizer, string("unexpected eof"));
         if(lToken == "/")
         {
            if(!inTokenizer.getNextToken(lToken)) 
               throwError(inTokenizer, string("unexpected eof"));
            if(lToken == getValue()) --lTags;
            if(lTags != 0) lValue += "</";
         }
         else 
         {
            if(lToken == getValue()) ++lTags;
            if(lTags != 0) lValue += "<";
         }
      }
      if(lTags != 0) lValue += lToken;
   }
}

//! Serialize the sub-tree rooted by this node; return result into streamer \c outStream, using argument \c inIndent to control indentation.
void PACC::XML::Node::serialize(PACC::XML::Streamer& outStream, bool inIndent) const
{
   bool lLastChildIsTag = true;
   switch(mType)
   {
      case eDecl: case eSpecial:
         outStream.insertSpecialTag(getValue());
         break;
      case eData:
         outStream.openTag(getValue(), inIndent);
         // serialize attribute list
         for(map<string,string>::const_iterator i = begin(); i != end(); ++i)
         {
            if(i->first != "") outStream.insertAttribute(i->first, i->second);
         }
         // serialize child nodes
         for(ConstIterator lChild = getFirstChild(); lChild; ++lChild)
         {
            lChild->serialize(outStream, lLastChildIsTag);
            lLastChildIsTag = (lChild->getType() != eString);
         }
         // indent closing tag only if last child is a tag
         outStream.closeTag(lLastChildIsTag && inIndent);
         break;
      case eString:
         outStream.insertStringContent(getValue());
         break;
      default:
         throw runtime_error("Node::serialize() unknown node type!");
   }
}

//! Throw runtime error with message \c inMessage using tokenizer \c inTokenizer.
void PACC::XML::Node::throwError(PACC::Tokenizer& inTokenizer, const string& inMessage) const
{
   ostringstream lStream;
   lStream << "\n<XML::Document>";
   if(inTokenizer.getStreamName() != "") 
      lStream << " in file \"" << inTokenizer.getStreamName() << "\",";
   lStream << " at line ";
   lStream << inTokenizer.getLineNumber() << "\nwhile parsing tag \"" << getValue() << "\": " << inMessage;
   throw runtime_error(lStream.str());
}
