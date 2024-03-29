#include "mpi_play_config.h"
#include "process_cmdline.h"
#include "test_fortran_io.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <iostream>
#include <vector>
#ifdef HAVE_XDR
#  include <rpc/rpc.h>
#  include <rpc/xdr.h>
#endif
#include <cstddef>
#include <stdio.h>
#include <numeric>
//#include <boost/timer.hpp>
#include <mpi.h>

#ifdef HAVE_HDF5
#  include <highfive/H5File.hpp>
#  include <highfive/H5DataSet.hpp>
#  include <highfive/H5DataSpace.hpp>
using namespace HighFive;
#endif

using namespace MPI_Playground;

namespace
{
  int rank, nprocs;
  std::string io_basename = "test.";
  const unsigned int bytes_per_MB = 1024 * 1024;
  const unsigned int bytes_per_GB = bytes_per_MB * 1024;
  const std::string DATASET_NAME("dset");

  class my_timer
  {
  private:
    double t_start;
  public:

    my_timer() :
      t_start(MPI_Wtime())
    {}

    double elapsed() const { return (MPI_Wtime() - this->t_start); }

    void restart() { t_start = MPI_Wtime(); }
  };

}



void print_bw (const std::string &label, const std::size_t bytes, const double time)
{
  if (rank) return;

  std::cout << label << " "
            << bytes/bytes_per_MB << " MB, "
            << time  << " s, "
            << static_cast<double>(bytes/bytes_per_MB)/time << " MB/s\n";
}



template <typename T>
void init_vector (std::vector<T> &data)
{
  my_timer timer;
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



#ifdef HAVE_XDR
void write_xdr (std::vector<double> &data)
{
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
}



void read_xdr (std::vector<double> &data)
{
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

  if (!data.empty() && (data[0] != static_cast<double>(rank*data.size())))
    std::cerr << "Unexpected value read!\n";

  fflush (fp);
  fclose (fp);
}
#endif


void write_c (std::vector<double> &data)
{
  std::string fname = io_basename + ".bin";
  FILE *fp = fopen(fname.c_str(), "w");

  fwrite (data.empty() ? NULL : &data[0],
          sizeof(double),
          data.size(),
          fp);

  fflush (fp);
  fclose (fp);
}



void read_c (std::vector<double> &data)
{
  std::string fname = io_basename + ".bin";
  FILE *fp = fopen(fname.c_str(), "r");

  fread (data.empty() ? NULL : &data[0],
         sizeof(double),
         data.size(),
         fp);

  if (!data.empty() && (data[0] != static_cast<double>(rank*data.size())))
    std::cerr << "Unexpected value read!\n";

  fflush (fp);
  fclose (fp);
}



#ifdef HAVE_HDF5

void write_h5 (std::vector<double> &data)
{
  std::string fname = io_basename + ".h5";

  // we create a new hdf5 file
  File file(fname, File::ReadWrite | File::Create | File::Truncate);

  // lets create a dataset of native double  with the size of the vector
  // 'data'
  DataSet dataset =
    file.createDataSet<double>(DATASET_NAME, DataSpace::From(data));

  // lets write our vector of int to the HDF5 dataset
  dataset.write(data); /**/ file.flush();
}



void read_h5 (std::vector<double> &data)
{
  std::string fname = io_basename + ".h5";

  // we create a new hdf5 file
  File file(fname, File::ReadOnly);

  // we get the dataset
  DataSet dataset = file.getDataSet(DATASET_NAME);

  // we convert the hdf5 dataset to a single dimension vector
  dataset.read(data);

  if (!data.empty() && (data[0] != static_cast<double>(rank*data.size())))
    std::cerr << "Unexpected value read!\n";
}
#endif // #ifdef HAVE_HDF5



int main (int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  process_command_line (argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  //const bool print_aggregate = (rank == 0) && (nprocs > 1);

  {
    char buf[256];
    sprintf(buf, "%03d", rank);
    io_basename.append(buf);
  }

  {
    my_timer t;
    std::vector<double> data;
    init_vector(data);

    //-------------------------------------------------------
    // write tests
    if (opts.write)
      {
#ifdef HAVE_XDR
        if (opts.do_xdr)
          {
            t.restart();
            write_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate XDR  write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#endif
        if (opts.do_c)
          {
            t.restart();
            write_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate C    write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#ifdef HAVE_HDF5
        if (opts.do_h5)
          {
            t.restart();
            write_h5(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate HDF5 write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#endif
        if (opts.do_fortran)
          {
            t.restart();
            unsigned int len = data.size();
            WRITE_FORTRAN(&rank, &len, &data[0]); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate F90  write bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
      }

    //-------------------------------------------------------
    // read tests
    if (opts.read)
      {
#ifdef HAVE_XDR
        if (opts.do_xdr)
          {
            t.restart();
            read_xdr(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate XDR  read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#endif
        if (opts.do_c)
          {
            t.restart();
            read_c(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate C    read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#ifdef HAVE_HDF5
        if (opts.do_h5)
          {
            t.restart();
            read_h5(data); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate HDF5 read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
          }
#endif
        if (opts.do_fortran)
          {
            t.restart();
            unsigned int len = data.size();
            READ_FORTRAN(&rank, &len, &data[0]); /**/ MPI_Barrier(MPI_COMM_WORLD);
            print_bw("--> Aggregate F90  read  bw ", nprocs*data.size()*sizeof(double), t.elapsed());
            if (!data.empty() && (data[0] != static_cast<double>(rank*data.size())))
              std::cerr << "Unexpected value read!\n";
          }
      }
  }

  MPI_Finalize();

  return 0;
}
