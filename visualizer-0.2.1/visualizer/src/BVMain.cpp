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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include "BVBeagle.hpp"

using namespace std;


void usage(char** argv, ostream& ioOS);


/*!
 *
 */
int main(int argc, char **argv)
{
  // System variables
  BVBeagle      lBVSystem;
  BVContextHTML lBVContext(".", "http://beagle.gel.ulaval.ca/visualizer");

  // Process input arguments
  std::vector<std::string> lFiles;
  for(int i=1; i<argc; ++i) {
    if(argv[i][0]=='-') {
      switch(argv[i][1]) {
        case 'h': usage(argv,cout); break;
        case 'o': if(++i==argc) usage(argv,cerr); lBVContext.mOutDir=argv[i]; break;
        case 'b': if(++i==argc) usage(argv,cerr); lBVContext.mBaseURL=argv[i]; break;
        default: usage(argv,cerr); break;
      }
    }
    else lFiles.push_back(argv[i]);
  }
  if(lFiles.size()==0) usage(argv, cerr);

  // Get time of day
  time_t lTime = time(NULL);
  tm* lTM = std::localtime(&lTime);
  char lFormatedTime[512];
  strftime(lFormatedTime, 512, "%b %d %X %Y", lTM);
  lBVContext.mDate = lFormatedTime;
  
  // Read Open BEAGLE XML files.
  cout << "Using output directory \"" << lBVContext.mOutDir << "\"" << endl;
  cout << "Using base URL " << lBVContext.mBaseURL << endl;
  for(unsigned int i=0; i<lFiles.size(); ++i) {
    cout << "Reading Open BEAGLE file \"" << lFiles[i] << "\".." << flush;
    try {
      bool lResult=lBVSystem.parse(lFiles[i]);
      if(lResult) cout << "done" << endl;
      else cout << "invalid file" << endl;
    }
    catch(std::exception& inException) {
      cout << "failed" << endl;
      cerr << "Exception catched while parsing XML file \"" << lFiles[i] << "\":" << endl;
      cerr << " \"" << inException.what() << "\"" << endl;
      cerr << "Exiting (error code 2)" << endl << endl;
      exit(2);
    }
    catch(...) {
      cout << "failed" << endl;
      cerr << "Unknown exception catched while parsing XML file \"" << lFiles[i] << "\"" << endl;
      cerr << "Exiting (error code 3)" << endl << endl;
      exit(3);
    }
  }

  // Generate HTML report.
  try {
    lBVSystem.generateReport(lBVContext, cout);
  }
  catch(std::exception& inException) {
    cerr << endl;
    cerr << "Exception catched while generating report:" << endl;
    cerr << " \"" << inException.what() << "\"" << endl;
    cerr << "Exiting (error code 4)" << endl << endl;
    exit(4);
  }
  catch(...) {
    cerr << endl;
    cerr << "Unknown exception catched while generating report" << endl;
    cerr << "Exiting (error code 5)" << endl << endl;
    exit(5);
  }
    
  // Exiting gracefully
  cout << "Report successfully generated" << endl;
  return 0;
}


/*!
 *
 */
void usage(char** argv, ostream& ioOS)
{
  ioOS << argv[0] << ": BEAGLE Visualizer HTML report generator" << endl;
  ioOS << "usage> " << argv[0] << " [OPTIONS] FILE1 [FILE2 [FILE3 ... ]]" << endl;
  ioOS << "  Where FILE# is an Open BEAGLE XML file" << endl;
  ioOS << "  Supported options are:" << endl;
  ioOS << "  -o DIRECTORY: Change output directory of report  (default: \".\")" << endl;
  ioOS << "  -b URL: Change base URL (default: http://beagle.gel.ulaval.ca/visualizer)" << endl;
  ioOS << "  -h: Show usage" << endl;
  if(ioOS==&cerr) {
    ioOS << "Bad command-line!" << endl;
    ioOS << "Exiting (error code 1)" << endl << endl;
    exit(1);
  }
  ioOS << endl;
  exit(0);
}

