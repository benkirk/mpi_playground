// C++ include files that we need
#include <iostream>
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"

// Bring in everything from the libMesh namespace
using namespace libMesh;

int main (int argc, char ** argv)
{
  // Initialize the library.  This is necessary because the library
  // may depend on a number of other libraries (i.e. MPI and PETSc)
  // that require initialization before use.  When the LibMeshInit
  // object goes out of scope, other libraries and resources are
  // finalized.
  LibMeshInit init (argc, argv);

  // Create a mesh, with dimension to be overridden later, on the
  // default MPI communicator.
  Mesh mesh(init.comm());

  // Print information about the mesh to the screen.
  mesh.print_info();

  libMesh::err << "Hello from rank "
               << init.comm().rank()
               << std::endl;

  // All done.  libMesh objects are destroyed here.  Because the
  // LibMeshInit object was created first, its destruction occurs
  // last, and it's destructor finalizes any external libraries and
  // checks for leaked memory.
  return 0;
}
