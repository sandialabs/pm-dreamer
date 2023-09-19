/*
 *  BEAGLE Visualizer
 *  Copyright (C) 2004 by Christian Gagne, Patrick-Emmanuel Boulanger-Nadeau,
 *  and Vincent-Olivier Gravel.
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

#include "BVContextHTML.hpp"

using namespace PACC;


/*!
 *
 */
BVContextHTML::BVContextHTML(std::string inOutDir, std::string inBaseURL) :
  mLogsState(BVContextHTML::eInactive),
  mStatsState(BVContextHTML::eInactive),
  mVivariumState(BVContextHTML::eInactive),
  mEvolverState(BVContextHTML::eInactive),
  mRegisterState(BVContextHTML::eInactive),
  mSeedsState(BVContextHTML::eInactive),
  mOutDir(inOutDir),
  mBaseURL(inBaseURL)
{ }


/*!
 *
 */
void BVContextHTML::writeFooter(XML::Streamer& ioStreamer) const
{
  ioStreamer.openTag("br");
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("font");
  ioStreamer.insertAttribute("size", "2");
  ioStreamer.insertStringContent(std::string("Report generated on ")+mDate+
                                 std::string(" by BEAGLE Visualizer v0.2.1"));
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();
  ioStreamer.insertStringContent(std::string("Copyright (C) 2004 by Christian Gagn&eacute;, ")+
                                 std::string("Patrick-Emmanuel Boulanger-Nadeau, and ")+
                                 std::string("Vincent-Olivier Gravel"));
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();

  ioStreamer.closeTag();  // </body>
  ioStreamer.closeTag();  // </html>
  
}


/*!
 *
 */
void BVContextHTML::writeHeader(XML::Streamer& ioStreamer) const
{
  // XHTML Header
  ioStreamer.insertHeader();
  ioStreamer.insertSpecialTag("!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"");
  ioStreamer.openTag("html");
  ioStreamer.openTag("head");
  ioStreamer.openTag("meta");
  ioStreamer.insertAttribute("http-equiv", "content-type");
  ioStreamer.insertAttribute("content", "text/html");
  ioStreamer.closeTag();
  ioStreamer.openTag("title");
  ioStreamer.insertStringContent("BEAGLE Visualizer Report");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag();
  ioStreamer.openTag("body");
  ioStreamer.insertAttribute("style", "background-color: #F5F5F5");
  ioStreamer.insertAttribute("text",    "#000000");
  ioStreamer.insertAttribute("link",    "#4B66D0");
  ioStreamer.insertAttribute("vlink",   "#F09519");
  ioStreamer.insertAttribute("alink",   "#F09519");
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();

  // Title
  ioStreamer.openTag("center");
  ioStreamer.openTag("font", false);
  ioStreamer.insertAttribute("color", "#4B66D0");
  ioStreamer.insertAttribute("size", "+5");
  ioStreamer.openTag("b", false);
  ioStreamer.openTag("em", false);
  ioStreamer.insertStringContent("BEAGLE Visualizer");
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag();
  
  // Menu
  ioStreamer.openTag("center");
  ioStreamer.insertStringContent("&nbsp;&nbsp;");
  switch(mLogsState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "logs.html");
      ioStreamer.insertStringContent("Logs");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Logs]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  switch(mStatsState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "stats.html");
      ioStreamer.insertStringContent("Statistics");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Statistics]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  switch(mVivariumState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "vivarium.html");
      ioStreamer.insertStringContent("Vivarium");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Vivarium]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  switch(mEvolverState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "evolver.html");
      ioStreamer.insertStringContent("Evolver");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Evolver]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  switch(mRegisterState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "register.html");
      ioStreamer.insertStringContent("Register");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Register]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  switch(mSeedsState) {
    case BVContextHTML::eActive: {
      ioStreamer.insertStringContent("[");
      ioStreamer.openTag("a", false);
      ioStreamer.insertAttribute("href", "seeds.html");
      ioStreamer.insertStringContent("Seeds");
      ioStreamer.closeTag(false);
      ioStreamer.insertStringContent("]&nbsp;&nbsp;");
      break;
    }
    case BVContextHTML::eInProcess: {
      ioStreamer.insertStringContent("[Seeds]&nbsp;&nbsp;");
      break;
    }
    default: break;
  }
  ioStreamer.closeTag(false);
  ioStreamer.openTag("br", false);
  ioStreamer.closeTag(false);
  ioStreamer.openTag("hr", false);
  ioStreamer.closeTag();
}


