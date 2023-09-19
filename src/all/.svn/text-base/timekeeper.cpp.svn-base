/***************************************************************************
                               timekeeper.cpp
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

#include "timekeeper.h"

TimeKeeper::TimeKeeper() {
}

TimeKeeper::~TimeKeeper() {}
  
// Set start time as now
void TimeKeeper::start() {
  timestart = clock();
}

// Set end time as now
void TimeKeeper::end() {
  timeend = clock();
}

// Print the CPU time used
void TimeKeeper::print(ostream &out) {
  double cpusec=timeend-timestart;
  double ctps=double(CLOCKS_PER_SEC);

  cpusec=cpusec/ctps;

  out << cpusec << " seconds.\n";
  return;
}

// Return the number of CPU seconds timed
double TimeKeeper::time() {
  double ctps=double(CLOCKS_PER_SEC);
  double cpusec=timeend-timestart;

  cpusec=cpusec/ctps;
  return cpusec;
}

// Return the number of CPU seconds from start() to now -without end() call
double TimeKeeper::current_time() {
  clock_t timecur=clock();
  double ctps=double(CLOCKS_PER_SEC);
  double cpusec=timecur-timestart;

  cpusec=cpusec/ctps;
  return cpusec;
}
  
// -------------------------Progress Meter Stuff

ProgMeter::ProgMeter() {
}

ProgMeter::~ProgMeter() {
}

// Start progress meter with a total and optionally output the percentage
void ProgMeter::start(string lab, unsigned len, unsigned tot, bool perc) {
	length=len;
	total=tot;
	percent=perc;
	numbars=0;
	total_percent=0.0;
	iterate_percent=1.0/total;
	count_before_bar=unsigned(ceil(double(total)/double(length)))-1;
	if (count_before_bar==0)
	  count_before_bar=1;
	bar_frac_count=0;

  cerr << endl << a::strcenter(lab,length) << endl;
	for (unsigned i=0; i<length+2; i++)
		cerr << "-";

	cerr << "\n|" << flush;

	if (percent) {
		cerr << "  0.0%" << flush;
	  cerr.setf(ios::fixed);
	  cerr.setf(ios::showpoint);
	  cerr.precision(1);
	  cerr.setf(ios::right);
	}

	return;
}

void ProgMeter::iterate() {
  #pragma omp critical
  {
	total_percent+=iterate_percent;
	bar_frac_count++;
	if (bar_frac_count>count_before_bar) {
		bar_frac_count=0;
		unsigned newbars=unsigned(total_percent*length)-numbars;
  	numbars+=newbars;
		if (percent)
			cerr << "\b\b\b\b\b\b";
		for (unsigned i=0; i<newbars; i++)
			cerr << "#";
		cerr << flush;
		if (percent) {
		  cerr << setw(5) << total_percent*100.0 << "%" << flush;
		}
	}
  }
}

void ProgMeter::iterate(unsigned n) {
  #pragma omp critical
  {
	total_percent+=iterate_percent*n;
  div_t answer=div(static_cast<int>(n+bar_frac_count),
                   static_cast<int>(count_before_bar));
	bar_frac_count=answer.rem;
	if (answer.quot>0) {
		unsigned newbars=unsigned(total_percent*length)-numbars;
  	numbars+=newbars;
		if (percent)
			cerr << "\b\b\b\b\b\b";
		for (unsigned i=0; i<newbars; i++)
			cerr << "#";
		cerr << flush;
		if (percent) {
		  cerr << setw(5) << total_percent*100.0 << "%" << flush;
		}
	}
  }
}

void ProgMeter::finish() {
	if (percent)
		cerr << "\b\b\b\b\b\b";
  for (unsigned i=numbars; i<length; i++)
		cerr << "#";
	cerr << "|     \n";
	for (unsigned i=0; i<length+2; i++)
		cerr << "-";
	cerr << endl << flush;
}
