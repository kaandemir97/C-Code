
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>  

#define bufsize 1024

int main(argc, argv) int argc; char *argv[];{
  int sock, rval,connectc,sends,readr;
  struct hostent *host;
  struct sockaddr_in server;  // not a pointer.
  char buf[bufsize];
  
  if(argc != 4){
    printf("usage:\ntcpclient hostname port string\n\n");
    return(-1);
  }
  
  // look up hostname (server) using DNS

  if ((host = gethostbyname(argv[1])) == 0) {
    fprintf(stderr, "%s: unknown host\n", argv[1]); 
    return(-1);  
  }

  // Set up fields for socket to point to host and port

  bcopy(host->h_addr, &server.sin_addr, host->h_length);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
 
  // Create socket
  if((sock = socket(AF_INET,SOCK_STREAM,0))<0){
	  printf("Socket failure \n");
  }

  // connect (3-way handshake)
  int serverlen = sizeof(server);
  //Connects client to public socket with server address
  if((connectc = connect(sock,(struct sockaddr*)&server, serverlen))<0){
	  printf("Connection failure \n");
	  printf("%d\n",&connectc);
  }
 
 //Sends 100 messages to server before closing client socket
 for(int i = 0; i < 100; i ++){
	 sleep(3);
	 // Copy the arg into buf so we can send it to the server
	 strncpy(buf, argv[3], bufsize);
	  // Send sentence to server
	  if((sends = write(sock,buf,bufsize))<0){
		  printf("Send failure \n");
	  }
	  // read response from server
	  if((readr = read(sock,buf,bufsize))<0){
		  printf("Read failure \n");
	  }
	// print result to window
	fprintf(stdout,"%s\n", buf);
  }
  printf("%s\n",buf);
  close(sock);
}
