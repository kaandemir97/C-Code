#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h>
#include <sys/types.h> 
#include <unistd.h>  
  
#define bufsize 1024

void eatZombies(int n){
  // This function removes the zombie process state left
  // Function written by Victoria University of Wellington

  wait3(NULL,WNOHANG,NULL); // Nom Nom 
}

int main(int argc, char *argv[]){
  int sock, length, msgsock, status, sends, readr, bindb, listens, accepta;
  struct sockaddr_in server, client;

  char buf[bufsize];

  // for forking, and cleaning up zombie child state afterwards
  // You must not change this code. 

  pid_t id;       
  signal(SIGCHLD, &eatZombies);
 
  // OK, NWEN 243 code starts here. 

  // Create a socket (see Lab 2) - it is exactly the same for a server!
  sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == 0){
	  printf("Socket failure \n");
  }

  // Next, create the socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#

  // Next you need to BIND your socket.  
  if((bindb = bind(sock, (const struct sockaddr*)&server, sizeof(server)))<0){
	  printf("Bind failure \n");
  } 

  // Start by listening
	if((listens = listen(sock,5))<0){
	  printf("Listen failure \n");
	}
	int serverlen = sizeof(server);
	//Main server loop
  while(1){
    // you need to accept the connection request
	if((accepta = accept(sock, 0,0))<0){
		printf("Accept failure \n");
	}
	else{ //Connection accepted

		// the next call makes a new child process that will actually handle the client.
		id = fork();
		
		// when id == 0, this is the child and needs to do the work for the server. 
		// when if > 0, this is the parent, and it should just loop around,
		// when id < 0, we had an error.

		// Your code here for the child process, that will read from the connection socket, process the data 
		// write back to the socket, and then close and finally call exit(0) when done.
		if(id == 0){
			//While the connection to client is open, listen to read requests
			while(accepta >= 0){
				if((readr = read(accepta,buf,bufsize))<0){
					printf("Server read error \n");
				}
				//Concatenate new message to send back to client message: Server saw this message!
				char newbuf[] = ": Server saw this message!";
				strcat(buf, newbuf);
				if((sends = write(accepta,buf,bufsize))<0){
					printf("Server send fail \n");
				}
			}
			exit(0);
		}
		// Note:  make sure the parent process (id > 0) does not execute this code, but immediately loops back
		// around (via the while) to get another connection request.
		else if(id > 0){
			continue;
		}
		else if(id < 0){
			printf("Fork error \n");
		}
	}
  }
}
