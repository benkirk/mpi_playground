ifeq (x$(PYMPIRUN),x)
  PYMPIRUN := mpirun
endif

ifeq (x$(NPROCS),x)
  NPROCS := -np 6
endif



run:
	echo "Launching tasks with \"$(PYMPIRUN) $(NPROCS)\""
	$(MAKE) osu_pt2pt
	$(MAKE) osu_collective
	$(MAKE) io



osu_pt2pt: osu_bw.py osu_bibw.py osu_latency.py osu_multi_lat.py
	for file in $^; do \
	  echo "#########################################" ; \
	  echo $$file: ; \
	  $(PYMPIRUN) -np 2 python $$file ; \
	done



osu_collective: osu_barrier.py osu_gather.py osu_allgather.py osu_scatter.py osu_bcast.py osu_alltoall.py osu_alltoallv.py
	for file in $^; do \
	  echo "#########################################" ; \
	  echo $$file: ; \
	  $(PYMPIRUN) $(NPROCS) python $$file ; \
	done

io: $(wildcard mpi_io_*.py)
	for file in $^; do \
	  echo "################################################################################" ; \
	  echo $$file: ; \
	  $(PYMPIRUN) $(NPROCS) python $$file ; \
	done


clean:
	rm -f *.contig mc_*.*
