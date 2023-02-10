BUILT_SOURCES =
CLEANFILES =

if VPATH_BUILD
linkstamp:
	$(AM_V_GEN)[ -w ] && rm -rf ./imb ; \
	[ -x $(RSYNC) ] && $(RSYNC) -axq $(top_srcdir)/imb/imb . --exclude="WINDOWS/" --include="*/" --exclude="*" >/dev/null ; \
	for dir in $$(find imb/ -type d); do \
	  echo $$dir ; \
	  cd $(abs_builddir)/$$dir ; pwd ; \
          for file in $$(find $(abs_srcdir)/$$dir -maxdepth 1 -type f); do \
            echo $$file ; \
            $(LN_S) -f $$file . || continue ; \
          done; \
	done
	touch $@

BUILT_SOURCES += linkstamp
CLEANFILES += $(BUILT_SOURCES)

distclean-local:
	rm -rf imb || echo "Cannot remove imb"
endif


all-local:
	cd imb && $(MAKE) V=0 $(AM_MAKEFLAGS) all
check-local:
	cd imb && $(MAKE) $(AM_MAKEFLAGS) test
clean-local:
	cd imb && $(MAKE) $(AM_MAKEFLAGS) clean || echo "Cannot make clean"

# Assuming the package knows how to install itself
install-exec-local:
	$(MKDIR_P) $(DESTDIR)$(bindir)
	$(INSTALL) imb/IMB-* $(DESTDIR)$(bindir)

uninstall-local:
	cd imb && $(MAKE) $(AM_MAKEFLAGS) uninstall

# Distribute files from here.
#EXTRA_DIST = subdir/Makefile subdir/program.c ...

# for file in $(find imb -type f | sort | egrep -v ".git|WINDOWS") ; do echo "   $file \\"; done
EXTRA_DIST = \
   imb/Makefile \
   imb/Makefile_win \
   imb/README.md \
   imb/ReadMe_IMB.txt \
   imb/license/license.txt \
   imb/license/third-party-programs.txt \
   imb/license/use-of-trademark-license.txt \
   imb/src_c/IMB_2018.c \
   imb/src_c/IMB_allgather.c \
   imb/src_c/IMB_allgatherv.c \
   imb/src_c/IMB_allreduce.c \
   imb/src_c/IMB_alltoall.c \
   imb/src_c/IMB_alltoallv.c \
   imb/src_c/IMB_appl_errors.h \
   imb/src_c/IMB_bandwidth.c \
   imb/src_c/IMB_barrier.c \
   imb/src_c/IMB_bcast.c \
   imb/src_c/IMB_benchlist.c \
   imb/src_c/IMB_benchmark.h \
   imb/src_c/IMB_bnames_ext.h \
   imb/src_c/IMB_bnames_io.h \
   imb/src_c/IMB_bnames_mpi1.h \
   imb/src_c/IMB_bnames_nbc.h \
   imb/src_c/IMB_bnames_rma.h \
   imb/src_c/IMB_cache.h \
   imb/src_c/IMB_chk_diff.c \
   imb/src_c/IMB_comm_info.h \
   imb/src_c/IMB_comments.h \
   imb/src_c/IMB_cpu_exploit.c \
   imb/src_c/IMB_declare.c \
   imb/src_c/IMB_declare.h \
   imb/src_c/IMB_err_check.h \
   imb/src_c/IMB_err_handler.c \
   imb/src_c/IMB_exchange.c \
   imb/src_c/IMB_g_info.c \
   imb/src_c/IMB_gather.c \
   imb/src_c/IMB_gatherv.c \
   imb/src_c/IMB_init.c \
   imb/src_c/IMB_init_file.c \
   imb/src_c/IMB_init_transfer.c \
   imb/src_c/IMB_input.txt \
   imb/src_c/IMB_l0.c \
   imb/src_c/IMB_l0.h \
   imb/src_c/IMB_mem_info.h \
   imb/src_c/IMB_mem_manager.c \
   imb/src_c/IMB_ones_accu.c \
   imb/src_c/IMB_ones_bidir.c \
   imb/src_c/IMB_ones_unidir.c \
   imb/src_c/IMB_open_close.c \
   imb/src_c/IMB_output.c \
   imb/src_c/IMB_parse_name_ext.c \
   imb/src_c/IMB_parse_name_io.c \
   imb/src_c/IMB_parse_name_mpi1.c \
   imb/src_c/IMB_parse_name_nbc.c \
   imb/src_c/IMB_parse_name_rma.c \
   imb/src_c/IMB_pingping.c \
   imb/src_c/IMB_pingpong.c \
   imb/src_c/IMB_prototypes.h \
   imb/src_c/IMB_read.c \
   imb/src_c/IMB_reduce.c \
   imb/src_c/IMB_reduce_local.c \
   imb/src_c/IMB_reduce_scatter.c \
   imb/src_c/IMB_reduce_scatter_block.c \
   imb/src_c/IMB_rma_atomic.c \
   imb/src_c/IMB_rma_get.c \
   imb/src_c/IMB_rma_put.c \
   imb/src_c/IMB_scatter.c \
   imb/src_c/IMB_scatterv.c \
   imb/src_c/IMB_sendrecv.c \
   imb/src_c/IMB_settings.h \
   imb/src_c/IMB_settings_io.h \
   imb/src_c/IMB_strgs.c \
   imb/src_c/IMB_user_set_info.c \
   imb/src_c/IMB_utils.c \
   imb/src_c/IMB_warm_up.c \
   imb/src_c/IMB_window.c \
   imb/src_c/IMB_write.c \
   imb/src_c/Makefile \
   imb/src_c/Makefile_win \
   imb/src_c/P2P/Makefile \
   imb/src_c/P2P/imb_p2p.c \
   imb/src_c/P2P/imb_p2p.h \
   imb/src_c/P2P/imb_p2p_birandom.c \
   imb/src_c/P2P/imb_p2p_corandom.c \
   imb/src_c/P2P/imb_p2p_pingping.c \
   imb/src_c/P2P/imb_p2p_pingpong.c \
   imb/src_c/P2P/imb_p2p_sendrecv_replace.c \
   imb/src_c/P2P/imb_p2p_stencil2d.c \
   imb/src_c/P2P/imb_p2p_stencil3d.c \
   imb/src_c/P2P/imb_p2p_unirandom.c \
   imb/src_cpp/EXT/EXT_benchmark.cpp \
   imb/src_cpp/EXT/EXT_suite.cpp \
   imb/src_cpp/EXT/Makefile.EXT.mk \
   imb/src_cpp/EXT/Makefile_win.EXT.mk \
   imb/src_cpp/HALO/Makefile.HALO.mk \
   imb/src_cpp/HALO/halo_benchmark.cpp \
   imb/src_cpp/HALO/halo_benchmark.h \
   imb/src_cpp/HALO/halo_suite.cpp \
   imb/src_cpp/IO/IO_benchmark.cpp \
   imb/src_cpp/IO/IO_suite.cpp \
   imb/src_cpp/IO/Makefile.IO.mk \
   imb/src_cpp/IO/Makefile_win.IO.mk \
   imb/src_cpp/MPI1/MPI1_benchmark.cpp \
   imb/src_cpp/MPI1/MPI1_suite.cpp \
   imb/src_cpp/MPI1/Makefile.MPI1.mk \
   imb/src_cpp/MPI1/Makefile_win.MPI1.mk \
   imb/src_cpp/MT/MT_benchmark.cpp \
   imb/src_cpp/MT/MT_benchmark.h \
   imb/src_cpp/MT/MT_suite.cpp \
   imb/src_cpp/MT/MT_types.h \
   imb/src_cpp/MT/Makefile.MT.mk \
   imb/src_cpp/MT/Makefile_win.MT.mk \
   imb/src_cpp/Makefile \
   imb/src_cpp/Makefile_win \
   imb/src_cpp/NBC/Makefile.NBC.mk \
   imb/src_cpp/NBC/Makefile_win.NBC.mk \
   imb/src_cpp/NBC/NBC_benchmark.cpp \
   imb/src_cpp/NBC/NBC_suite.cpp \
   imb/src_cpp/RMA/Makefile.RMA.mk \
   imb/src_cpp/RMA/Makefile_win.RMA.mk \
   imb/src_cpp/RMA/RMA_benchmark.cpp \
   imb/src_cpp/RMA/RMA_suite.cpp \
   imb/src_cpp/any.h \
   imb/src_cpp/args_parser.cpp \
   imb/src_cpp/args_parser.h \
   imb/src_cpp/args_parser_utests.cpp \
   imb/src_cpp/benchmark.h \
   imb/src_cpp/benchmark_suite.h \
   imb/src_cpp/benchmark_suite_base.h \
   imb/src_cpp/benchmark_suites_collection.cpp \
   imb/src_cpp/benchmark_suites_collection.h \
   imb/src_cpp/example/Makefile.example.mk \
   imb/src_cpp/example/example_benchmark1.cpp \
   imb/src_cpp/example/example_benchmark2.cpp \
   imb/src_cpp/example/example_benchmark3.cpp \
   imb/src_cpp/example/example_benchmark4.cpp \
   imb/src_cpp/example/example_benchmark5.cpp \
   imb/src_cpp/helpers/Makefile.helpers.mk \
   imb/src_cpp/helpers/Makefile_win.helpers.mk \
   imb/src_cpp/helpers/helper_IMB_functions.h \
   imb/src_cpp/helpers/original_benchmark.h \
   imb/src_cpp/imb.cpp \
   imb/src_cpp/scope.cpp \
   imb/src_cpp/scope.h \
   imb/src_cpp/smart_ptr.h \
   imb/src_cpp/utils.h
