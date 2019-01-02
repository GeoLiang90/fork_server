#include "pipe_networking.h"
#include <signal.h>
void sighandler(int signo){
  if (signo == SIGINT){
    remove("WKP");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);
  int to_client;
  int from_client;
  while(1){
  from_client = server_handshake( &to_client );
  while(1){
    char in[500];
    int z = read(from_client,in,sizeof(in));
    if (!z){
      break;
    }
    int i = 0;
    while(in[i]){
      in[i] += 5;
      i++;
    }
    write(to_client,in,sizeof(in));
  }
}
}
