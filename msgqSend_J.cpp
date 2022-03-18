#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <errno.h>
#include <sys/msg.h>

#include <fstream>
#include <cstdint>
#include <filesystem>

using namespace std;

#define MAX_LINE 80 /* 80 chars per line, per command */

typedef struct {
    long mtype;
    char mtext[MAX_LINE];
} msgbuf_1 ;

int main(int argc, char *argv[]){

  // exit conditions
  char q1[5] = "quit";
  char q2[5] = "exit";

  int should_run = 1;

  msgbuf_1 data;
  int msgid;
  char buffer[BUFSIZ];

  key_t key; /* message queue key */
  key = ftok(argv[1], 'q');

  /* Create/attach message queue using key */

  msgid = msgget(key, 0666 | IPC_CREAT);

  if (msgid == -1) {
    fprintf(stderr, "msgget error: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  while (should_run) {
    printf("msgq>");
    fflush(stdout);

    /** read command from stdin then send command to message queue
    Break out of loop if user types ‘quit’ or ‘exit’ then delete the message queue and exit
    program **/

    fgets(buffer, BUFSIZ, stdin);
    data.mtype = 1;
    strcpy(data.mtext, buffer);

    if (msgsnd(msgid, (void *)&data, MAX_LINE, 0) == -1) {
      fprintf(stderr, "msgsnd failed\n");
      exit(EXIT_FAILURE);
    }
    if (strncmp(buffer, "end", 3) == 0) {
      should_run = 0;
    }

    //exit loop if user types quit or exit
    if((strncmp(buffer, q2 , 4) == 0 )| (strncmp(buffer, q1 ,4) == 0)){
      should_run = 0;
    }
  }
  exit(EXIT_SUCCESS);

  return 0;
}
