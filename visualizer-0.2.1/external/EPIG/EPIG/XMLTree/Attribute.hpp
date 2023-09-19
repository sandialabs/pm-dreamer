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
 * \file  EPIG/XMLTree/Attribute.hpp
 */

#ifndef EPIG_XMLTree_Attribute_hpp_
#define EPIG_XMLTree_Attribute_hpp_

#include <map>
#include "EPIG/XMLTree/String.hpp"

namespace EPIG {
namespace XMLTree {

    class AttributeList;

    /*!
     * \brief  A value associated with an attribute name.
     */
    class Attribute : private std::pair<std::string, String> {
    public:
        /*!
         * \brief  Constructor with a name and a value.
         * \param  inName   A name for this attribute.
         * \param  inValue  A String value for this attribute.
         * 
         * The value parameter is optional and you can omit it. In that case
         * an empty string will be used as the value.
         */
        Attribute(const std::string &inName, const String &inValue = String()) 
        : std::pair<std::string, String>(inName, inValue) { }

        /*!
         * \brief   Get the name of this attribute.
         * \return  The name of this attribute.
         */
        std::string getName() const { return first; }
        /*!
         * \brief   Get the value of this attribute.
         * \return  The valye of this attribute.
         */
        String getValue() const { return second; }
        /*!
         * \brief  Set the value of this attribute.
         * \param  inValue  A new value of this attribute.
         */
        void setValue(const String &inValue) { second = inValue; }

        /*!
         * \brief   Make a list of two attributes.
         * \param   inAttribute  An attribute to put after this one in the list.
         * \return  The resulting attribute list.
         */
        AttributeList operator + (const Attribute &inAttribute) const;
        /*!
         * \brief   Add this attribute to a list.
         * \param   inAttributeList  An attribute list to add this attibute to.
         * \return  The resulting attribute list.
         */
        AttributeList operator + (const AttributeList &inAttributeList) const;

    private:
        /*!
         * \brief  
         */
        Attribute(const std::pair<std::string, String> &inPair) 
        : std::pair<std::string, String>(inPair) { }
        
        friend class AttributeList; //!< Needed for access to pair base type.
    };

    /*!
     * \brief  A list of attribute.
     * 
     * An attribute list can contain only one attribute with a given name.
     * It is implemented as a std::map and inherit all it's member. 
     * AttributeList define special behaviour allowing to add attributes to
     * it using the + operator.
     */
    class AttributeList : public std::map<std::string, String> {
    public:
        /*!
         * \brief  Default constructor making an empty list.
         */
        AttributeList();
        /*!
         * \brief  Constructor with one attribute.
         * \param  inAttribute  An attribute to put in the list.
         */
        AttributeList(const Attribute &inAttribute);

        //@{
        /*!
         * \brief   Add an attribute to the list.
         * \param   inAttribute  An attribute to add to the list.
         * \return  The resulting list.
         */
        AttributeList operator + (const Attribute &inAttribute) const;
        AttributeList &operator += (const Attribute &inAttribute);
        //@}
        //@{
        /*!
         * \brief   Add another attribute list to this list.
         * \param   inAttributeList  An attribute list to add to this list.
         * \return  The resulting list.
         */
        AttributeList operator + (const AttributeList &inAttributeList) const;
        AttributeList &operator += (const AttributeList &inAttributeList);
        //@}

        /*!
         * \brief   Get attribute's value.
         * \param   inName  The name of the attribute.
         * \return  The string value of this attribute, or an empty string if
         *          the attribute is not defined.
         */
        String get(const std::string &inName) const;
        /*!
         * \brief   Tell if the attribute is defined.
         * \param   inName  The name of the attribute.
         * \return  True if the attribute can be found in the list, false
         *          otherwise.
         */
        bool defined(const std::string &inName) const;
        /*!
         * \brief  Remove specified attribute.
         * \param  inName  The name of the attribute to remove.
         */
        void remove(const std::string &inName);
    };

} // namespace XMLTree
} // namespace EPIG

#endif
