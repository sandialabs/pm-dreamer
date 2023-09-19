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
 * \file  EPIG/XMLTree/String.hpp
 */

#ifndef EPIG_XMLTree_String_hpp_
#define EPIG_XMLTree_String_hpp_

#include <string>
#include <stdexcept>
#include "EPIG/XMLTree/Node.hpp"

namespace EPIG {
namespace XMLTree {

    /*!
     * \brief  Memory efficient string object.
     *
     * This class is a Node and saves memory by not making a copy of the data
     * each time you copy the object. This class won't allocate more memory when
     * you make a copy of an already existing String, and it will do 
     * string concatenations in a memory efficient manner.
     *
     * String can seamlessly convert to and from std::string. You do not have
     * to worrie about this convertion unless you care much about performance.
     * Like for std::string, this class can be seamlessly converted from a
     * normal C string constant.
     *
     * This String class is a decendent of Node, so you can insert it as text in
     * an Element.
     *
     * \internal
     * String use two data classes. DataString is when storing the string in
     * it's simplest form as a std::string object. When making concatenation the
     * string only keep a refererence to both substrings using a DataPair 
     * object.
     *
     * Both String::DataString and String::DataPair are private and inherit from
     * the String::Data abstract class. String's data is always of this last 
     * class.
     */
    class String : public Node {
    public:
        /*!
         * \brief  Default constructor. Construct an empty string.
         */
        String();
        /*!
         * \brief  Constructor for one character string.
         * \param  inChar  Character composing the string.
         */
        explicit String(char inChar);
        //@{
        /*!
         * \brief  Constructor for converting a number.
         * \param  inNumber  Number to convert to a string.
         */
        explicit String(float inNumber);
        explicit String(double inNumber);
        explicit String(signed int inNumber);
        explicit String(signed short inNumber);
        explicit String(signed long inNumber);
        explicit String(unsigned int inNumber);
        explicit String(unsigned short inNumber);
        explicit String(unsigned long inNumber);
        //@}
        /*!
         * \brief  Constructor by C-style string.
         * \param  inString  Null-terminated character array to copy from.
         */
        String(const char inString[]);
        /*!
         * \brief  Constructor by standard string.
         * \param  inString  Standard library string to copy characters from.
         */
        String(const std::string &inString);

        //@{
        /*!
         * \brief   Concatenation operator.
         * \param   inString  String to concatenate to this one.
         * \return  This string with the second one appended at the end.
         *
         * Concatenation works in constant time because internaly a concatened
         * string is made of a reference to each of its two comonents.
         */
        String operator + (const String &inString) const;
        String &operator += (const String &inString);
        //@}

        /*!
         * \brief   Tell if the content of two string is similar.
         * \param   inString  A string to compare to this one.
         * \return  True if both strings are identical, false otherwise.
         */
        bool operator == (const String &inString) const;
        /*!
         * \brief   Tell if the content of two strings is different.
         * \param   inString  A string to compare to this one.
         * \return  True if both strings are different, false otherwise.
         */
        bool operator != (const String &inString) const;
        /*!
         * \brief   Tell if this string sould be classed higher or lower that
         *          the second one when sorting.
         * \param   inString  A string to compare to this one.
         * \return  True if the first string get ranked higher, false otherwise.
         */
        bool operator < (const String &inString) const;

        /*!
         * \brief   Get the number of characters of the string.
         * \return  The number of characters in the string.
         */
        unsigned length() const;

        /*!
         * \brief  Automatic convertion to a standard library string object.
         *
         * This is a convenience for where you need to use a std::string for
         * compatibility purpose.
         */
        operator std::string () const;

        //@{
        /*!
         * \brief   Convert the string into the appropriate numeric format.
         * \return  A number extracted from the string.
         * \throws  DataFormatException  If the string is unsuitable for
         *                                       conversion.
         */
        signed int getIntValue() const;
        signed short getShortValue() const;
        signed long getLongValue() const;
        unsigned int getUnsignedIntValue() const;
        unsigned short getUnsignedShortValue() const;
        unsigned long getUnsignedLongValue() const;
        float getFloatValue() const;
        double getDoubleValue() const;
        //@}
        
        static const NodeType cNodeType; //!< Node type a string.

    private:
        /*!
         * \brief  Concatenation constructor.
         * \param  inString1  First part of the new string.
         * \param  inString2  Second part of the new string.
         *
         * This constructor enforce the minimum character per fragment ratio.
         */
        String(const String &inString1, const String &inString2);

        /*!
         * \brief  Default empty string.
         *
         * Used to avoid allocaing a different data block for each empty string.
         */
        static const String cEmpty;
        /*!
         * \brief  Minimum character per fragment required when concatenating.
         */
        static const unsigned cMinCharPerFragment;
        
        class Data;
        class DataString;
        class DataPair;

        const Data &getData() const { return (Data &)Node::getData(); }
        Data &getData() { return (Data &)Node::getData(); }
        
        /*!
         * \brief   Ensure the string data is in one single memory block.
         * 
         * This method consolidate the string with only one block of data. It is
         * called automaticaly when the minimum character per fragment ratio is
         * too small.
         */
        void consolidate() const;
        /*!
         * \brief   Merge string data with another string.
         * \param   inString  The string to merge with.
         * 
         * This method dispose of the string data from one of the two strings to
         * free up some memory. Since it is assumed that both strings are equals
         * the merging can replace one's data with the other without breaking
         * constness.
         *
         * \note    This should really ONLY be called when the two strings are 
         *          known to be EQUALS, otherwise unpredictable result may 
         *          occur.
         */
        void mergeWith(const String &inString) const;
        /*!
         * \brief   Tell the number of fragments that are present in the string
         *          data.
         * \return  The count of all the fragments or one if not fragmented.
         *
         * When strings are concatenated, character data is stored a
         * reference to the two other string data. For small strings this may
         * cause the string to take more memory than needed because of the
         * pointer size, and some operations to be slower than they need to be
         * because the data is fragmented.
         */
        unsigned fragmentation() const;
    };

    /*!
     * \internal
     * \brief  Abstract class for string data.
     *
     * This class provide abstract string data funcions. String never use this 
     * class directly. See String::DataString and String::DataPair for concrete
     * implementation of this class.
     */
    class String::Data : public Node::Data {
    public:
        Data() : Node::Data(cNodeType) { }
    
        /*!
         * \brief   Tell the number of characters in the string.
         * \return  The number of characters as an unsigned integer.
         */
        virtual unsigned length() const = 0;
        
        /*!
         * \brief  Write the string to the stream.
         * \param  inStream  A stream to write the string to.
         *
         * When writting text, some characters are replaced by corresponding
         * entities to avoid clash with formatting characters. This produce text
         * that is suitable in a XML document. Here are the characters and the 
         * corresponding entities:
         *
         * <table>
         * <tr><th> Character </th><th> Replacement Entity </th></tr>
         * <tr><td> &amp;     </td><td> &amp;amp;          </td></tr>
         * <tr><td> &lt;      </td><td> &amp;lt;           </td></tr>
         * <tr><td> &gt;      </td><td> &amp;gt;           </td></tr>
         * </table>
         */
        virtual void write(std::ostream &inStream) const = 0;
        
        /*!
         * \brief  Write the string to the stream.
         * \param  inStream  A stream to write the string to.
         *
         * Contrary to write(), this method do not escape any characters.
         */
        virtual void writeText(std::ostream &inStream) const = 0;        
        /*!
         * \brief   Get the string associated with this data.
         * \return  A standard library string.
         */
        virtual std::string getStdStringValue() const = 0;
        
        /*!
         * \brief   Tell if the string is fragmented.
         * \return  True if the data is made of many smaller chrunchs, false
         *          if it is all in one block.
         *
         * This is used by the consolidate String method.
         */
        virtual bool isFragmented() const { return false; }
        
        /*!
         * \brief   Tell if the string is fragmented.
         * \return  True if the data is made of many smaller chrunchs, false
         *          if it is all in one block.
         *
         * This is used by the consolidate String method.
         */
        virtual unsigned fragmentation() const { return 1; }
    };

    /*!
     * \internal
     * \brief  Data containing a string.
     */
    class String::DataString : public String::Data {
    public:
        /*!
         * \brief  Default constructor.
         *
         * Create an empty string data.
         */
        DataString();
        /*!
         * \brief  Constructor with a character array.
         * \param  inString  A character array.
         *
         * Create a string from the array. DataString does not in anyway free
         * the array memory.
         */
        DataString(const char inString[]);
        /*!
         * \brief  Constructor with a standard library string.
         * \param  inString  A standard string.
         */
        DataString(const std::string &inString);

        virtual unsigned length() const;
        virtual void writeText(std::ostream &inStream) const;
        virtual std::string getStdStringValue() const;
        
        virtual void write(std::ostream &inStream) const;
        virtual DataString *clone() const;

    private:
        std::string mString; //!< The std::string encapsulating the data.
    };

    /*!
     * \internal
     * \brief  Data containing two string joined together.
     */
    class String::DataPair : public String::Data {
    public:
        /*!
         * \brief  Constructor with a pair of string data.
         * \param  inData1  First string of the pair.
         * \param  inData2  Second string of the pair.
         */
        DataPair(const String &inData1, const String &inData2);

        virtual unsigned length() const;
        virtual void writeText(std::ostream &inStream) const;
        virtual std::string getStdStringValue() const;

        virtual void write(std::ostream &inStream) const;
        virtual DataPair *clone() const;
        
        virtual bool isFragmented() const;
        virtual unsigned fragmentation() const;

    private:
        unsigned mLength; //!< Length of the combined string.
        String mString1, mString2; //!< String data.
    };
    
    /*!
     * \brief  Exception thrown when a string could not be converted in the
     *         right data format.
     */
    class DataFormatException : public std::runtime_error {
    public:
        /*!
         * \brief  Construction with description.
         * \param  inDescription  A description C string.
         */
        DataFormatException(const char inDescription[])
        : std::runtime_error(inDescription) { }
    };

} // namespace XMLTree
} // namespace EPIG

#endif
