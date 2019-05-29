ifeq (x$(PYMPIRUN),x)
  PYMPIRUN := mpirun
endif

ifeq (x$(NPROCS),x)
  NPROCS := -np 6
endif



run:
	echo "Launching tasks with \"$(PYMPIRUN) $(NPROCS)\""
	$(MAKE) tests

tests: $(wildcard test_*.py)
	for file in $^; do \
	  echo "#########################################" ; \
	  echo $$file: ; \
	  $(PYMPIRUN) $(NPROCS) python $$file ; \
	done
