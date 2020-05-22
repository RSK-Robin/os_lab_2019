#include "sum.h"

#include <stdio.h>
#include <stdlib.h>
int Sum(const struct SumArgs *args) {
int sum = 0;
  int result = 0;
	for (int i = args->begin; i < args->end; i++)
	{
		result += args->array[i];
	}
	
    return result;
}