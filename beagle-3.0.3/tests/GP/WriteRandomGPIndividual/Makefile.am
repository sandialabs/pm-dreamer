if COMPILE_TESTS
noinst_PROGRAMS = writeRandomGPIndividual
INCLUDES        = -I. -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include -I$(top_srcdir)/beagle/GA/include -I$(top_srcdir)/beagle/GP/include
writeRandomGPIndividual_LDADD = $(top_srcdir)/PACC/Math/libpacc-math.la \
                                $(top_srcdir)/PACC/Threading/libpacc-threading.la \
                                $(top_srcdir)/PACC/Util/libpacc-util.la \
                                $(top_srcdir)/PACC/XML/libpacc-xml.la \
                                $(top_srcdir)/beagle/src/libbeagle.la \
                                $(top_srcdir)/beagle/GP/src/libbeagle-GP.la
writeRandomGPIndividual_SOURCES = WriteRandomGPIndividual.cpp
endif COMPILE_TESTS
