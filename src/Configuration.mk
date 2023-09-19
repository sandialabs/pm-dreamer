#***************************************************************************
#                               Makefile
#                          -------------------
#
#  _________________________________________________________________________
#
#  Configuration for Makefiles
#
#  _________________________________________________________________________
#
#    begin                : Thu Jun 5 2008
#    copyright            : (C) 2008 by W. Michael Brown
#    email                : wmbrown@sandia.gov
# ***************************************************************************/

#Compiler type
COMPILER = gnu_fast
#COMPILER = intel_openmp
#COMPILER = gnu4_openmp

#Locations of outside objects relative to a source directory
HOBJ_DIR = ../../obj
BIN_DIR = ../../bin/

#Location of Beagle include files
BEAGLE_DIR = /usr/local/include/beagle

#Location of Beagle libraries
BEAGLE_LIB = /usr/local/lib

ifeq ($(COMPILER),intel_openmp)
  CPP         = icpc  
  OPENMP      = -openmp -DGP_OMP
  AR          = xiar
  OPT         = -ansi-alias -O2 -xT #-ipo -no-prec-div -static
endif

ifeq ($(COMPILER),gcc4)
  CPP         = g++
  OPENMP      = 
  AR          = ar
  OPT         = -O2
endif

ifeq ($(COMPILER),gnu_fast)
  CPP         = g++
  AR          = ar
  OPT         = -O2
endif

ifeq ($(COMPILER),gnu4_openmp)
  CPP         = g++4
  AR          = ar
  OPT         = -O2
  OPENMP      = -fopenmp -DGP_OMP
endif

INC = -I$(BEAGLE_DIR)

CFLAGS        = $(OPT) $(OPENMP) $(DBUG) $(INC) -c
LFLAGS        = $(OPT) $(OPENMP) -L$(BEAGLE_LIB) -lbeagle-GP -lbeagle-GA \
                -lbeagle -lpacc-math -lpacc-xml -lpacc-util -lz
