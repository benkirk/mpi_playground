#include "process_cmdline.h"
#include <getopt.h>
#include <cstdlib>
#include <iostream>

namespace MPI_Playground {

  Options opts;

  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep)
  {
    // default values
    nrep = opts.nrep;

    char optionStr[] =
      "n:S:rwRWxcHF?";

    int opt;
    while ((opt = getopt(argc, argv, optionStr)) != -1)
      {
	switch (opt)
	  {
	  case 'n':
            opts.nrep = nrep = std::atoi(optarg);
	    break;

          case 'r':
            opts.read = true;
            break;

          case 'R':
            opts.write = false;
            break;

          case 'w':
            opts.write = true;
            break;

          case 'W':
            opts.read = false;
            break;

          case 'x':
            opts.do_xdr = !opts.do_xdr;
            break;

          case 'c':
            opts.do_c = !opts.do_c;
            break;

          case 'H':
            opts.do_h5 = !opts.do_h5;
            break;

          case 'F':
            opts.do_fortran = !opts.do_fortran;
            break;

          case 'S':
            opts.nbytes  = static_cast<unsigned int>(std::atof(optarg));
            opts.bufsize = opts.nbytes / sizeof(double);
            break;
	  }
      }
  }
}
