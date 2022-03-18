/* Here's the receiver program. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/msg.h>
#include <signal.h>

#define MAX_LINE     80 /* 80 chars per line, per command */

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_LINE];
} ;

int main(int argc, char *argv[])
{
    printf("started\n");
    int running = 1;
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 0;
    char exitWord[5] = "exit";
    char quitWord[5] = "quit";

/* First, we set up the message queue. */
    key_t key; /* message queue key */
    key = ftok(argv[1], 'q');
    msgid = msgget(key, 0666 | IPC_CREAT);
    printf("message created\n");
    printf("msgid: %d", msgid);
    printf("\n");
    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

/* Then the messages are retrieved from the queue, until an end message is encountered.
 Lastly, the message queue is deleted. */

    while(running) {
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                   msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("msg received, parsing...\n");
        //get out of program if input is quit or exit
        if((strncmp(some_data.some_text, exitWord, 4) == 0) | (strncmp(some_data.some_text, quitWord ,4) == 0)){
        running = 0;
        }else{
           printf("You wrote: %s", some_data.some_text);
            int i;
            printf ("Processor status: ");
            if (system(NULL))
                puts ("Ready");
            else exit (EXIT_FAILURE);
                printf ("Executing command: %s \n", some_data.some_text);

            i = system(some_data.some_text);
            if (i == 0){
                printf ("System call executed successfully\n");
            }
            else{
                printf("System call failed, returned %d \n", i);
            }
            
        }

    }


    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
    return 0;
}
