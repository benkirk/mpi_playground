# bin_PROGRAMS =

CLEANFILES    = $(top_builddir)/collective_data.contig
BUILT_SOURCES =
bin_SCRIPTS   =

python_PYTHON =

python_PYTHON += nonblocking_consensus.py

python_PYTHON += mpi_io_collective_write.py mpi_io_collective_read.py

python_PYTHON += mpi_io_master_slave_write.py mpi_io_master_slave_read.py


#------------------------------------------------------------------------------
mpi_io_collective_write: py_wrapper
	$(AM_V_GEN)cp $< $@

BUILT_SOURCES += mpi_io_collective_write
bin_SCRIPTS   += mpi_io_collective_write

#------------------------------------------------------------------------------
mpi_io_collective_read: py_wrapper
	$(AM_V_GEN)cp $< $@

BUILT_SOURCES += mpi_io_collective_read
bin_SCRIPTS   += mpi_io_collective_read



CLEANFILES += $(BUILT_SOURCES)
