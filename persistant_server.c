#include "pipe_networking.h"

int main() {

  int to_client;
  int from_client;

  while (1) {
    from_client = server_handshake( &to_client );
    sendInts(to_client);
  }
}

void sendInts(int to_client) {
  int num = rand();
  srand(time(NULL));
  while (write(to_client, &num, sizeof(num))) {
    printf("server sent: %d\n", num);
    sleep(1);
    num = rand();
  }
}
