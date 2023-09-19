#***************************************************************************
#                                  Makefile 
#                             -------------------
#
#  Build for PM-Dreamer project
#
#  __________________________________________________________________________
#
#    begin                : Thu Jun 5 2008
#    copyright            : (C) 2008 by W. Michael Brown
#    email                : wmbrown@sandia.gov
# ***************************************************************************/

include ../Makefile.vars

OBJ_DIR = $(HOBJ_DIR)

SOBJS = $(OBJ_DIR)/pair_potential.o \
        $(OBJ_DIR)/MPITerminateOp.o $(OBJ_DIR)/MigrationMPIOp.o \
	$(OBJ_DIR)/HFCompMPIOp.o $(OBJ_DIR)/TermMaxTimeOp.o \
	$(OBJ_DIR)/StatsCalcFitSimpleMPIOp.o $(OBJ_DIR)/two_three_potential.o \
	$(OBJ_DIR)/vtwo_three_potential.o $(OBJ_DIR)/vpair_potential.o \
	$(OBJ_DIR)/fit_stat.o $(OBJ_DIR)/base_potential.o \
	$(OBJ_DIR)/base_pair.o $(OBJ_DIR)/base_twothree.o \
	$(OBJ_DIR)/SimplifyOp.o $(OBJ_DIR)/DreamerOp.o \
	$(OBJ_DIR)/HybridOptOp.o $(OBJ_DIR)/OLSCorrectOp.o $(OBJ_DIR)/manpage.o

POBJS =

ifeq ($(MPIF),-DGPMPI)
  OBJS = $(SOBJS) $(POBJS)
else
  OBJS = $(SOBJS)
endif

HEADER_DEP = PowT.hpp EphemeralInt.hpp EphemeralD.hpp \
             LeftRightT.hpp RestartReadOp.hpp VDouble.hpp VExp.hpp \
	     VLog.hpp VSin.hpp VCos.hpp VDivide.hpp

HEADERS = $(HEADER_DEP) pair_potential.h \
        MPITerminateOp.hpp MigrationMPIOp.hpp \
	HFCompMPIOp.hpp TermMaxTimeOp.hpp StatsCalcFitSimpleMPIOp.hpp \
	two_three_potential.h vtwo_three_potential.h vpair_potential.h \
	fit_stat.h base_potential.h base_pair.h base_twothree.h \
	SimplifyOp.hpp DreamerOp.hpp HybridOptOp.hpp OLSCorrectOp.hpp


HEADERS_POT = base_potential.h VDouble.hpp fit_stat.h
HEADERS_TWO = $(HEADERS_POT) base_pair.h
HEADERS_THREE = $(HEADERS_POT) base_twothree.h LeftRightT.hpp


all: libs $(BIN_DIR)/pm_dreamer $(BIN_DIR)/mdiff

libs:
	cd ../all; make; cd ../pm_dreamer

test: all
	tcsh ./run_tests.sh

testmpi: all
	tcsh ./run_tests.sh mpi

$(OBJ_DIR)/StatsCalcFitSimpleMPIOp.o: StatsCalcFitSimpleMPIOp.cpp StatsCalcFitSimpleMPIOp.hpp
	$(CPP) $(CFLAGS) -o $@ StatsCalcFitSimpleMPIOp.cpp

$(OBJ_DIR)/MPITerminateOp.o: MPITerminateOp.cpp MPITerminateOp.hpp
	$(CPP) $(CFLAGS) -o $@ MPITerminateOp.cpp

$(OBJ_DIR)/TermMaxTimeOp.o: TermMaxTimeOp.cpp TermMaxTimeOp.hpp
	$(CPP) $(CFLAGS) -o $@ TermMaxTimeOp.cpp

$(OBJ_DIR)/HFCompMPIOp.o: HFCompMPIOp.cpp HFCompMPIOp.hpp
	$(CPP) $(CFLAGS) -o $@ HFCompMPIOp.cpp

$(OBJ_DIR)/MigrationMPIOp.o: MigrationMPIOp.cpp MigrationMPIOp.hpp
	$(CPP) $(CFLAGS) -o $@ MigrationMPIOp.cpp

$(OBJ_DIR)/SimplifyOp.o: SimplifyOp.cpp SimplifyOp.hpp EphemeralD.hpp $(HEADERS_POT)
	$(CPP) $(CFLAGS) -o $@ SimplifyOp.cpp

$(OBJ_DIR)/OLSCorrectOp.o: OLSCorrectOp.cpp OLSCorrectOp.hpp EphemeralD.hpp $(HEADERS_POT)
	$(CPP) $(CFLAGS) -o $@ OLSCorrectOp.cpp

$(OBJ_DIR)/HybridOptOp.o: HybridOptOp.cpp HybridOptOp.hpp $(HEADERS_POT)
	$(CPP) $(CFLAGS) -o $@ HybridOptOp.cpp

$(OBJ_DIR)/DreamerOp.o: DreamerOp.cpp DreamerOp.hpp $(HEADERS_POT)
	$(CPP) $(CFLAGS) -o $@ DreamerOp.cpp

$(OBJ_DIR)/fit_stat.o: fit_stat.cpp fit_stat.h
	$(CPP) $(CFLAGS) -o $@ fit_stat.cpp

$(OBJ_DIR)/base_potential.o: base_potential.cpp $(HEADERS_POT)
	$(CPP) $(CFLAGS) -o $@ base_potential.cpp

$(OBJ_DIR)/base_pair.o: base_pair.cpp $(HEADERS_TWO)
	$(CPP) $(CFLAGS) -o $@ base_pair.cpp

$(OBJ_DIR)/base_twothree.o: base_twothree.cpp $(HEADERS_THREE)
	$(CPP) $(CFLAGS) -o $@ base_twothree.cpp

$(OBJ_DIR)/pair_potential.o: pair_potential.cpp pair_potential.h $(HEADERS_TWO)
	$(CPP) $(CFLAGS) -o $@ pair_potential.cpp

$(OBJ_DIR)/vpair_potential.o: vpair_potential.cpp vpair_potential.h $(HEADERS_TWO)
	$(CPP) $(CFLAGS) -o $@ vpair_potential.cpp

$(OBJ_DIR)/two_three_potential.o: two_three_potential.cpp two_three_potential.h $(HEADERS_THREE)
	$(CPP) $(CFLAGS) -o $@ two_three_potential.cpp

$(OBJ_DIR)/vtwo_three_potential.o: vtwo_three_potential.cpp vtwo_three_potential.h $(HEADERS_THREE)
	$(CPP) $(CFLAGS) -o $@ vtwo_three_potential.cpp

$(OBJ_DIR)/manpage.o: manpage.cpp $(ALL_DIR)/commandline.h
	$(CPP) $(CFLAGS) -o $@ manpage.cpp

$(OBJ_DIR)/pm_dreamer.o: pm_dreamer.cpp $(HEADERS)
	$(CPP) $(CFLAGS) -o $@ pm_dreamer.cpp

$(OBJ_DIR)/mdiff.o: mdiff.cpp
	$(CPP) $(CFLAGS) -o $@ mdiff.cpp

$(BIN_DIR)/pm_dreamer: $(OBJS) $(OBJ_DIR)/pm_dreamer.o $(ALL_LIB)
	$(CPP) -o $@  $(OBJ_DIR)/pm_dreamer.o $(OBJS) $(ALL_LIB) $(LFLAGS) $(LLIBS)

$(BIN_DIR)/mdiff: $(OBJS) $(OBJ_DIR)/mdiff.o $(ALL_LIB)
	$(CPP) -o $@ $(OBJ_DIR)/mdiff.o $(OBJS) $(ALL_LIB) $(LFLAGS) $(LLIBS)

clean:
	rm -f *.o a.out core $(SOBJS) $(POBJS) $(BIN_DIR)/pm_dreamer \
	$(BIN_DIR)/mdiff $(OBJ_DIR)/mdiff.o $(OBJ_DIR)/pm_dreamer.o; \
        cd ../all; make clean; cd ../pm_dreamer

veryclean: clean
	rm -f *~ ./test/*~ ./test/*.obm ./test/*.log ./test/*.valgrind ./test/*.log
