#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>

#include <pthread.h>
#include "utils.h"

void GenerateArray(int *array, unsigned int array_size, unsigned int seed);

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
    int sum = 0;
	for (int i = args->begin; i < args->end; i++)
	{
		sum += args->array[i];
	}
	
    return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
    int threads_num = -1;
    int seed = -1;
    int array_size = -1;
    int current_optind = optind ? optind : 1;

    while(true){
        static struct option options[] = {{"threads_num", required_argument, 0, 0},
                                        {"seed", required_argument, 0, 0},
                                        {"array_size", required_argument, 0, 0},
                                        {0, 0, 0, 0}};
                                        
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);
    if (c == -1) break;
        switch (c) {
        case 0:
            switch (option_index) {
            case 0:
                threads_num = atoi(optarg);
                break;

            case 1:
                seed = atoi(optarg);
                break;

            case 2:
                array_size = atoi(optarg);
                break;

            defalut:
                printf("Index %d is out of options\n", option_index);
            }
            break;

        case '?':
            break;

        default:
            printf("getopt returned character code 0%o?\n", c);
        }
    
    }
    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || threads_num == -1) {
        printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\" \n",
            argv[0]);
        return 1;
    }

  pthread_t threads[threads_num];

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int part_size = array_size / threads_num;

  struct timeval begin_time;
  gettimeofday(&begin_time, NULL);
  
  struct SumArgs args[threads_num];

  for (uint32_t i = 0; i < threads_num; i++) {
     args[i].array = array;
     args[i].begin = (i == 0) ? i*part_size : i*part_size + 1;
     args[i].end = (i == threads_num - 1) ? array_size : (i + 1)*part_size;

    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  struct timeval end_time;
  gettimeofday(&end_time, NULL);

  double elapsed_time = (end_time.tv_sec - begin_time.tv_sec) * 1000.0;
  elapsed_time += (end_time.tv_usec - begin_time.tv_usec) / 1000.0;


  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

