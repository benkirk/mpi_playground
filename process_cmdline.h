#ifndef PROCESS_CMDLINE_H
#define PROCESS_CMDLINE_H

#include <set>

namespace MPI_Playground {

  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep);
}

#endif
