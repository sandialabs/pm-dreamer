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
 * \file  EPIG/XMLTree/Document.hpp
 */

#ifndef EPIG_XMLTree_Document_hpp_
#define EPIG_XMLTree_Document_hpp_

#include "EPIG/XMLTree/Element.hpp"
#include "EPIG/XMLTree/String.hpp"

namespace EPIG {
namespace XMLTree {

    /*!
     * \brief  Character encoding name.
     *
     * This class is a specialized string to express character encoding names.
     * It provides constants for some standard encodings. You can also set the
     * default encoding for your program to match the one of your files.
     */
    class Encoding : public std::string {
    public:
        /*!
         * \brief  Constructor with an encoding name.
         * \param  inName  A string with a valid encoding name.
         */
        explicit Encoding(const String &inName) : std::string(inName) { }
        
        static const Encoding cUTF8;           //!< 8 bit Unicode
        static const Encoding cISOLatin1;      //!< ISO-Latin 1 (Linux)
        static const Encoding cWindowsLatin1;  //!< Windows Latin 1
        static const Encoding cMacRoman;       //!< Macincosh Roman
        
        /*!
         * \brief  Set the default encoding to this encoding.
         */
        void setDefault() const { smCurrentEncoding = *this; }
        
        /*!
         * \brief   Get the default encoding.
         * \return  An encoding that has been set as the default.
         */
        static Encoding getDefault() { return smCurrentEncoding; }
        
    private:
        static Encoding smCurrentEncoding; //!< Default encoding.
    };
    
    /*!
     * \brief  Representation of an XML document
     *
     * This class handle the components of an XML document, like the XML header
     * and the doctype.
     */
    class Document {
    public:
        /*!
         * \brief  Constructor for a document with root element, specifying
         *         doctype, namespace, and text encoding.
         * \param  inRoot       Optional root element for the document.
         * \param  inDocType    Optional document type definition content.
         * \param  inEncoding   Optional text encoding name for the document.
         *
         * If you do not spefify a character encoding, Document use the default
         * encoding provided by Encoding::getDefault().
         */
        Document(const Element &inRoot, 
                 const std::string &inDocType = std::string(), 
                 const std::string &inEncoding = Encoding::getDefault());
        
        /*!
         * \brief  Write the document to the stream.
         * \param  inStream  A stream to write the document to.
         *
         * \see  operator<<(std::ostream &, const Document &)
         */
        void write(std::ostream &inStream) const;
        /*!
         * \brief  Write the document to a file.
         * \param  inFileName  A fully qualified file name. 
         * \note   If the file already exists, it's content will be replaced by 
         *         the new document.
         */
        void writeToFile(const std::string &inFileName) const;
        /*!
         * \brief   Get the document as a string.
         * \return  A standard library string with the XML document.
         */
        std::string getStdStringValue() const;

    private:
        std::string mEncoding; //!< Character encoding.
        std::string mDocType;  //!< Document type definition for this document.
    protected:
        Element mRoot;    //!< Root element.
    };
    
    /*!
     * \brief   Output a document to an output stream.
     * \param   inStream    A stream to output the document to.
     * \param   inDocument  A document to output to the stream.
     * \return  A reference to the stream.
     *
     * This method simply call the Document::write().
     */
    inline std::ostream &operator << (std::ostream &inStream, 
                                      const XMLTree::Document &inDocument) {
        inDocument.write(inStream);
        return inStream;
    }

} // namespace XMLTree
} // namespace EPIG

#endif
