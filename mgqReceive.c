#define MAX_LINE 80 /* 80 chars per line, per command */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

struct msgbuf {
long mtype;
      char mtext[MAX_LINE];
};
int main(int argc, char *argv[])
{
key_t  key;  /* message queue key */
   int should_run = 1;
key = ftok(argv[1], ‘q’);
/* Create/attach message queue using key */
    while (should_run) {   
        
/** read command from message queue then call the system ( ) 
function to execute the system command. Break out of loop if 
‘quit’ or ‘exit’ is read from the queue then exit program **/
}
return 0;
}