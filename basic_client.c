#include "pipe_networking.h"

int to_server;
int from_server;

int main() {

  from_server = client_handshake( &to_server );

  sendLetters();
}

void readInts() {
  signal(SIGINT, sighandler);
  int num;
  while (read(from_server, &num, sizeof(num))) printf("client received: %d\n", num);
}

void sendLetters() {
  signal(SIGINT, sighandler);
  char letter;
  srand(time(NULL));
  while (1) {
    letter = rand() % 25 + 97;
    write(to_server, &letter, sizeof(letter));
    printf("client sent: %c\n", letter);
    read(from_server, &letter, sizeof(letter));
    printf("client received: %c\n", letter);
    sleep(1);
  }
}

static void sighandler(int signo) {
  close(to_server);
  close(from_server);
  exit(0);
}
