#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    int arr_len = argc - 1;
    int arr_start = 1;
    char inbuf[20];
    int p[2];
    int num_children = 1;
    printf("I am parent with pid: %d, sending the array: ", (int) getpid());
    for (int i = arr_start; i < arr_len; i++)
    {
        printf("%d ", atoi(argv[i]));
    }
    printf("to child with pid: %d\n", (int) getpid());
    
    int rc = fork();

    if(rc < 0)
    {
        printf("Fork failed. Exiting.....");
        exit(1);
    }
    else if(rc == 0)
    {
        int sum = 0;
        printf("I am child with pid: %d, adding the array: ");
        for(int i = 0; i < arr_len; i++)
        {
            printf("%d ", atoi(argv[i]));
            sum += atoi(argv[i]);
        }
        printf("and sending partial sum: %d\n", sum);
        if(pipe(p) < 0)
        {
            printf("Sending partial sum failed. Exiting....");
            exit(2);
        }
        else
        {
            char * sum_char = {sum + '0', '\0'};
            write(p[1], sum_char, 20);
        }
    }
    else {
        wait(NULL);
        int total = 0;
        printf("I am parent with pid: %d, recieving partial sum: ", (int) getpid());
        for(int i = 0; i < num_children; i++)
        {
            read(p[1], inbuf, 20);
            total += atoi(inbuf);
            printf("%d, ", atoi(inbuf));
        }

        printf("and printing: %d", total);

    }

    return 0;

}