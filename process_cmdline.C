#include "process_cmdline.h"
#include <getopt.h>
#include <cstdlib>
#include <iostream>

namespace MPI_Playground {

  void process_command_line (const int argc, char** argv,
			     unsigned int &nrep)
  {
    // default values
    nrep = 100;

    char optionStr[] =
      "n:k:h?";

    int opt;
    while ((opt = getopt(argc, argv, optionStr)) != -1)
      {
	switch (opt)
	  {
	  case 'n':
	    nrep = std::atoi(optarg);
	    break;
	  }
      }
  }
}
