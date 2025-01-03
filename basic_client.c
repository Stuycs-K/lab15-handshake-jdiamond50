#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  readInts(from_server);
}

void readInts(int from_server) {
  int num;
  while (read(from_server, &num, sizeof(num))) printf("client received: %d\n", num);
}
