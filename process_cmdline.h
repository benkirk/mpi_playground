#ifndef PROCESS_CMDLINE_H
#define PROCESS_CMDLINE_H

#include <set>

namespace MPI_Playground {

  namespace Options
  {
    extern unsigned int nrep;
    extern bool write;
    extern bool read;
  };


  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep);

  inline void process_command_line (const int argc, char** argv)
  {
    process_command_line(argc, argv, Options::nrep);
  }

}

#endif
