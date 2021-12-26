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
#include <pthread.h>


#define SHSIZE 100

void *serve_client_1 (void *arg)
{
  int shmid;
  key_t key;
  char *shm_1;
  
  char *messages_from_server = "/tmp/messages_for_client1";
  int fd_answers;
  mkfifo(messages_from_server, 0666);
  fd_answers = open(messages_from_server, O_RDWR);
  
  int fd;
  char *registration = "/tmp/registration";
  mkfifo(registration, 0666);
  fd = open(registration, O_RDWR);
  
  key = 9874;
 
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
  memcpy(shm_1,"server is on",13);
  int a;
  while (*shm_1 != '*' )
  { 
   a = 1;
   char buf[PATH_MAX];
   read(fd,buf,PATH_MAX);
   char *s;
   if (!strcmp(shm_1,"1"))
   { 
     for (s = buf;*s != '\0';++s)
     {
       if (*s == '1')
       {
         a = 0;
       }
     }
     if (a)
     {
        write(fd, "1",sizeof("1"));
     }
     else 
     {
       write(fd_answers,"YOU ARE ALREADY REGISTERED!",sizeof("YOU ARE ALREADY REGISTERED!"));
     }
     read(fd_answers,buf,PATH_MAX);
     printf("%s\n",buf);
     memcpy(shm_1,"/",1);
   }
   else if (!strcmp(shm_1,"2"))
   {
     fd = open(registration, O_RDONLY);
     read(fd, buf,PATH_MAX);
     printf("%s",buf);
     *shm_1 = '/';
   }
   else if (*shm_1 == '3')
   {
     *shm_1 = '/';
   }
    
  }

  
}


void serve_client_2 ()
{
 
  int shmid;
  key_t key;
  char *shm_2;
  int fd;
  char *registration = "/tmp/registration";
  
  mkfifo(registration, 0666);
  fd = open(registration, O_RDWR);
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
  memcpy(shm_2,"server is on",13);
  int a;
  while (*shm_2 != '*' )
  { 
   a = 1;
   if (!strcmp(shm_2,"1"))
   {
     char buf[PATH_MAX];
     char *s;
     for (s = buf;*s != '\0';++s)
     {
       if (*s == '2')
       {
         a = 0;
       }
     }
     if (a)
     {
        write(fd, "2",sizeof("1"));
        read(fd, buf, PATH_MAX);
        close(fd);
     }
     else 
     {
       
       printf("YOU ARE ALREADY REGISTERED!\n");
     }
     memcpy(shm_2,"/",1);
   }

}
}

int main(int argc, char* argv[])
{
 
 char *s;
 char *messages_from_1_to_2;
 char *messages_from_2_to_1;
 
 pthread_t mythread;
 pthread_create(&mythread, NULL,serve_client_1,NULL);
 serve_client_2();
 pthread_join(mythread, NULL);
      
}
