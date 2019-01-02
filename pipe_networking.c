#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Created upstream pipe for the first time, or it already exists \n");
  int pipe = mkfifo("WKP",0644);
  printf("Waiting for a client \n");
  int up_pipe = open("WKP",O_RDONLY);
  if (!up_pipe){
    perror("Error opening the upstream pipe");
  }
  else{
    printf("Opened upstream pipe\n");
  }
  
  char s[HANDSHAKE_BUFFER_SIZE];

  read(up_pipe,s,HANDSHAKE_BUFFER_SIZE);


  if (s){
    printf("Client message successfully received\n");

    *to_client = open(s,O_WRONLY);
    remove("WKP");
    if (*to_client < 0){
      perror("Error opening downpipe from client to write");
      return -1;
    }

    printf("Writing to client\n");
    write(*to_client,ACK,sizeof(ACK));
    printf("Wrote message to client\n");
    return up_pipe;
  }
  else{
    printf("Client message verification error\n");
    printf("%s\n",s);
    return -1;
  }
  return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Opening comms to WKP \n");
  int comm = open("WKP", O_WRONLY);

  if (comm < 0){
    perror("Error opening up to WKP");
  }

  int l = mkfifo("DP",0644);
  if (l != 0){
    perror("Error with downpipe");
  }

  printf("Now writing DP (My pipe name) to the WKP \n");

  write(comm, "DP", sizeof("DP"));

  *to_server = comm;

  printf("Now opening downstream from server to read returns \n");

  int dcomm = open("DP", O_RDONLY);
  remove("DP");
  if (dcomm < 0){
    perror("Error opening up to downpipe");
  }

  char s[HANDSHAKE_BUFFER_SIZE];

  read(dcomm,s,HANDSHAKE_BUFFER_SIZE);
  if (strcmp(s,ACK) == 0){
    printf("Successfully received info from server \n");
  }
  else{
    printf("Not ackowledged by server");
    return -1;
  }

//  char * g = "Got message";
//  write(comm,g, sizeof(g));
//  printf("Replying to server and finishing\n");
  printf("Finishing Handshake \n");
  return dcomm;
}
