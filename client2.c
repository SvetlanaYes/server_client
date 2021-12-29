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
#include <stdbool.h>


#define SHSIZE 100
#define BUFSIZE 100
#define CLINET_COUNT 10


int connect_to_pipe() {
        char *registration_pipe = "/tmp/registration";
        mkfifo(registration_pipe, 0666);
        int fd = open(registration_pipe, O_RDWR);
        if (fd == -1) {
                printf("Cannot open regisration pipe. error %s", strerror(errno));
                exit(1);
        }

        return fd;
}



void registration() {

        int pid = getpid();
        int reg_fd = connect_to_pipe();


        char pid_str[BUFSIZE];
        sprintf(pid_str, "%d", pid);
        write(reg_fd, pid_str, sizeof(pid_str));
        printf("Client registered with pid %d\n", pid);

}

void get_clients()
{

      int pid = getpid();
        int reg_fd = connect_to_pipe();

        key_t key = 9874;
	int shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
	if (shmid < 0) {
		printf("Cannot shmget\n");
		exit(1);
	}

	char* shm = shmat(shmid, NULL, 0);

	 if (shm == (char *) -1) {
	 perror("shmat");
	 exit(1);
	 }
	 printf("%s\n",shm);
}


bool is_Server_on() {

	key_t key = 5874;
	int shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
	if (shmid < 0) {
		printf("Cannot shmget\n");
		exit(1);
	}

	char* shm_r = shmat(shmid, NULL, 0);

	 if (shm_r == (char *) -1) {
	    perror("shmat");
	    exit(1);
	 }
	 if (strncmp(shm_r,"server is on",12) != 0)
         {
           return 0;
         } 
         return 1;
}


void send_message(char* message,char* client_name)
{
        key_t key = 9874;
	int shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
	if (shmid < 0) {
		printf("Cannot shmget\n");
		exit(1);
	}

	char* shm = shmat(shmid, NULL, 0);

	 if (shm == (char *) -1) {
	 perror("shmat");
	 exit(1);
	 }
	 
        char* ret;
        ret =  strstr(shm, client_name);
        if(!ret)
        {
         printf("No such client in server");
         return;
        }
        
        
        char *messages = "/tmp/messages";
        int fd = open(messages, O_RDWR);
        
        write(fd,client_name,sizeof(client_name));
        write(fd,message,sizeof(message));

}

int main(int argc, char* argv[])
{ 
       if (!is_Server_on())
       {
         fprintf(stderr, "Server is not on !");
         exit(1);
       }
       registration(); 
       
       int cmd = 1;     
       while(cmd)
       {
          printf (" Choose option\n 1 - <get_clients>\n 2 - <client_id> <message>\n 0 - EXIT");
          scanf("%d",&cmd);
          if (cmd == 1)
          {
            get_clients();
          }
          
          if (cmd == 2)
          {
            char message[BUFSIZE];
            char client_name[BUFSIZE];
            scanf("%s",message);
            scanf("%s",client_name);
            send_message(message,client_name);
          }
       }

}
