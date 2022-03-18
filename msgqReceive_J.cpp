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
#include <vector>
#include <signal.h>

using namespace std;

#define MAX_LINE     80 /* 80 chars per line, per command */

typedef struct {
    long mtype;
    char mtext[MAX_LINE];
} msgbuff ;

int main(int argc, char *argv[]){

  // exit conditions
  char q1[5] = "quit";
  char q2[5] = "exit";

  int should_run = 1;
  long int msg_to_receive = 0;
  int msgid;

  key_t  key;       /* message queue key */
  key = ftok(argv[1], 'q');
  /* Create/attach message queue using key */
  msgid = msgget(key, 0666 | IPC_CREAT);

  if (msgid == -1) {
    fprintf(stderr, "msgget error: %d\n", errno);
    exit(EXIT_FAILURE);
  }


  msgbuff data;

  while (should_run) {
    /** read command from message queue then call the system()
      function to execute the system command. Break out of loop if
      ‘quit’ or ‘exit’ is read from the queue then exit program **/

    if (msgrcv(msgid, (void *)&data, BUFSIZ, msg_to_receive, 0) == -1) {
      fprintf(stderr, "msgrcv error: %d\n", errno);
      exit(EXIT_FAILURE);
    }

    // check if exit or quit
    if((strncmp(data.mtext, q2 , 4) == 0) | (strncmp(data.mtext, q1 ,4) == 0)){
      should_run = 0;
    }

    else {
      printf("Text: %s", data.mtext);
      int i;
      printf ("Processor status: ");
      if (system(NULL))
        puts ("Ready");
      else exit (EXIT_FAILURE);
        printf ("Executing command: %s \n", data.mtext);

      i=system (data.mtext);
      if (i == 0){
        printf ("Operation executed succesfully\n");
      }
      else{
        printf("Operation failed\n");
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
