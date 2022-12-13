#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// params to be passed inside thread function
struct FunParams {
    char* string_1;
    char* string_2;
    int str_len;
    int threads_num;
    int curr_thread_num;
};


int main(int argc, char** argv)
{
    double start_time, finish_time;

    time_t time_seed = time(NULL);
    // changeable variables
    const int str_size = 1024 * 256; // string size
    int i;

    // defining two strings
    char first_string[str_size + 1];
    char second_string[str_size + 1];


    // get tread number/count
    int total_threads, curr_thread;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &total_threads);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_thread);


    // random string params setup
    srand((unsigned int)(time_seed));
    char rnd_charset[] = "ab";//cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/,.-+=~`<>:";

    // filling both with random chars
    for (i = 0; i <= str_size - 1; i++) {
        first_string[i] = rnd_charset[rand() % (sizeof(rnd_charset) - 1)];
        second_string[i] = rnd_charset[rand() % (sizeof(rnd_charset) - 1)];
    }
    first_string[str_size] = '\0';
    second_string[str_size] = '\0';

    //// print to see strings
    //if (curr_thread == 0) {
    //    printf("%s\n", first_string);
    //    printf("%s\n", second_string);
    //}

    // wait till all processes do that
    MPI_Barrier(MPI_COMM_WORLD);

    // starting loop time
    start_time = MPI_Wtime();

    // start counting
    int local_result = 0, global_result = 0;



    // looping for all elements for current thread
    for (int ind = curr_thread; ind < str_size; ind += total_threads) {
        if (first_string[ind] != second_string[ind]) {
            local_result++;
        }
    }

    // check result
    printf("rank %d, result %d\n", curr_thread, local_result);

    // wait till all processes do that
    MPI_Barrier(MPI_COMM_WORLD);

    // reducing all local results to global one
    MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // starting loop time
    finish_time = MPI_Wtime();

    // print to see strings
    if (curr_thread == 0) {
        printf("rank %d, total_result %d; time: %.7f\n", curr_thread, global_result, finish_time - start_time);
    }

    MPI_Finalize();
}

/*
time results:
string len = 1024*256

threads_num |   time
    1       | 0.0034667
    2       | 0.0005669
    4       | 0.0007330
    8       | 0.0007050
    16      | 0.0005944
*/