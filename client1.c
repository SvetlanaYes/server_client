#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>
#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>

#define SHSIZE 100
//client1 ID = 1
int main(int argc, char* argv[])
{
 char buf[PATH_MAX];
 int shmid;
 key_t key;
 char *shm_1;
 char *s;
 key = 9874;
 
 char *messages_from_server = "/tmp/messages_for_client1";
 int fd_answers;
 mkfifo(messages_from_server, 0666);
 fd_answers = open(messages_from_server, O_RDONLY);
 
 shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
 
 if (shmid < 0)
 {
   perror("shmget");
   exit(1);
 }
 
 shm_1 = shmat(shmid, NULL, 0);
 
 if (shm_1 == (char *) -1)
 {
   perror("shmat");
   exit(1);
 }
 if (strncmp(shm_1,"server is on",12) != 0)
 {
   fprintf(stderr, "Server is not on !");
   exit(1);
 } 
 char cmd[2];
 while(1)
 {
   printf("1 - <register> <client_id>\n2 - <get_clients>\n3 - <send client_id> <message>\n");
   printf("\n");
   scanf("%c",cmd);
   memcpy(shm_1,cmd,strlen(cmd)+1);
       printf("fsdfs\n");
   if (read(fd_answers, buf, PATH_MAX) == -1)
   {
      fprintf(stderr, "cannot read from fifo !");
   exit(1);
   }
   else
   {
    printf("fsdfs\n");
   }


 }
   
 }
