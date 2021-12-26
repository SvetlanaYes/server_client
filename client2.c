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
 int shmid;
 key_t key;
 char *shm_2;
 char *s;
 key = 5874;
 
 shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
 
 if (shmid < 0)
 {
   perror("shmget");
   exit(1);
 }
 
 shm_2 = shmat(shmid, NULL, 0);
 
 if (shm_2 == (char *) -1)
 {
   perror("shmat");
   exit(1);
 }
 if (strncmp(shm_2,"server is on",12) != 0)
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
   memcpy(shm_2,cmd,strlen(cmd)+1);
   printf("client - %c\n",*shm_2);
 }
   
 }
