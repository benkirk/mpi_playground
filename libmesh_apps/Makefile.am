bin_PROGRAMS =

AM_CPPFLAGS = $(LIBMESH_CXXFLAGS)
LIBS = $(LIBMESH_LIBS)

# Only build here if we have libmesh
if ENABLE_LIBMESH

  # 'hello' world program
  bin_PROGRAMS += hello
  hello_SOURCES = src/hello.C

  # ex4 Poisson problem
  bin_PROGRAMS   += poisson
  poisson_SOURCES = src/poisson/poisson.C src/poisson/exact_solution.C
endif
