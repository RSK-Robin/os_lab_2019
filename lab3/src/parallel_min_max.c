#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            
            break;
          case 1:
            array_size = atoi(optarg);
            
            break;
          case 2:
            pnum = atoi(optarg);
            
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  
  const int part_size = array_size / pnum; 
  const char* file_prefix = "part_result_";
  char fname[32];

  
  int* fd_array, *fd;
  if (!with_files) {
  fd_array = (int*)malloc(sizeof(int) * 2 * pnum);
  }

  for (int i = 0; i < pnum; i++) {
      if(!with_files){
        fd = fd_array + (i * 2); 
        if (pipe(fd) == -1) { 
        exit(EXIT_FAILURE);
        }   
      }

    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        const unsigned int begin = i * part_size;
        const unsigned int end = (i == pnum-1) ? array_size - 1 : begin+part_size; 
        struct MinMax part_minmax = GetMinMax(array, begin, end);

        if(with_files) {
            printf("READING \n");
          sprintf(fname, "%s%d", file_prefix, i);
          FILE* fp = fopen(fname, "w");
          fprintf(fp, "%d %d", part_minmax.min, part_minmax.max);
          fclose(fp); 
        } else {
          close(fd[0]);
          write(fd[1], &part_minmax.min, sizeof(part_minmax.min));
          write(fd[1], &part_minmax.max, sizeof(part_minmax.max));
          close(fd[1]);
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  while (active_child_processes > 0) {
    
 
    wait(0);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min, max;
    if (with_files) {
        printf("WRITING \n");
      sprintf(fname, "%s%d", file_prefix, i);
      FILE* fp = fopen(fname, "r");
      fscanf(fp, "%d %d", &min, &max);
      fclose(fp);
    } else {
      // read from pipes
      fd = fd_array + (i * 2);
      close(fd[1]);
      read(fd[0], &min, sizeof(min));
      read(fd[0], &max, sizeof(max));
      close(fd[0]);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  if(!with_files)
    free(fd_array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
