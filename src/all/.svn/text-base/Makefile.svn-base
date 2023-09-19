#***************************************************************************
#                                  Makefile 
#                             -------------------
#
#  Build for 'all' library
#
#  __________________________________________________________________________
#
#    begin                : Thu Oct 9 2003
#    copyright            : (C) 2003 by W. Michael Brown
#    email                : wmbrown@sandia.gov
#***************************************************************************/

include ../Makefile.vars

OBJ_DIR = $(HOBJ_DIR)/all

OBJS = $(OBJ_DIR)/error.o $(OBJ_DIR)/commandline.o \
       $(OBJ_DIR)/misc.o $(OBJ_DIR)/timekeeper.o

EXECS =

all: $(OBJS) $(ALL_LIB) $(EXECS)

$(OBJ_DIR)/error.o: error.h error.cpp
	$(CPP) $(CFLAGS) -o $@ error.cpp

$(OBJ_DIR)/commandline.o: commandline.h commandline.cpp
	$(CPP) $(CFLAGS) -o $@ commandline.cpp

$(OBJ_DIR)/misc.o: misc.h misc.cpp
	$(CPP) $(CFLAGS) -o $@ misc.cpp

$(OBJ_DIR)/miscmath.o: miscmath.h miscmath.cpp
	$(CPP) $(CFLAGS) -o $@ miscmath.cpp

$(OBJ_DIR)/timekeeper.o: timekeeper.h timekeeper.cpp
	$(CPP) $(CFLAGS) -o $@ timekeeper.cpp

$(ALL_LIB): $(OBJS)
	$(AR) -crusv $(ALL_LIB) $(OBJS)

clean:
	rm -f $(OBJS) $(ALL_LIB) $(EXECS) a.out core

veryclean: clean
	rm -f *~ ./api
