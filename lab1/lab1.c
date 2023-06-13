#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define MY_SIZEOF(input) printf("Size of " #input ": %u\n", sizeof(input))

int main() {
    MY_SIZEOF(char);
    MY_SIZEOF(short);
    MY_SIZEOF(short int);
    MY_SIZEOF(int);
    MY_SIZEOF(long int);
    MY_SIZEOF(unsigned int);
    MY_SIZEOF(void *);
    MY_SIZEOF(size_t);
    MY_SIZEOF(float);
    MY_SIZEOF(double);
    MY_SIZEOF(int8_t);
    MY_SIZEOF(int32_t);
    MY_SIZEOF(int64_t);
    MY_SIZEOF(time_t);
    MY_SIZEOF(clock_t);
    MY_SIZEOF(struct tm);
    MY_SIZEOF(NULL);
    return 0;
}

/*gcc -Wpedantic -Wall -Wextra -Werror -std=c89 -m64 -o lab1 lab1.c*/
/*We can change "%ld" to "%u" and then we can use -m32 to compile the file*/
