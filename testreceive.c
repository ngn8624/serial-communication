#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <stdbool.h>

void send(void *data);
void receive(void *data);
void datasend(void *data);
void setelements(char arr[], int count);
bool bSend = true; //
int b = 1;

// typedef struct TYPE
// {
//     char *message;
//     int *fd;
// } TYPE;

int main()
{

    int fd;
    struct termios tty;
    int i, nread;

    fd = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);

    if (tcgetattr(fd, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }
    tty.c_cflag = B115200;
    tty.c_cflag |= CS8;
    tty.c_cflag |= CLOCAL;
    tty.c_cflag |= CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tty);

    // char message[255];

    // TYPE fdfd;
    // fdfd.fd = &fd;
    // fdfd.message = message;

    pthread_t p_thread[2];

    int status;

    pthread_create(&p_thread[0], NULL, (void *)send, (void *)&fd);
    pthread_create(&p_thread[1], NULL, (void *)receive, (void *)&fd);

    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1], NULL);

    close(fd);
}

void datasend(void *data)
{
    int *fd = (int *)data;
    while (bSend)
    {
        // if (bSend == false)
        //     break;
        printf("datasend\n");
        const int count = 2050;
        char arr[count];
        setelements(arr, count);

        write(*fd, arr, sizeof(arr));
        sleep(1);
        //arr[i] = rand() / (char)RAND_MAX;
    }

    // char arr[10];
    // arr[0] = '0';
    // arr[1] = '1';
    // arr[2] = '2';
    // arr[3] = '3';
    // arr[4] = '4';
    // arr[5] = '5';
    // arr[6] = '6';
    // arr[7] = '7';
    // arr[8] = '8';
    // arr[9] = '9';
    // write(*fd, arr, sizeof(arr));
    // sleep(1);
    // printf("%c\n", arr[1]);
}

void setelements(char arr[], int count)
{
    for (int i = 0; i < count; i++)
    {
        arr[i] = rand() % 100;
        printf("%d ", arr[i]);
    }
}
void send(void *data)
{
    int *fd = (int *)data;
    while (1)
    {
        char message[255];
        scanf("%s", message);
        if (message[0] == 49)
        {
            printf("send : 1번");
            char msg[1] = {'1'};
            bSend = true;
            write(*fd, msg, sizeof(msg));
            printf("%s\n", "보냈습니다.(bSend = true)");
        }
        if (message[0] == 48)
        {
            printf("send : 0번");
            char msg[1] = {'0'};
            bSend = false;
            write(*fd, msg, sizeof(msg));
            printf("%s\n", "보냈습니다.(bSend = false)");
        }
    }
}

void receive(void *data)
{
    int *fd = (int *)data;
    // TYPE *type = (TYPE *)data;
    // char *msg = type->message;
    //float *msg = type->msg;
    pthread_t p_datasendthread;

    char message[2050];

    while (1)
    {
        int nread = read(*(fd), message, sizeof(char) * 255);

        if (nread == 1)
        {
            if (message[0] == 49)
            {
                printf("receive %d 받기 : ", 1);
                bSend = true;
                pthread_create(&p_datasendthread, NULL, (void *)datasend, (void *)&*(fd));
                printf("%s\n ", "datathread 실행시켰습니다.");
            }
            if (message[0] == 48)
            {
                printf("receive %d 받기 : ", 0);
                bSend = false;
                printf("%s\n ", "data전송을 멈췄습니다.");
            }
        }
        if (nread > 1)
        {
            printf("recv: ");
            for (int i = 0; i < 2050; i++)
            {
                printf("%s", message);
                //printf("%f", msg[i]);
            }
            printf("\n");
        }
    }
    pthread_join(p_datasendthread, NULL);
}
