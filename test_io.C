#include <algorithm>
#include <iterator>
#include <iostream>
#include <iostream>
#include <vector>
#include <rpc/rpc.h>
#include <cstddef>
#include <stdio.h>
#include <numeric>
#include <boost/timer.hpp>
#include <mpi.h>
#include <process_cmdline.h>


using namespace MPI_Playground;

namespace
{
  int rank, nprocs;
  std::string io_basename = "test.";
}



void print_bw (const std::string &label, const std::size_t bytes, const double time)
{
  if (rank) return;

  std::cout << label << " "
            << bytes/1e6 << " MB, "
            << time  << " s, "
            << static_cast<double>(bytes)/1.e6/time << " MB/s\n";
}



template <typename T>
void init_vector (std::vector<T> &data)
{
  boost::timer timer;
  data.resize(opts.bufsize);

  if (!opts.write)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      print_bw ("Allocated", nprocs*data.size()*sizeof(double), timer.elapsed());
      return;
    }

  std::iota(data.begin(), data.end(), static_cast<T>(rank*data.size()));

  MPI_Barrier(MPI_COMM_WORLD);
  print_bw ("Initialized", nprocs*data.size()*sizeof(double), timer.elapsed());
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

  // std::cout << "Proc " << rank << " "
  //           << "XDR: Wrote " << data.size()*sizeof(double)/1.e6
  //           << "MB in " << t.elapsed() << " seconds\n";
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

  // std::cout << "Proc " << rank << " "
  //           << "XDR: Read  " << data.size()*sizeof(double)/1.e6
  //           << "MB in " << t.elapsed() << " seconds\n";
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

  // std::cout << "Proc " << rank << " "
  //           << "C: Wrote " << data.size()*sizeof(double)/1.e6
  //           << "MB " << t.elapsed() << " seconds\n";
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

  // std::cout << "Proc " << rank << " "
  //           << "C: Read  " << data.size()*sizeof(double)/1.e6
  //           << "MB " << t.elapsed() << " seconds\n";
}



int main (int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  process_command_line (argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  const bool print_aggregate = (rank == 0) && (nprocs > 1);

  io_basename += std::string(std::to_string(rank));

  {
    boost::timer t;
    std::vector<double> data;
    init_vector(data);


    if (opts.write)
      {
        if (opts.do_xdr)
          {
            t.restart();
            write_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

            print_bw("--> Aggregate XDR write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
        if (opts.do_c)
          {
            t.restart();
            write_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

            print_bw("--> Aggregate C   write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
      }

    if (opts.read)
      {
        if (opts.do_xdr)
          {
            t.restart();
            read_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

            print_bw("--> Aggregate XDR read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
        if (opts.do_c)
          {
            t.restart();
            read_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);

            print_bw("--> Aggregate C   read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
      }
  }

  MPI_Finalize();

  return 0;
}
