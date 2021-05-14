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

void *recieve(void *data);
void send(void *data);
void datasend(void *data);
int bbb = 0;
int main()
{

    int fd;
    struct termios tty;
    int i, nread;

    fd = open("/dev/ttyUSB0", O_RDWR);

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

    pthread_t p_thread[2];
    pthread_t p_datasendthread;
    //int status;

    pthread_create(&p_thread[0], NULL, (void *)send, (void *)&fd);
    //    pthread_create(&p_thread[1], NULL, recieve, (void *)&fd);

    char message[10];

    while (1)
    {
        //memset(message, 0, 10);
        nread = read(fd, message, 10);
        //printf("recv: nread %d %d\n", nread, message[0]);
        // if (nread == 1)
        // {
        //     printf("receive %d\n", nread);
        // }
        if (nread == 1)
        {
            //printf("recv: nread %d %d\n", nread, message[0]);
            if (message[0] == '1')
            {
                printf("receive 1\n");
                bbb = 0;
                pthread_create(&p_datasendthread, NULL, (void *)datasend, (void *)&fd);
            }
            if (message[0] == '0')
            {
                printf("receive 0\n");
                bbb = 1;
            }
        }
        if (nread > 1)
        {
            printf("recv: ");
            for (int i = 0; i < 10; i++)
            {
                printf("%c", message[i]);
            }
            printf("\n");
            sleep(1);
        }
    }

    pthread_join(p_thread[0], NULL);
    pthread_join(p_datasendthread, NULL);

    close(fd);
}
void datasend(void *data)
{
    int *fd = (int *)data;
    while (1)
    {
        if (bbb == 1)
            break;
        //printf("datasend\n");
        char arr[10];
        arr[0] = '0';
        arr[1] = '1';
        arr[2] = '2';
        arr[3] = '3';
        arr[4] = '4';
        arr[5] = '5';
        arr[6] = '6';
        arr[7] = '7';
        arr[8] = '8';
        arr[9] = '9';
        write(*fd, arr, sizeof(arr));
        sleep(1);
    }
}
void send(void *data)
{
    int *fd = (int *)data;
    while (1)
    {
        char message[255];
        //printf("\nServer -> : ");
        scanf("%s", message);
        if (message[0] == '1')
        {
            printf("send 1\n");
            char msg[1] = {'1'};
            write(*fd, msg, sizeof(msg));
        }

        if (message[0] == '0')
        {
            printf("send 0\n");
            char msg[1] = {'0'};
            write(*fd, msg, sizeof(msg));
            bbb = 1;
        }
    }
}

// void *recieve(void *data)
// {
//     int *fd = (int *)data;

//     while (1)
//     {
//         char message[10];

//         int str_len = read(*fd, message, 10);
//         //printf("%d", str_len);
//         if (str_len > 0)
//         {
//             printf("receive");
//             // if (message[0] == '1')
//             // {
//             //     printf("receive 1");
//             //     while (1)
//             //     {
//             //         pthread_t p_thread[1];
//             //         int status;

//             //         char arr[10];
//             //         arr[0] = '0';
//             //         arr[1] = '1';
//             //         arr[2] = '2';
//             //         arr[3] = '3';
//             //         arr[4] = '4';
//             //         arr[5] = '5';
//             //         arr[6] = '6';
//             //         arr[7] = '7';
//             //         arr[8] = '8';
//             //         arr[9] = '9';
//             //         write(*fd, arr, sizeof(arr));

//             //         sleep(1000);
//             //     }
//             // }
//         }
//     }
// }
