#ifndef PROCESS_CMDLINE_H
#define PROCESS_CMDLINE_H

#include <set>

namespace MPI_Playground {

  class Options
  {
  public:
    unsigned int nrep;
    bool write;
    bool read;
    bool do_xdr;
    bool do_c;
    bool do_h5;
    unsigned int nbytes;
    unsigned int bufsize;

    Options ()
    {
      //default values
      nrep = 100;
      write = true;
      read  = true;
      do_xdr = true;
      do_c = true;
      do_h5 = true;
      nbytes = 2e9;
      bufsize = nbytes / sizeof(double);
    }

  };

  extern Options opts;

  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep);

  inline void process_command_line (const int argc, char** argv)
  {
    process_command_line(argc, argv, opts.nrep);
  }

}

#endif
