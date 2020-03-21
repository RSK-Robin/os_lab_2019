#include "revert_string.h"

void RevertString(char *str)
{
	int size = sizeof(str), i = 0;
	char temp;
	for (i = 0; i < size / 2; i++){
		temp = str[i];
		str[i] = str[size - 1 - i];
		str[size - 1 - i] = temp;
	}
}

