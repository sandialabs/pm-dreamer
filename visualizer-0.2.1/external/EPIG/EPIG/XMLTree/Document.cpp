/*
 * EPIG - The Easy Programming Interface for Graphics
 * Copyright (C) 2003  Michel Fortin, Jean-Phillipe Lebel, Marie-Eve Tremblay
 * http://www.gel.ulaval.ca/~epig/
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*!
 * \file  EPIG/XMLTree/Document.cpp
 */

#include <sstream>
#include <fstream>
#include "Document.hpp"

using namespace EPIG::XMLTree;


// Encoding

const Encoding Encoding::cUTF8("UTF-8");
const Encoding Encoding::cISOLatin1("ISO-8859-1");
const Encoding Encoding::cWindowsLatin1("windows-1250");
const Encoding Encoding::cMacRoman("macintosh");

Encoding Encoding::smCurrentEncoding = cUTF8;


// Document

Document::Document(const Element &inRoot, const std::string &inDocType,
                   const std::string &inEncoding)
: mEncoding(inEncoding), mDocType(inDocType), mRoot(inRoot) { }

void Document::write(std::ostream &inStream) const {
    // Writing XML header
    inStream << "<?xml version=\"1.0\"";
    if ( mEncoding.length() > 0 )
        inStream << " encoding=\"" << mEncoding << "\"?>\n";
    
    // Writing DocType header
    if ( mDocType.length() > 0 )
        inStream << "<!DOCTYPE " << mDocType << ">\n\n";
    
    // Writing document root
    inStream << mRoot << "\n";
}

void Document::writeToFile(const std::string &inFileName) const {
    std::ofstream lOutputFile(inFileName.c_str());
    write(lOutputFile);
    lOutputFile.close();
}
        
std::string Document::getStdStringValue() const {
    std::ostringstream lStream;
    write(lStream);
    return lStream.str();
}
