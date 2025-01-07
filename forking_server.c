#include "pipe_networking.h"

int sigPipeReceived = 0;

int main() {

  int to_client;
  int from_client;

  while(1) {
    signal(SIGINT, sighandler);
    from_client = server_setup();
    printf("forking\n");
    pid_t p = fork();
    if (p == 0) {
      sigPipeReceived = 0;
      server_handshake_half(&to_client, from_client);
      nextLetter(to_client, from_client);
      printf("subserver terminated\n");
      exit(0);
    }
    close(to_client);
    close(from_client);
  }
}

void sendInts(int to_client) {
  signal(SIGPIPE, sighandler);
  int num = rand();
  srand(time(NULL));
  while(!sigPipeReceived) {
    write(to_client, &num, sizeof(num));
    printf("subserver sent: %d\n", num);
    num = rand();
    sleep(1);
  }
}

void nextLetter(int to_client, int from_client) {
  signal(SIGPIPE, sighandler);
  char letter;
  while(!sigPipeReceived) {
    read(from_client, &letter, sizeof(letter));
    letter++;
    write(to_client, &letter, sizeof(letter));
  }
}

static void sighandler(int signo) {
  if (signo == SIGPIPE) sigPipeReceived = 1;
  if (signo == SIGINT) {
    printf("SIGINT intercepted\n");
    remove(WKP);
    exit(0);
  }
}
