#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// params to be passed inside thread function
struct FunParams {
    char* string_1;
    char* string_2;
    int str_len;
    int threads_num;
    int curr_thread_num;
    pthread_mutex_t *mutex;
    int* result;
};

// thread function
void *count_strings_common_char(void* recieved_params)
{
    // getting params
    struct FunParams* params;
    params = (struct FunParams *) recieved_params;


    // looping for all elements for given thread
    for (int ind = params->curr_thread_num; ind < params->str_len; ind += params->threads_num){
        if (params->string_1[ind] != params->string_2[ind]) {
            pthread_mutex_lock(params->mutex);
            (*params->result)++;
            pthread_mutex_unlock(params->mutex);
        }
    }
}


int main()
{
    // changeable variables
	const int str_size = 10; // string size
	const int threads_amount = 3; // amount of threads

    int i;

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


    // initiate mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // result variable
    int result = 0;

    // creating threads
    pthread_t threads[threads_amount];
    for (i = 0; i < threads_amount; i++) {

        struct FunParams *params = malloc(sizeof(struct FunParams));
        params->string_1 = first_string;
        params->string_2 = second_string;
        params->curr_thread_num = i;
        params->str_len = str_size;
        params->threads_num = threads_amount;
        params->mutex = &mutex;
        params->result = &result;

        pthread_create(&threads[i], NULL, count_strings_common_char, params);
	}

    // waiting till treads done
    for (i = 0; i < threads_amount; i++) {
        pthread_join(threads[i], NULL);
    }

    // summing up results of all threads
	printf("there are total %d symbols\n", str_size);
	printf("there are %d NOT same symbols\n", result);
	printf("there are %d same symbols\n", str_size-result);

    return 0;
}


