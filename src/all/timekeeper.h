/***************************************************************************
                                timekeeper.h
                             -------------------
                               W. Michael Brown

  Class for timing stuff

  __________________________________________________________________________
    This file is part of the 'all' library
  __________________________________________________________________________

    begin      : May 26 2003
    authors    : W. Michael Brown
    email      : wmbrown@sandia.gov
 ***************************************************************************/

/* -----------------------------------------------------------------------
   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.
   ----------------------------------------------------------------------- */

#ifndef TIMEKEEPER
#define TIMEKEEPER

#include "misc.h"

#include <iostream>
#include <iomanip>

using namespace std;

/// Keep track of cpu time required for operations
class TimeKeeper {
 public:
  TimeKeeper();
  ~TimeKeeper();
  
  /// Set start time as now
  void start();
  /// Set end time as now
  void end();
  /// Print the CPU time used
  void print(ostream &out);
  /// Return the number of CPU seconds timed
  double time();
  /// Return the number of CPU seconds from start() to now -without end() call
  double current_time();

 private:
  clock_t timestart;
  clock_t timeend;
};

/// Visual Display of Operation Progress
class ProgMeter {
 public:
	ProgMeter();
	~ProgMeter();

  /// Start progress meter with a total and optionally output the percentage
	void start(string label, unsigned length, unsigned total, bool percent);
	/// Advance the progress one more towards the total supplied in start()
  /** \note Thread safe for OpenMP **/
	void iterate();
	/// Advance the progress n more towards the total supplied in start()
  /** \note Thread safe for OpenMP **/
	void iterate(unsigned n);
	/// Close out the progress meter
	void finish();
 private:
  unsigned total,length;
	double total_percent;
	double iterate_percent;
	unsigned count_before_bar;
	unsigned bar_frac_count;
  bool percent;

  unsigned numbars;
};
#endif
