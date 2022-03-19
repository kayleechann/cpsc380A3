#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/msg.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_LINE];
};

int main(int argc, char *argv[]){

  int running = 1;
  char quitword[5] = "quit";
  char exitword[5] = "exit";
  struct my_msg_st some_data;
  int msgid;
  char buffer[BUFSIZ];

  //message queue key 
  key_t key; 
  key = ftok(argv[1], 'q');

  //Set up message queue
  msgid = msgget(key, 0666 | IPC_CREAT);

  if (msgid == -1) {
    fprintf(stderr, "msgget error: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  //Ask user for input into queue in
  while (running) {
    printf("msgq> ");
    fflush(stdout);

    fgets(buffer, BUFSIZ, stdin);
    some_data.my_msg_type = 1;
    strcpy(some_data.some_text, buffer);

    if (msgsnd(msgid, (void *)&some_data, MAX_LINE, 0) == -1) {
      fprintf(stderr, "msgsnd failed\n");
      exit(EXIT_FAILURE);
    }

    //exit loop if user types quit or exit
    if((strncmp(buffer, exitword, 4) == 0 )| (strncmp(buffer, quitword,4) == 0)){
      running = 0;
    }
  }
  exit(EXIT_SUCCESS);
  return 0;
}
