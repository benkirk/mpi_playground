
AM_CPPFLAGS = -I$(srcdir)/../mpl
AM_CFLAGS = -Wall

bin_PROGRAMS =
bin_PROGRAMS += \
   arrays \
   blocking \
   blocking_vector \
   collective \
   communicator \
   distributed_grid \
   distributed_grid_scatter_gather \
   gather \
   gatherv \
   heat_equation_Jacobi_method \
   heat_equation_successive_over_relaxation \
   hello_world \
   intercommunicator \
   iterators \
   layouts \
   matrix_gather \
   nonblocking \
   nonblocking_mult \
   parallel_sort_mpl \
   parallel_sort_mpi \
   probe \
   reduce_lcm \
   reduce_min_loc \
   standard_types \
   stl_container \
   struct \
   subarray \
   vibrating_string_mpl

arrays_SOURCES = $(top_srcdir)/mpl/examples/arrays.cc

blocking_SOURCES = $(top_srcdir)/mpl/examples/blocking.cc

blocking_vector_SOURCES = $(top_srcdir)/mpl/examples/blocking_vector.cc

collective_SOURCES = $(top_srcdir)/mpl/examples/collective.cc

communicator_SOURCES = $(top_srcdir)/mpl/examples/communicator.cc

distributed_grid_SOURCES = $(top_srcdir)/mpl/examples/distributed_grid.cc

distributed_grid_scatter_gather_SOURCES = $(top_srcdir)/mpl/examples/distributed_grid_scatter_gather.cc

gather_SOURCES = $(top_srcdir)/mpl/examples/gather.cc

gatherv_SOURCES = $(top_srcdir)/mpl/examples/gatherv.cc

heat_equation_Jacobi_method_SOURCES = $(top_srcdir)/mpl/examples/heat_equation_Jacobi_method.cc

heat_equation_successive_over_relaxation_SOURCES = $(top_srcdir)/mpl/examples/heat_equation_successive_over-relaxation.cc

hello_world_SOURCES = $(top_srcdir)/mpl/examples/hello_world.cc

intercommunicator_SOURCES = $(top_srcdir)/mpl/examples/intercommunicator.cc

iterators_SOURCES = $(top_srcdir)/mpl/examples/iterators.cc

layouts_SOURCES = $(top_srcdir)/mpl/examples/layouts.cc

matrix_gather_SOURCES = $(top_srcdir)/mpl/examples/matrix_gather.cc

nonblocking_SOURCES = $(top_srcdir)/mpl/examples/nonblocking.cc

nonblocking_mult_SOURCES = $(top_srcdir)/mpl/examples/nonblocking_mult.cc

parallel_sort_mpl_SOURCES = $(top_srcdir)/mpl/examples/parallel_sort_mpl.cc

parallel_sort_mpi_SOURCES = $(top_srcdir)/mpl/examples/parallel_sort_mpi.c

probe_SOURCES = $(top_srcdir)/mpl/examples/probe.cc

reduce_lcm_SOURCES = $(top_srcdir)/mpl/examples/reduce_lcm.cc

reduce_min_loc_SOURCES = $(top_srcdir)/mpl/examples/reduce_min_loc.cc

standard_types_SOURCES = $(top_srcdir)/mpl/examples/standard_types.cc

stl_container_SOURCES = $(top_srcdir)/mpl/examples/stl_container.cc

struct_SOURCES = $(top_srcdir)/mpl/examples/struct.cc

subarray_SOURCES = $(top_srcdir)/mpl/examples/subarray.cc

vibrating_string_mpl_SOURCES = $(top_srcdir)/mpl/examples/vibrating_string_mpl.cc

# where to install these headers
includedir=$(prefix)/include/mpl
mpl_headers = \
   $(top_srcdir)/mpl/mpl/cartesian_communicator.hpp \
   $(top_srcdir)/mpl/mpl/comm_group.hpp \
   $(top_srcdir)/mpl/mpl/datatype.hpp \
   $(top_srcdir)/mpl/mpl/displacements.hpp \
   $(top_srcdir)/mpl/mpl/distributed_graph_communicator.hpp \
   $(top_srcdir)/mpl/mpl/distributed_grid.hpp \
   $(top_srcdir)/mpl/mpl/environment.hpp \
   $(top_srcdir)/mpl/mpl/error.hpp \
   $(top_srcdir)/mpl/mpl/flat_memory.hpp \
   $(top_srcdir)/mpl/mpl/graph_communicator.hpp \
   $(top_srcdir)/mpl/mpl/layout.hpp \
   $(top_srcdir)/mpl/mpl/message.hpp \
   $(top_srcdir)/mpl/mpl/mpl.hpp \
   $(top_srcdir)/mpl/mpl/operator.hpp \
   $(top_srcdir)/mpl/mpl/ranks.hpp \
   $(top_srcdir)/mpl/mpl/request.hpp \
   $(top_srcdir)/mpl/mpl/status.hpp \
   $(top_srcdir)/mpl/mpl/tag.hpp \
   $(top_srcdir)/mpl/mpl/topology_communicator.hpp \
   $(top_srcdir)/mpl/mpl/utility.hpp \
   $(top_srcdir)/mpl/mpl/vector.hpp

include_HEADERS = $(mpl_headers)
