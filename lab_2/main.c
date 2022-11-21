#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



int main()
{
    // changeable variables
	const int str_size = 16; // string size
	const int threads_amount = 4; // amount of threads
	const int chunk_size = 4;

    int i;

    // defining omp time variables
    double begin_time, end_time;

    // random string params setup
    srand((unsigned int)(time(NULL)));
	char rnd_charset[] = "ab";//cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/,.-+=~`<>:";

    // defining two strings
	char first_string[str_size+1];
	char second_string[str_size+1];

	// filling both with random chars
	for (i = 0; i <= str_size - 1; i++) {
        first_string[i] = rnd_charset[rand() % (sizeof(rnd_charset) - 1)];
        second_string[i] = rnd_charset[rand() % (sizeof(rnd_charset) - 1)];
	}
	first_string[str_size] = '\0';
	second_string[str_size] = '\0';

    // print to see strings
    printf("%s\n", first_string);
    printf("%s\n", second_string);

	// start time count
    begin_time = omp_get_wtime();

    // result variable
    int result = 0;
    int local_result = 0;


    // counting loop

    #pragma omp parallel firstprivate(local_result)
    {
        #pragma omp for schedule(dynamic, chunk_size)
        for (int ind = 0; ind < str_size; ind += 1){
            if (first_string[ind] != second_string[ind]) {
                result++;
            }
        }

        #pragma omp critical
        result += local_result;
    }



    // end time count
    end_time = omp_get_wtime();



    // summing up results of all threads
	printf("there are total %d symbols\n", str_size);
	printf("there are %d NOT same symbols\n", result);
	printf("there are %d same symbols\n", str_size-result);
	printf("counting time: %f \n", end_time - begin_time);

    return 0;
}


