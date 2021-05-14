#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    // float형 배열 만들기
    const int count = 10;
    float arr[count];
    for (int i = 0; i < count; i++)
    {
        arr[i] = rand() / (float)RAND_MAX;
        printf("arr : %f\n", arr[i]);
    }
    return 0;
}