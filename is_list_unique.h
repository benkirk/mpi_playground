#ifndef __is_list_unique_h__
#define __is_list_unique_h__

#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <random>
#include <mpi.h>
#ifdef HAVE_CONFIG_H
# include "mpi_play_config.h"
#endif


namespace MPI_Playground
{
  template <typename T> inline void append_random_list (std::vector<T> &l,
                                                        const std::size_t nentries,
                                                        const T minval=0,
                                                        const T maxval=std::numeric_limits<T>::max())
  {
    // https://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c/32887614

    static std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
    static std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator
                                      // and seed it with entropy.

    // Define the distribution, by default it goes from 0 to MAX(unsigned long long)
    // or what have you.
    std::uniform_int_distribution<T> distr(minval, maxval);


    //l.clear();
    l.reserve(l.size() + nentries);

    for (auto i=0; i<nentries; i++)
      l.push_back(distr(eng));

    return;
  }
}


#endif // #define __is_list_unique_h__
