doc:
	rm -rf refman
	rm -rf PACC/refman
	mkdir -p refman/pacc
	cd PACC && ln -s ../refman/pacc refman && doxygen Doxyfile
	mkdir -p refman/beagle
	doxygen Doxyfile
	echo "<HTML><META HTTP-EQUIV='refresh' content='0;url=beagle/html/index.html'></HTML>" > refman/index.html

distdoc:
	$(MAKE) $(AM_MAKEFLAGS) doc
	rm -rf beagle-refmanual-@VERSION@-html beagle-refmanual-@VERSION@-html.tar.gz beagle-refmanual-@VERSION@-html.zip
	cp -r refman beagle-refmanual-@VERSION@-html
	tar zcvf beagle-refmanual-@VERSION@-html.tar.gz beagle-refmanual-@VERSION@-html
	zip -r beagle-refmanual-@VERSION@-html.zip beagle-refmanual-@VERSION@-html
	rm -rf beagle-refmanual-@VERSION@-html

example:
	cd examples/Coev/coev_symbreg && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/Coev/ipd          && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/knapsack       && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/maxfct         && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/maxfctes       && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/maxfctfloat    && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/onemax         && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/tsp            && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GA/zeromin        && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GP/parity         && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GP/spambase       && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)
	cd examples/GP/symbreg        && $(MAKE) $(AM_MAKEFLAGS) clean && $(MAKE) $(AM_MAKEFLAGS)

pch:
	rm -f beagle/include/beagle/Beagle.hpp.gch beagle/GA/include/beagle/GA.hpp.gch beagle/GP/include/beagle/GP.hpp.gch beagle/Coev/include/beagle/Coev.hpp.gch
	cd beagle/Coev/include/beagle && $(MAKE) $(AM_MAKEFLAGS) pch
	cd beagle/GP/include/beagle && $(MAKE) $(AM_MAKEFLAGS) pch
	cd beagle/GA/include/beagle && $(MAKE) $(AM_MAKEFLAGS) pch
	cd beagle/include/beagle && $(MAKE) $(AM_MAKEFLAGS) pch

AUTOMAKE_OPTIONS=dist-zip
SUBDIRS = PACC beagle tests
EXTRA_DIST = \
bootstrap \
beagle.xsd \
beagle.kdevelop \
beagle.lsm \
Doxyfile \
Makefile.cvs \
reconfigure \
config/config.guess \
config/config.sub \
config/depcomp \
config/install-sh \
config/ltmain.sh \
config/missing \
config/mkinstalldirs \
examples/Coev/coev_symbreg/acinclude.m4 \
examples/Coev/coev_symbreg/aclocal.m4 \
examples/Coev/coev_symbreg/bootstrap \
examples/Coev/coev_symbreg/configure \
examples/Coev/coev_symbreg/configure.ac \
examples/Coev/coev_symbreg/Makefile.am \
examples/Coev/coev_symbreg/Makefile.cvs \
examples/Coev/coev_symbreg/Makefile.in \
examples/Coev/coev_symbreg/AUTHORS \
examples/Coev/coev_symbreg/COPYING \
examples/Coev/coev_symbreg/INSTALL \
examples/Coev/coev_symbreg/README \
examples/Coev/coev_symbreg/coev_symbreg.kdevelop \
examples/Coev/coev_symbreg/config/config.guess \
examples/Coev/coev_symbreg/config/config.sub \
examples/Coev/coev_symbreg/config/depcomp \
examples/Coev/coev_symbreg/config/install-sh \
examples/Coev/coev_symbreg/config/ltmain.sh \
examples/Coev/coev_symbreg/config/missing \
examples/Coev/coev_symbreg/coev_symbreg/Makefile.am \
examples/Coev/coev_symbreg/coev_symbreg/Makefile.in \
examples/Coev/coev_symbreg/coev_symbreg/config.hpp.in \
examples/Coev/coev_symbreg/coev_symbreg/CoevSymbregMain.cpp \
examples/Coev/coev_symbreg/coev_symbreg/CoSymEvalOp.cpp \
examples/Coev/coev_symbreg/coev_symbreg/CoSymEvalOp.hpp \
examples/Coev/coev_symbreg/coev_symbreg/SymGPEvalOp.cpp \
examples/Coev/coev_symbreg/coev_symbreg/SymGPEvalOp.hpp \
examples/Coev/coev_symbreg/coev_symbreg/SymGPThread.cpp \
examples/Coev/coev_symbreg/coev_symbreg/SymGPThread.hpp \
examples/Coev/coev_symbreg/coev_symbreg/TrainSetEvalOp.cpp \
examples/Coev/coev_symbreg/coev_symbreg/TrainSetEvalOp.hpp \
examples/Coev/coev_symbreg/coev_symbreg/TrainSetThread.cpp \
examples/Coev/coev_symbreg/coev_symbreg/TrainSetThread.hpp \
examples/Coev/coev_symbreg/coev_symbreg/symgp-thread.conf \
examples/Coev/coev_symbreg/coev_symbreg/trainset-thread.conf \
examples/Coev/coev_symbreg/templates/cpp \
examples/Coev/coev_symbreg/templates/hpp \
examples/Coev/coev_symbreg/MSVCPP/coev_symbreg.sln \
examples/Coev/coev_symbreg/MSVCPP/coev_symbreg.suo \
examples/Coev/coev_symbreg/MSVCPP/coev_symbreg/coev_symbreg.vcproj \
examples/Coev/ipd/acinclude.m4 \
examples/Coev/ipd/aclocal.m4 \
examples/Coev/ipd/bootstrap \
examples/Coev/ipd/configure \
examples/Coev/ipd/configure.ac \
examples/Coev/ipd/Makefile.am \
examples/Coev/ipd/Makefile.cvs \
examples/Coev/ipd/Makefile.in \
examples/Coev/ipd/AUTHORS \
examples/Coev/ipd/COPYING \
examples/Coev/ipd/INSTALL \
examples/Coev/ipd/README \
examples/Coev/ipd/ipd.kdevelop \
examples/Coev/ipd/config/config.guess \
examples/Coev/ipd/config/config.sub \
examples/Coev/ipd/config/depcomp \
examples/Coev/ipd/config/install-sh \
examples/Coev/ipd/config/ltmain.sh \
examples/Coev/ipd/config/missing \
examples/Coev/ipd/ipd/Makefile.am \
examples/Coev/ipd/ipd/Makefile.in \
examples/Coev/ipd/ipd/config.hpp.in \
examples/Coev/ipd/ipd/IPDEvalOp.cpp \
examples/Coev/ipd/ipd/IPDEvalOp.hpp \
examples/Coev/ipd/ipd/IPDMain.cpp \
examples/Coev/ipd/ipd/IPDThread.cpp \
examples/Coev/ipd/ipd/IPDThread.hpp \
examples/Coev/ipd/ipd/ipd-thread1.conf \
examples/Coev/ipd/ipd/ipd-thread2.conf \
examples/Coev/ipd/templates/cpp \
examples/Coev/ipd/templates/hpp \
examples/Coev/ipd/MSVCPP/ipd.sln \
examples/Coev/ipd/MSVCPP/ipd.suo \
examples/Coev/ipd/MSVCPP/ipd/ipd.vcproj \
examples/GA/knapsack/acinclude.m4 \
examples/GA/knapsack/aclocal.m4 \
examples/GA/knapsack/bootstrap \
examples/GA/knapsack/configure \
examples/GA/knapsack/configure.ac \
examples/GA/knapsack/Makefile.am \
examples/GA/knapsack/Makefile.cvs \
examples/GA/knapsack/Makefile.in \
examples/GA/knapsack/AUTHORS \
examples/GA/knapsack/COPYING \
examples/GA/knapsack/INSTALL \
examples/GA/knapsack/README \
examples/GA/knapsack/knapsack.kdevelop \
examples/GA/knapsack/config/config.guess \
examples/GA/knapsack/config/config.sub \
examples/GA/knapsack/config/depcomp \
examples/GA/knapsack/config/install-sh \
examples/GA/knapsack/config/ltmain.sh \
examples/GA/knapsack/config/missing \
examples/GA/knapsack/knapsack/Makefile.am \
examples/GA/knapsack/knapsack/Makefile.in \
examples/GA/knapsack/knapsack/config.hpp.in \
examples/GA/knapsack/knapsack/KnapsackEvalOp.cpp \
examples/GA/knapsack/knapsack/KnapsackEvalOp.hpp \
examples/GA/knapsack/knapsack/KnapsackMain.cpp \
examples/GA/knapsack/knapsack/knapsack.conf \
examples/GA/knapsack/knapsack/knapsack-npga2.conf \
examples/GA/knapsack/knapsack/knapsack-nsga2.conf \
examples/GA/knapsack/knapsack/knapsack-nsga2-non-breeder.conf \
examples/GA/knapsack/templates/cpp \
examples/GA/knapsack/templates/hpp \
examples/GA/knapsack/MSVCPP/knapsack.sln \
examples/GA/knapsack/MSVCPP/knapsack.suo \
examples/GA/knapsack/MSVCPP/knapsack/knapsack.vcproj \
examples/GA/maxfct/acinclude.m4 \
examples/GA/maxfct/aclocal.m4 \
examples/GA/maxfct/bootstrap \
examples/GA/maxfct/configure \
examples/GA/maxfct/configure.ac \
examples/GA/maxfct/Makefile.am \
examples/GA/maxfct/Makefile.cvs \
examples/GA/maxfct/Makefile.in \
examples/GA/maxfct/AUTHORS \
examples/GA/maxfct/COPYING \
examples/GA/maxfct/INSTALL \
examples/GA/maxfct/README \
examples/GA/maxfct/maxfct.kdevelop \
examples/GA/maxfct/config/config.guess \
examples/GA/maxfct/config/config.sub \
examples/GA/maxfct/config/depcomp \
examples/GA/maxfct/config/install-sh \
examples/GA/maxfct/config/ltmain.sh \
examples/GA/maxfct/config/missing \
examples/GA/maxfct/maxfct/Makefile.am \
examples/GA/maxfct/maxfct/Makefile.in \
examples/GA/maxfct/maxfct/config.hpp.in \
examples/GA/maxfct/maxfct/MaxFctEvalOp.cpp \
examples/GA/maxfct/maxfct/MaxFctEvalOp.hpp \
examples/GA/maxfct/maxfct/MaxFctMain.cpp \
examples/GA/maxfct/maxfct/maxfct.conf \
examples/GA/maxfct/maxfct/maxfct-generational.conf \
examples/GA/maxfct/maxfct/maxfct-steadystate.conf \
examples/GA/maxfct/templates/cpp \
examples/GA/maxfct/templates/hpp \
examples/GA/maxfct/MSVCPP/maxfct.sln \
examples/GA/maxfct/MSVCPP/maxfct.suo \
examples/GA/maxfct/MSVCPP/maxfct/maxfct.vcproj \
examples/GA/maxfctes/acinclude.m4 \
examples/GA/maxfctes/aclocal.m4 \
examples/GA/maxfctes/bootstrap \
examples/GA/maxfctes/configure \
examples/GA/maxfctes/configure.ac \
examples/GA/maxfctes/Makefile.am \
examples/GA/maxfctes/Makefile.cvs \
examples/GA/maxfctes/Makefile.in \
examples/GA/maxfctes/AUTHORS \
examples/GA/maxfctes/COPYING \
examples/GA/maxfctes/INSTALL \
examples/GA/maxfctes/README \
examples/GA/maxfctes/maxfctes.kdevelop \
examples/GA/maxfctes/config/config.guess \
examples/GA/maxfctes/config/config.sub \
examples/GA/maxfctes/config/depcomp \
examples/GA/maxfctes/config/install-sh \
examples/GA/maxfctes/config/ltmain.sh \
examples/GA/maxfctes/config/missing \
examples/GA/maxfctes/maxfctes/Makefile.am \
examples/GA/maxfctes/maxfctes/Makefile.in \
examples/GA/maxfctes/maxfctes/config.hpp.in \
examples/GA/maxfctes/maxfctes/MaxFctESEvalOp.cpp \
examples/GA/maxfctes/maxfctes/MaxFctESEvalOp.hpp \
examples/GA/maxfctes/maxfctes/MaxFctESMain.cpp \
examples/GA/maxfctes/maxfctes/maxfctes.conf \
examples/GA/maxfctes/maxfctes/maxfctes-comma.conf \
examples/GA/maxfctes/maxfctes/maxfctes-plus.conf \
examples/GA/maxfctes/maxfctes/maxfctes-comma-non-breeder.conf \
examples/GA/maxfctes/maxfctes/maxfctes-plus-non-breeder.conf \
examples/GA/maxfctes/templates/cpp \
examples/GA/maxfctes/templates/hpp \
examples/GA/maxfctes/MSVCPP/maxfctes.sln \
examples/GA/maxfctes/MSVCPP/maxfctes.suo \
examples/GA/maxfctes/MSVCPP/maxfctes/maxfctes.vcproj \
examples/GA/maxfctfloat/acinclude.m4 \
examples/GA/maxfctfloat/aclocal.m4 \
examples/GA/maxfctfloat/bootstrap \
examples/GA/maxfctfloat/configure \
examples/GA/maxfctfloat/configure.ac \
examples/GA/maxfctfloat/Makefile.am \
examples/GA/maxfctfloat/Makefile.cvs \
examples/GA/maxfctfloat/Makefile.in \
examples/GA/maxfctfloat/AUTHORS \
examples/GA/maxfctfloat/COPYING \
examples/GA/maxfctfloat/INSTALL \
examples/GA/maxfctfloat/README \
examples/GA/maxfctfloat/maxfctfloat.kdevelop \
examples/GA/maxfctfloat/config/config.guess \
examples/GA/maxfctfloat/config/config.sub \
examples/GA/maxfctfloat/config/depcomp \
examples/GA/maxfctfloat/config/install-sh \
examples/GA/maxfctfloat/config/ltmain.sh \
examples/GA/maxfctfloat/config/missing \
examples/GA/maxfctfloat/maxfctfloat/Makefile.am \
examples/GA/maxfctfloat/maxfctfloat/Makefile.in \
examples/GA/maxfctfloat/maxfctfloat/config.hpp.in \
examples/GA/maxfctfloat/maxfctfloat/MaxFctFloatEvalOp.cpp \
examples/GA/maxfctfloat/maxfctfloat/MaxFctFloatEvalOp.hpp \
examples/GA/maxfctfloat/maxfctfloat/MaxFctFloatMain.cpp \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat.conf \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat-cmaes.conf \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat-cmaes-non-breeder.conf \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat-generational.conf \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat-sbx.conf \
examples/GA/maxfctfloat/maxfctfloat/maxfctfloat-steadystate.conf \
examples/GA/maxfctfloat/templates/cpp \
examples/GA/maxfctfloat/templates/hpp \
examples/GA/maxfctfloat/MSVCPP/maxfctfloat.sln \
examples/GA/maxfctfloat/MSVCPP/maxfctfloat.suo \
examples/GA/maxfctfloat/MSVCPP/maxfctfloat/maxfctfloat.vcproj \
examples/GA/onemax/acinclude.m4 \
examples/GA/onemax/aclocal.m4 \
examples/GA/onemax/bootstrap \
examples/GA/onemax/configure \
examples/GA/onemax/configure.ac \
examples/GA/onemax/Makefile.am \
examples/GA/onemax/Makefile.cvs \
examples/GA/onemax/Makefile.in \
examples/GA/onemax/AUTHORS \
examples/GA/onemax/COPYING \
examples/GA/onemax/INSTALL \
examples/GA/onemax/README \
examples/GA/onemax/onemax.kdevelop \
examples/GA/onemax/config/config.guess \
examples/GA/onemax/config/config.sub \
examples/GA/onemax/config/depcomp \
examples/GA/onemax/config/install-sh \
examples/GA/onemax/config/ltmain.sh \
examples/GA/onemax/config/missing \
examples/GA/onemax/onemax/Makefile.am \
examples/GA/onemax/onemax/Makefile.in \
examples/GA/onemax/onemax/config.hpp.in \
examples/GA/onemax/onemax/OneMaxEvalOp.cpp \
examples/GA/onemax/onemax/OneMaxEvalOp.hpp \
examples/GA/onemax/onemax/OneMaxMain.cpp \
examples/GA/onemax/onemax/onemax.conf \
examples/GA/onemax/onemax/onemax-generational.conf \
examples/GA/onemax/onemax/onemax-steadystate.conf \
examples/GA/onemax/templates/cpp \
examples/GA/onemax/templates/hpp \
examples/GA/onemax/MSVCPP/onemax.sln \
examples/GA/onemax/MSVCPP/onemax.suo \
examples/GA/onemax/MSVCPP/onemax/onemax.vcproj \
examples/GA/tsp/acinclude.m4 \
examples/GA/tsp/aclocal.m4 \
examples/GA/tsp/bootstrap \
examples/GA/tsp/configure \
examples/GA/tsp/configure.ac \
examples/GA/tsp/Makefile.am \
examples/GA/tsp/Makefile.cvs \
examples/GA/tsp/Makefile.in \
examples/GA/tsp/AUTHORS \
examples/GA/tsp/COPYING \
examples/GA/tsp/INSTALL \
examples/GA/tsp/README \
examples/GA/tsp/tsp.kdevelop \
examples/GA/tsp/config/config.guess \
examples/GA/tsp/config/config.sub \
examples/GA/tsp/config/depcomp \
examples/GA/tsp/config/install-sh \
examples/GA/tsp/config/ltmain.sh \
examples/GA/tsp/config/missing \
examples/GA/tsp/tsp/Makefile.am \
examples/GA/tsp/tsp/Makefile.in \
examples/GA/tsp/tsp/config.hpp.in \
examples/GA/tsp/tsp/TSPEvalOp.cpp \
examples/GA/tsp/tsp/TSPEvalOp.hpp \
examples/GA/tsp/tsp/TSPMain.cpp \
examples/GA/tsp/tsp/tsp.conf \
examples/GA/tsp/templates/cpp \
examples/GA/tsp/templates/hpp \
examples/GA/tsp/MSVCPP/tsp.sln \
examples/GA/tsp/MSVCPP/tsp.suo \
examples/GA/tsp/MSVCPP/tsp/tsp.vcproj \
examples/GA/zeromin/acinclude.m4 \
examples/GA/zeromin/aclocal.m4 \
examples/GA/zeromin/bootstrap \
examples/GA/zeromin/configure \
examples/GA/zeromin/configure.ac \
examples/GA/zeromin/Makefile.am \
examples/GA/zeromin/Makefile.cvs \
examples/GA/zeromin/Makefile.in \
examples/GA/zeromin/AUTHORS \
examples/GA/zeromin/COPYING \
examples/GA/zeromin/INSTALL \
examples/GA/zeromin/README \
examples/GA/zeromin/zeromin.kdevelop \
examples/GA/zeromin/config/config.guess \
examples/GA/zeromin/config/config.sub \
examples/GA/zeromin/config/depcomp \
examples/GA/zeromin/config/install-sh \
examples/GA/zeromin/config/ltmain.sh \
examples/GA/zeromin/config/missing \
examples/GA/zeromin/templates/cpp \
examples/GA/zeromin/templates/hpp \
examples/GA/zeromin/zeromin/Makefile.am \
examples/GA/zeromin/zeromin/Makefile.in \
examples/GA/zeromin/zeromin/config.hpp.in \
examples/GA/zeromin/zeromin/ZeroMinEvalOp.cpp \
examples/GA/zeromin/zeromin/ZeroMinEvalOp.hpp \
examples/GA/zeromin/zeromin/ZeroMinMain.cpp \
examples/GA/zeromin/zeromin/zeromin.conf \
examples/GA/zeromin/zeromin/zeromin-generational.conf \
examples/GA/zeromin/zeromin/zeromin-steadystate.conf \
examples/GA/zeromin/MSVCPP/zeromin.sln \
examples/GA/zeromin/MSVCPP/zeromin.suo \
examples/GA/zeromin/MSVCPP/zeromin/zeromin.vcproj \
examples/GP/symbreg/acinclude.m4 \
examples/GP/symbreg/aclocal.m4 \
examples/GP/symbreg/bootstrap \
examples/GP/symbreg/configure \
examples/GP/symbreg/configure.ac \
examples/GP/symbreg/Makefile.am \
examples/GP/symbreg/Makefile.cvs \
examples/GP/symbreg/Makefile.in \
examples/GP/symbreg/AUTHORS \
examples/GP/symbreg/COPYING \
examples/GP/symbreg/INSTALL \
examples/GP/symbreg/README \
examples/GP/symbreg/symbreg.kdevelop \
examples/GP/symbreg/config/config.guess \
examples/GP/symbreg/config/config.sub \
examples/GP/symbreg/config/depcomp \
examples/GP/symbreg/config/install-sh \
examples/GP/symbreg/config/ltmain.sh \
examples/GP/symbreg/config/missing \
examples/GP/symbreg/MSVCPP/symbreg.sln \
examples/GP/symbreg/MSVCPP/symbreg.suo \
examples/GP/symbreg/MSVCPP/symbreg/symbreg.vcproj \
examples/GP/symbreg/symbreg/Makefile.am \
examples/GP/symbreg/symbreg/Makefile.in \
examples/GP/symbreg/symbreg/config.hpp.in \
examples/GP/symbreg/symbreg/SymbRegEvalOp.cpp \
examples/GP/symbreg/symbreg/SymbRegEvalOp.hpp \
examples/GP/symbreg/symbreg/SymbRegMain.cpp \
examples/GP/symbreg/symbreg/symbreg.conf \
examples/GP/symbreg/symbreg/symbreg-generational.conf \
examples/GP/symbreg/symbreg/symbreg-steadystate.conf \
examples/GP/symbreg/symbreg/symbreg-adfs.conf \
examples/GP/symbreg/symbreg/symbreg-ema.conf \
examples/GP/symbreg/symbreg/symbreg-hfc.conf \
examples/GP/symbreg/templates/cpp \
examples/GP/symbreg/templates/hpp \
examples/GP/parity/acinclude.m4 \
examples/GP/parity/aclocal.m4 \
examples/GP/parity/bootstrap \
examples/GP/parity/configure \
examples/GP/parity/configure.ac \
examples/GP/parity/Makefile.am \
examples/GP/parity/Makefile.cvs \
examples/GP/parity/Makefile.in \
examples/GP/parity/AUTHORS \
examples/GP/parity/COPYING \
examples/GP/parity/INSTALL \
examples/GP/parity/README \
examples/GP/parity/parity.kdevelop \
examples/GP/parity/config/config.guess \
examples/GP/parity/config/config.sub \
examples/GP/parity/config/depcomp \
examples/GP/parity/config/install-sh \
examples/GP/parity/config/ltmain.sh \
examples/GP/parity/config/missing \
examples/GP/parity/MSVCPP/parity.sln \
examples/GP/parity/MSVCPP/parity.suo \
examples/GP/parity/MSVCPP/parity/parity.vcproj \
examples/GP/parity/parity/Makefile.am \
examples/GP/parity/parity/Makefile.in \
examples/GP/parity/parity/config.hpp.in \
examples/GP/parity/parity/ParityEvalOp.cpp \
examples/GP/parity/parity/ParityEvalOp.hpp \
examples/GP/parity/parity/ParityMain.cpp \
examples/GP/parity/parity/parity.conf \
examples/GP/parity/parity/parity-generational.conf \
examples/GP/parity/parity/parity-steadystate.conf \
examples/GP/parity/parity/parity-elitism.conf \
examples/GP/parity/parity/parity-adfs.conf \
examples/GP/parity/parity/parity-ema.conf \
examples/GP/parity/templates/cpp \
examples/GP/parity/templates/hpp \
examples/GP/spambase/acinclude.m4 \
examples/GP/spambase/aclocal.m4 \
examples/GP/spambase/bootstrap \
examples/GP/spambase/configure \
examples/GP/spambase/configure.ac \
examples/GP/spambase/Makefile.am \
examples/GP/spambase/Makefile.cvs \
examples/GP/spambase/Makefile.in \
examples/GP/spambase/AUTHORS \
examples/GP/spambase/COPYING \
examples/GP/spambase/INSTALL \
examples/GP/spambase/README \
examples/GP/spambase/spambase.kdevelop \
examples/GP/spambase/config/config.guess \
examples/GP/spambase/config/config.sub \
examples/GP/spambase/config/depcomp \
examples/GP/spambase/config/install-sh \
examples/GP/spambase/config/ltmain.sh \
examples/GP/spambase/config/missing \
examples/GP/spambase/MSVCPP/spambase.sln \
examples/GP/spambase/MSVCPP/spambase.suo \
examples/GP/spambase/MSVCPP/spambase/spambase.vcproj \
examples/GP/spambase/spambase/Makefile.am \
examples/GP/spambase/spambase/Makefile.in \
examples/GP/spambase/spambase/config.hpp.in \
examples/GP/spambase/spambase/EphemeralPercent.cpp \
examples/GP/spambase/spambase/EphemeralPercent.hpp \
examples/GP/spambase/spambase/EqualTo.cpp \
examples/GP/spambase/spambase/EqualTo.hpp \
examples/GP/spambase/spambase/IfThenElse.cpp \
examples/GP/spambase/spambase/IfThenElse.hpp \
examples/GP/spambase/spambase/LessThan.cpp \
examples/GP/spambase/spambase/LessThan.hpp \
examples/GP/spambase/spambase/SpambaseEvalOp.cpp \
examples/GP/spambase/spambase/SpambaseEvalOp.hpp \
examples/GP/spambase/spambase/SpambaseMain.cpp \
examples/GP/spambase/spambase/spambase.conf \
examples/GP/spambase/spambase/spambase-generational.conf \
examples/GP/spambase/spambase/spambase-steadystate.conf \
examples/GP/spambase/spambase/spambase.cldist \
examples/GP/spambase/spambase/spambase.data \
examples/GP/spambase/spambase/spambase.info \
examples/GP/spambase/spambase/spambase.names \
examples/GP/spambase/templates/cpp \
examples/GP/spambase/templates/hpp \
m4/ac_cxx_bool.m4 \
m4/ac_cxx_complex_math_in_namespace_std.m4 \
m4/ac_cxx_const_cast.m4 \
m4/ac_cxx_default_template_parameters.m4 \
m4/ac_cxx_dynamic_cast.m4 \
m4/ac_cxx_exceptions.m4 \
m4/ac_cxx_explicit.m4 \
m4/ac_cxx_have_ext_hash_map.m4 \
m4/ac_cxx_have_ext_hash_set.m4 \
m4/ac_cxx_have_numeric_limits.m4 \
m4/ac_cxx_have_sstream.m4 \
m4/ac_cxx_have_std.m4 \
m4/ac_cxx_have_stl.m4 \
m4/ac_cxx_namespaces.m4 \
m4/ac_cxx_new_for_scoping.m4 \
m4/ac_cxx_reinterpret_cast.m4 \
m4/ac_cxx_rtti.m4 \
m4/ac_cxx_static_cast.m4 \
m4/ac_cxx_template_qualified_base_class.m4 \
m4/ac_cxx_template_qualified_return_type.m4 \
m4/ac_cxx_template_scoped_argument_matching.m4 \
m4/ac_cxx_templates.m4 \
m4/ac_cxx_typename.m4 \
m4/acx_pthread.m4 \
m4/ax_prefix_config_h.m4 \
m4/check_zlib.m4 \
m4/ob_cxx_have_isinfinite.m4 \
m4/ob_cxx_long_long.m4 \
m4/ob_have_hash_map.m4 \
m4/ob_have_hash_set.m4 \
MSVCPP/beagle.sln \
MSVCPP/beagle.suo \
MSVCPP/beagle/beagle.vcproj \
MSVCPP/beagle-GA/beagle-GA.vcproj \
MSVCPP/beagle-GP/beagle-GP.vcproj \
MSVCPP/beagle-Coev/beagle-Coev.vcproj \
MSVCPP/pacc-util/pacc-util.vcproj \
MSVCPP/pacc-xml/pacc-xml.vcproj \
MSVCPP/pacc-math/pacc-math.vcproj \
MSVCPP/pacc-threading/pacc-threading.vcproj \
MSVCPP/pacc-socket/pacc-socket.vcproj \
MSVCPP/pacc-svg/pacc-svg.vcproj \
PACC/Doxyfile \
templates/cpp \
templates/hpp


