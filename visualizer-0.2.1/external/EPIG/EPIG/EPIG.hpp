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
 * \file   EPIG/EPIG.hpp
 * \brief  Master header file for the entire EPIG interface.
 *
 * Including this header ensure that all classes of EPIG are available.
 */
 
#ifndef EPIG_hpp_
#define EPIG_hpp_

/*!
 * \mainpage  Easy Programming Interface for Graphics
 *
 * %EPIG is an easy way to output graphics from a C++ program. Graphics can be
 * sent to a <a href="http://www.w3.org/TR/SVG/">SVG</a> file to be displayed by
 * any SVG capable viewer. Alternatively, you can also send your graphic output
 * to the %EPIG Viewer using the Canvas class. The later option has the advantage
 * of being interactive, that means your program can change graphics on the
 * canvas dynamically.
 *
 * Please visit the <a href="http://www.gel.ulaval.ca/~epig/">%EPIG
 * Web site</a> for a detailed manual.
 */

/*!
 * \brief  Provides classes for generating graphics and sending them to
 *         EPIG Viewer.
 */
namespace EPIG { }

// Include EPIG XML Classes

#include "EPIG/XMLTree/XMLTree.hpp"

// Include EPIG Graphic Classes

// Special types
#include "EPIG/Types.hpp"
#include "EPIG/Styles.hpp"
#include "EPIG/Transforms.hpp"

// Graphics
#include "EPIG/Graphics.hpp"
#include "EPIG/CommonShapes.hpp"
#include "EPIG/Path.hpp"
#include "EPIG/Text.hpp"

#endif
