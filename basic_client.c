#include "pipe_networking.h"

int to_server;
int from_server;

int main() {

  from_server = client_handshake( &to_server );

  readInts(from_server);
}

void readInts(int from_server) {
  signal(SIGINT, sighandler);
  int num;
  while (read(from_server, &num, sizeof(num))) printf("client received: %d\n", num);
}

static void sighandler(int signo) {
  close(to_server);
  close(from_server);
  exit(0);
}
