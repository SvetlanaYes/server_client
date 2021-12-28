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
#define BUFSIZE 100
#define CLIENT_COUNT 10

pthread_t client_threads[CLIENT_COUNT] = {0};


void startup_server() {
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
	 char up_message[] = "server is on";
 	 memcpy(shm_r, up_message, sizeof(up_message));	
}


void* client(void* args) {

	char* child_pipe = (char*) args;
	printf("In client function `%s`\n", child_pipe);
	free(child_pipe);
}


int create_pipe() {
	char *messages_from_server = "/tmp/registration";
        mkfifo(messages_from_server, 0666);
        int fd = open(messages_from_server, O_RDWR);
	return fd;
}

void start_to_lisen() {
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
	 
	int fd = create_pipe();
	int connected_client_count = 0;

	char client_pid[BUFSIZE];
	char buf[BUFSIZE];
	int fd_child;
	while(read(fd, client_pid, 5)) {
	   
	       if (connected_client_count < CLIENT_COUNT) {
                       strcat(shm,client_pid);
                       strcat(shm,"\n");
			char* child_pipe = (char*)malloc(BUFSIZE);
			sprintf(child_pipe, "/tmp/client_%s", client_pid); 
			printf("%s\n",child_pipe);
                       if (mkfifo(child_pipe,0666) == -1)
                       {
                         perror("mkfifo");
                         exit(1);
                       }
                       fd_child = open(child_pipe,O_RDWR);
                       write(fd,"hi",sizeof("hi"));
			pthread_create(&client_threads[connected_client_count], NULL, 
				       client, child_pipe);
                        
             	        pthread_join(client_threads[connected_client_count], NULL);
                      read(fd, buf, BUFSIZE);
		} else {
			printf("Already registered\n");
		}
	}
	
}

void* messages(void* args) {

	char *messages = "/tmp/messages";
	if (mkfifo(messages, 0666) == -1)
	{
	   unlink(messages);
	}
       if (mkfifo(messages, 0666) == -1)
	{
	   unlink(messages);
	}
        int fd = open(messages, O_RDWR);

        char client_pid[BUFSIZE];
	char buf[BUFSIZE];
	write(fd,"start",sizeof("start"));
	
       while(read(fd,buf,5))
       {
         printf("ed");
       }

}





void clean()
{
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
	key_t key1 = 9874;
	int shmid1 = shmget(key1, SHSIZE, IPC_CREAT | 0666);
	if (shmid1 < 0) {
		printf("Cannot shmget\n");
		exit(1);
	}

	char* shm = shmat(shmid1, NULL, 0);

	 if (shm == (char *) -1) {
	 perror("shmat");
	 exit(1);
	 }
	 
	 
	 memcpy(shm_r,"",sizeof(""));
	 memcpy(shm,"",sizeof(""));

}



int main(int argc, char* argv[])
{
        clean();
	startup_server();
	pthread_t mythread;
        pthread_create(&mythread, NULL,messages,NULL);
        start_to_lisen();
        pthread_join(mythread, NULL);
}
