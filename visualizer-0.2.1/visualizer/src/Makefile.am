INCLUDES          = -I../../PACC -I../../external/EPIG -I../include
bin_PROGRAMS      = beagleviz tree2svg
beagleviz_LDADD   = ../../external/EPIG/EPIG/libEPIG.la ../../PACC/XML/libXML.la ../../PACC/Util/libUtil.la
beagleviz_LDFLAGS = -static
beagleviz_SOURCES = \
BVBeagle.cpp \
BVContextHTML.cpp \
BVDeme.cpp \
BVEvolver.cpp \
BVFitness.cpp \
BVGenotype.cpp \
BVGraph.cpp \
BVHallOfFame.cpp \
BVIndividual.cpp \
BVLogger.cpp \
BVMain.cpp \
BVRegister.cpp \
BVSeeds.cpp \
BVStats.cpp \
BVTreeAverager.cpp \
BVUtil.cpp \
BVVivarium.cpp
tree2svg_LDADD   = ../../external/EPIG/EPIG/libEPIG.la ../../PACC/XML/libXML.la ../../PACC/Util/libUtil.la
tree2svg_LDFLAGS = -static
tree2svg_SOURCES = tree2svg.cpp
