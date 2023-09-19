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
 * \file   EPIG/XMLTree/XMLTree.hpp
 * \brief  Master header file for the entire XMLTree interface.
 *
 * Including this header ensure that all classes of XMLTree are available.
 */

#ifndef EPIG_XMLTree_hpp_
#define EPIG_XMLTree_hpp_

/*!
 * \mainpage  EPIG XML classes
 *
 * This is the classes for XML manipulation used behind the senes by EPIG.
 * Theses classes keep a tree representation of the data that can be easyly
 * manipulated and converted in XML text at the right moment.
 *
 * Please visit the <a href="http://www.gel.ulaval.ca/~epig/">EPIG
 * Web site</a> for actual informations.
 */

namespace EPIG {
    /*!
     * \brief  Contain classes used to maintain a tree of XML data.
     */
    namespace XMLTree { }
}

#include "EPIG/XMLTree/Node.hpp"
#include "EPIG/XMLTree/String.hpp"
#include "EPIG/XMLTree/Element.hpp"
#include "EPIG/XMLTree/Attribute.hpp"
#include "EPIG/XMLTree/Document.hpp"

#endif
