#define BENCHMARK "OSU MPI%s Non-blocking Scatter Latency Test"
/*
 * Copyright (C) 2002-2022 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 *
 * Contact: Dr. D. K. Panda (panda@cse.ohio-state.edu)
 *
 * For detailed copyright and licensing information, please refer to the
 * copyright file COPYRIGHT in the top level OMB directory.
 */

#include <osu_util_mpi.h>

int main(int argc, char *argv[])
{
    int i = 0, j, rank, size;
    int numprocs;
    double latency = 0.0, t_start = 0.0, t_stop = 0.0;
    double test_time = 0.0, test_total = 0.0;
    double tcomp = 0.0, tcomp_total=0.0, latency_in_secs=0.0;
    double timer=0.0;
    double avg_time = 0.0;
    int errors = 0, local_errors = 0;
    double wait_time = 0.0, init_time = 0.0;
    double init_total = 0.0, wait_total = 0.0;
    char *sendbuf=NULL;
    char *recvbuf=NULL;
    int po_ret;
    size_t bufsize;
    MPI_Datatype omb_ddt_datatype = MPI_CHAR;
    size_t omb_ddt_size = 0;
    size_t omb_ddt_transmit_size = 0;
    omb_graph_options_t omb_graph_options;
    omb_graph_data_t *omb_graph_data = NULL;

    set_header(HEADER);
    set_benchmark_name("osu_iscatter");

    options.bench = COLLECTIVE;
    options.subtype = NBC_SCATTER;

    po_ret = process_options(argc, argv);

    if (PO_OKAY == po_ret && NONE != options.accel) {
        if (init_accel()) {
            fprintf(stderr, "Error initializing device\n");
            exit(EXIT_FAILURE);
        }
    }

    MPI_CHECK(MPI_Init(&argc, &argv));
    MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &numprocs));
    MPI_Request request;
    MPI_Status status;

    omb_graph_options_init(&omb_graph_options);
    switch (po_ret) {
        case PO_BAD_USAGE:
            print_bad_usage_message(rank);
            MPI_CHECK(MPI_Finalize());
            exit(EXIT_FAILURE);
        case PO_HELP_MESSAGE:
            print_help_message(rank);
            MPI_CHECK(MPI_Finalize());
            exit(EXIT_SUCCESS);
        case PO_VERSION_MESSAGE:
            print_version_message(rank);
            MPI_CHECK(MPI_Finalize());
            exit(EXIT_SUCCESS);
        case PO_OKAY:
            break;
    }

    if (numprocs < 2) {
        if (rank == 0) {
            fprintf(stderr, "This test requires at least two processes\n");
        }

        MPI_CHECK(MPI_Finalize());
        exit(EXIT_FAILURE);
    }
    check_mem_limit(numprocs);
    if (0 == rank) {
        bufsize = options.max_message_size * numprocs;
        if (allocate_memory_coll((void**)&sendbuf, bufsize, options.accel)) {
            fprintf(stderr, "Could Not Allocate Memory [rank %d]\n", rank);
            MPI_CHECK(MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE));
        }
        set_buffer(sendbuf, options.accel, 1, bufsize);
    }

    if (allocate_memory_coll((void**)&recvbuf, options.max_message_size,
                options.accel)) {
        fprintf(stderr, "Could Not Allocate Memory [rank %d]\n", rank);
        MPI_CHECK(MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE));
    }
    set_buffer(recvbuf, options.accel, 0, options.max_message_size);

    print_preamble_nbc(rank);

    for (size = options.min_message_size; size <= options.max_message_size;
            size *= 2) {
        omb_ddt_size = omb_ddt_get_size(size);
        if (size > LARGE_MESSAGE_SIZE) {
            options.skip = options.skip_large;
            options.iterations = options.iterations_large;
        }

        omb_graph_allocate_and_get_data_buffer(&omb_graph_data,
                &omb_graph_options, size, options.iterations);
        timer = 0.0;
        omb_ddt_transmit_size = omb_ddt_assign(&omb_ddt_datatype, MPI_CHAR,
                size);

        for (i = 0; i < options.iterations + options.skip; i++) {
            if (options.validate) {
                set_buffer_validation(sendbuf, recvbuf, size, options.accel, i);
                for (j = 0; j < options.warmup_validation; j++) {
                    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
                    MPI_CHECK(MPI_Iscatter(sendbuf, omb_ddt_size,
                                omb_ddt_datatype, recvbuf, omb_ddt_size,
                                omb_ddt_datatype, 0, MPI_COMM_WORLD, &request));
                    MPI_CHECK(MPI_Wait(&request,&status));
                }
                MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
            }

            t_start = MPI_Wtime();
            MPI_CHECK(MPI_Iscatter(sendbuf, omb_ddt_size, omb_ddt_datatype,
                        recvbuf, omb_ddt_size, omb_ddt_datatype, 0,
                        MPI_COMM_WORLD, &request));
            MPI_CHECK(MPI_Wait(&request,&status));

            t_stop = MPI_Wtime();
            MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

            if (options.validate) {
                local_errors += validate_data(recvbuf, size, numprocs,
                        options.accel, i);
            }

            if (i>=options.skip) {
                timer += t_stop-t_start;
            }
        }
        MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

        latency = (timer * 1e6) / options.iterations;

        latency_in_secs = timer/options.iterations;

        init_arrays(latency_in_secs);

        MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

        timer = 0.0; tcomp_total = 0; tcomp = 0;
        init_total = 0.0; wait_total = 0.0;
        test_time = 0.0, test_total = 0.0;

        for (i = 0; i < options.iterations + options.skip; i++) {
            if (options.validate) {
                set_buffer_validation(sendbuf, recvbuf, size, options.accel, i);
                for (j = 0; j < options.warmup_validation; j++) {
                    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
                    MPI_CHECK(MPI_Iscatter(sendbuf, omb_ddt_size,
                                omb_ddt_datatype, recvbuf, omb_ddt_size,
                                omb_ddt_datatype, 0, MPI_COMM_WORLD, &request));
                    MPI_CHECK(MPI_Wait(&request,&status));
                }
                MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
            }

            t_start = MPI_Wtime();
            init_time = MPI_Wtime();
            MPI_CHECK(MPI_Iscatter(sendbuf, omb_ddt_size, omb_ddt_datatype,
                        recvbuf, omb_ddt_size, omb_ddt_datatype, 0,
                        MPI_COMM_WORLD, &request));
            init_time = MPI_Wtime() - init_time;

            tcomp = MPI_Wtime();
            test_time = dummy_compute(latency_in_secs, &request);
            tcomp = MPI_Wtime() - tcomp;

            wait_time = MPI_Wtime();
            MPI_CHECK(MPI_Wait(&request,&status));
            wait_time = MPI_Wtime() - wait_time;

            t_stop = MPI_Wtime();
            MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

            if (options.validate) {
                local_errors += validate_data(recvbuf, size, numprocs,
                        options.accel, i);
            }

            if (i>=options.skip) {
                timer += t_stop-t_start;
                tcomp_total += tcomp;
                test_total += test_time;
                init_total += init_time;
                wait_total += wait_time;
                if (options.graph && 0 == rank) {
                    omb_graph_data->data[i - options.skip] = (t_stop -
                            t_start) * 1e6;
                }
            }
        }

        if (options.validate) {
            MPI_CHECK(MPI_Allreduce(&local_errors, &errors, 1, MPI_INT, MPI_SUM,
                        MPI_COMM_WORLD));
        }

        MPI_CHECK(MPI_Barrier (MPI_COMM_WORLD));

        avg_time = calculate_and_print_stats(rank, size, numprocs,
                                  timer, latency,
                                  test_total, tcomp_total,
                                  wait_total, init_total,
                                  errors);
        if (options.graph && 0 == rank) {
            omb_graph_data->avg = avg_time;
        }
        omb_ddt_append_stats(omb_ddt_transmit_size);
        omb_ddt_free(&omb_ddt_datatype);
        if (0 != errors) {
            break;
        }
    }
    if (0 == rank && options.graph) {
        omb_graph_plot(&omb_graph_options, benchmark_name);
    }
    omb_graph_combined_plot(&omb_graph_options, benchmark_name);
    omb_graph_free_data_buffers(&omb_graph_options);

    if (0 == rank) {
        free_buffer(sendbuf, options.accel);
    }
    free_buffer(recvbuf, options.accel);
    MPI_CHECK(MPI_Finalize());

    if (NONE != options.accel) {
        if (cleanup_accel()) {
            fprintf(stderr, "Error cleaning up device\n");
            exit(EXIT_FAILURE);
        }
    }

    if (0 != errors && options.validate && 0 == rank) {
        fprintf(stdout, "DATA VALIDATION ERROR: %s exited with status %d on"
                " message size %d.\n", argv[0], EXIT_FAILURE, size);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

/* vi: set sw=4 sts=4 tw=80: */
