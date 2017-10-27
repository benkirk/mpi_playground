#include "process_cmdline.h"
#include <getopt.h>
#include <cstdlib>
#include <iostream>

namespace MPI_Playground {

  //default values
  namespace Options {
    unsigned int nrep = 100;
    bool write = true;
    bool read  = true;
  }


  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep)
  {
    // default values
    nrep = Options::nrep;

    char optionStr[] =
      "n:rwRWh?";

    int opt;
    while ((opt = getopt(argc, argv, optionStr)) != -1)
      {
	switch (opt)
	  {
	  case 'n':
            Options::nrep = nrep = std::atoi(optarg);
	    break;

          case 'r':
            Options::read = true;
            break;

          case 'R':
            Options::write = false;
            break;

          case 'w':
            Options::write = true;
            break;

          case 'W':
            Options::read = false;
            break;

	  }
      }
  }
}
