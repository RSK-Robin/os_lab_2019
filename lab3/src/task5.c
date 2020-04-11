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

int main(int argc, char **argv){

    pid_t child_pid = fork();
    if (child_pid >= 0) {
      if (child_pid == 0){
          execv(argv[1], argv + 1);
          return 0;
      }
    }else {
      printf("Fork failed!\n");
      return 1;
    }

}