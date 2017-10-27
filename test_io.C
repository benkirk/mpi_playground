#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <iostream>
#include <vector>
#include <rpc/rpc.h>
#include <cstddef>
#include <stdio.h>
#include <numeric>
#include <boost/timer.hpp>
#include <mpi.h>
#include <process_cmdline.h>


namespace
{
  static const std::size_t BUFSIZE = 2e8;
  int rank, nprocs;
  std::string io_basename = "test.";
}



template <typename T>
void init_vector (std::vector<T> &data)
{
  boost::timer t;
  data.resize(BUFSIZE);

  if (!MPI_Playground::Options::write)
    {
      std::cout << "Proc " << rank << " "
            << "Allocated "
            << BUFSIZE
            << " values in "
            << t.elapsed()
            << " seconds\n";
      return;
    }

  std::iota(data.begin(), data.end(), static_cast<T>(rank));

  std::cout << "Proc " << rank << " "
            << "Initialized "
            << BUFSIZE
            << " values in "
            << t.elapsed()
            << " seconds\n";
}



void write_xdr (std::vector<double> &data)
{
  boost::timer t;

  XDR *xdrs = new XDR;
  std::string fname = io_basename + ".xdr";
  FILE  *fp = fopen(fname.c_str(), "w");

  xdrstdio_create (xdrs, fp, XDR_ENCODE);

  xdr_vector(xdrs,
	     (char*) &data[0],
	     data.size(),
	     sizeof(double),
	     (xdrproc_t) xdr_double);

  xdr_destroy (xdrs);
  delete xdrs;

  fflush (fp);
  fclose (fp);

  std::cout << "Proc " << rank << " "
            << "XDR: Wrote " << data.size()*sizeof(double)/1.e6
	    << "MB in " << t.elapsed() << " seconds\n";
}



void read_xdr (std::vector<double> &data)
{
  boost::timer t;

  XDR *xdrs = new XDR;
  std::string fname = io_basename + ".xdr";
  FILE  *fp = fopen(fname.c_str(), "r");

  xdrstdio_create (xdrs, fp, XDR_DECODE);

  xdr_vector(xdrs,
	     (char*) &data[0],
	     data.size(),
	     sizeof(double),
	     (xdrproc_t) xdr_double);

  xdr_destroy (xdrs);
  delete xdrs;

  fflush (fp);
  fclose (fp);

  std::cout << "Proc " << rank << " "
            << "XDR: Read  " << data.size()*sizeof(double)/1.e6
	    << "MB in " << t.elapsed() << " seconds\n";
}



void write_c (std::vector<double> &data)
{
  boost::timer t;

  std::string fname = io_basename + ".bin";
  FILE *fp = fopen(fname.c_str(), "w");

  fwrite (data.empty() ? NULL : &data[0],
          sizeof(double),
          data.size(),
          fp);

  fflush (fp);
  fclose (fp);

  std::cout << "Proc " << rank << " "
            << "C: Wrote " << data.size()*sizeof(double)/1.e6
	    << "MB " << t.elapsed() << " seconds\n";
}



void read_c (std::vector<double> &data)
{
  boost::timer t;

  std::string fname = io_basename + ".bin";
  FILE *fp = fopen(fname.c_str(), "r");

  fread (data.empty() ? NULL : &data[0],
         sizeof(double),
         data.size(),
         fp);

  fflush (fp);
  fclose (fp);

  std::cout << "Proc " << rank << " "
            << "C: Read  " << data.size()*sizeof(double)/1.e6
	    << "MB " << t.elapsed() << " seconds\n";
}



int main (int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  MPI_Playground::process_command_line (argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  const bool print_aggregate = (rank == 0) && (nprocs > 1);

  io_basename += std::string(std::to_string(rank));

  {
    boost::timer t;
    std::vector<double> data;
    init_vector(data);


    if (MPI_Playground::Options::write)
      {
        {
          t.restart();
          write_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

          if (print_aggregate)
            std::cout << std::string(80, '-') << '\n'
                      << "--> Aggregate XDR write bw "
                      << nprocs*data.size()*sizeof(double)/1.e6/t.elapsed()
                      << " MB/s\n";
        }
        {
          t.restart();
          write_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

          if (print_aggregate)
            std::cout << std::string(80, '-') << '\n'
                      << "--> Aggregate C write bw "
                      << nprocs*data.size()*sizeof(double)/1.e6/t.elapsed()
                      << " MB/s\n";
        }
      }

    if (MPI_Playground::Options::read)
      {
        {
          t.restart();
          read_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

          if (print_aggregate)
            std::cout << std::string(80, '-') << '\n'
                      << "--> Aggregate XDR read bw "
                      << nprocs*data.size()*sizeof(double)/1.e6/t.elapsed()
                      << " MB/s\n";
        }
        {
          t.restart();
          read_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

          if (print_aggregate)
            std::cout << std::string(80, '-') << '\n'
                      << "--> Aggregate C read bw "
                      << nprocs*data.size()*sizeof(double)/1.e6/t.elapsed()
                      << " MB/s\n";
        }
      }
  }

  MPI_Finalize();

  return 0;
}
