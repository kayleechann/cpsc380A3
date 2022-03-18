/* The sender program is very similar to msg1.c. In the main set up, delete the
 msg_to_receive declaration and replace it with buffer[BUFSIZ], remove the message
 queue delete and make the following changes to the running loop.
 We now have a call to msgsnd to send the entered text to the queue. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int main(int argc, char *argv[])
{
    int running = 1;
    struct my_msg_st some_data;
    int msgid;
    char buffer[BUFSIZ];

    if (argc == 1){
        printf("Please enter filename\n");
        exit(1);
    }

    key_t key; /* message queue key */
    key = ftok(argv[1], 'q');


    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while(running) {
        printf("msgq> ");
        fflush(stdout);
        fgets(buffer, BUFSIZ, stdin);
        some_data.my_msg_type = 1;
        strcpy(some_data.some_text, buffer);

        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        if (strncmp(buffer, "exit", 4) == 0 || strncmp(buffer, "quit", 4) == 0) {
            running = 0;
        }
    }

    exit(EXIT_SUCCESS);
}