#include "pipe_networking.h"

int sigPipeReceived = 0;

int main() {

  int to_client;
  int from_client;

  while (1) {
    sigPipeReceived = 0;
    from_client = server_handshake( &to_client );
    sendInts(to_client);
    close(to_client);
    close(from_client);
  }
}

void sendInts(int to_client) {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  int num = rand();
  srand(time(NULL));
  while (!sigPipeReceived) {
    write(to_client, &num, sizeof(num));
    printf("server sent: %d\n", num);
    num = rand();
    sleep(1);
  }
}

static void sighandler(int signo) {
  if (signo == SIGPIPE) sigPipeReceived = 1;
  if (signo == SIGINT) {
    printf("SIGINT intercepted");
    remove(WKP);
    exit(0);
  }
}
