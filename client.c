#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  while(1){
    char in[500];
    printf("Enter Input:");
    fgets(in,sizeof(in),stdin);
    write(to_server,in,sizeof(in));
    read(from_server,in,sizeof(in));
    int max = 0;
    while(in[max]){
      max++;
    }
    in[max - 1] = 0;
    printf("Data Returned From Server: %s \n",in);
  }
}
