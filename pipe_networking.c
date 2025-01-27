#include "pipe_networking.h"

int err() {
  printf("errno %d\n", errno);
  printf("%s\n", strerror(errno));
  exit(1);
}
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;

  // creatintg WKP
  int mkfifoOut = mkfifo(WKP, 0666);

  // opening WKP
  from_client = open(WKP, O_RDONLY);
  if (from_client == -1) err();

  // remove WKP
  remove(WKP);

  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {

  int from_client = server_setup();

  // reads PP from WKP
  char* PP = (char*) malloc(20 * sizeof(char));
  read(from_client, PP, 20);
  printf("server received: %s\n", PP);

  // opens PP
  *to_client = open(PP, O_WRONLY);

  // sending SYN_ACK
  srand(time(NULL));
  int syn_ack = rand();
  write(*to_client, &syn_ack, sizeof(syn_ack));
  printf("server sent: %d (SYN_ACK)\n", syn_ack);

  // reading ACK
  int ack;
  read(from_client, &ack, sizeof(ack));
  printf("server received: %d (ACK)\n", ack);
  if (ack != syn_ack + 1) err();

  printf("handshake complete\n");

  return from_client;
}

void server_handshake_half(int *to_client, int from_client) {

  // reads PP from WKP
  char* PP = (char*) malloc(20 * sizeof(char));
  read(from_client, PP, 20);
  printf("subserver received: %s\n", PP);

  // opens PP
  *to_client = open(PP, O_WRONLY);

  // sending SYN_ACK
  srand(time(NULL));
  int syn_ack = rand();
  write(*to_client, &syn_ack, sizeof(syn_ack));
  printf("subserver sent: %d (SYN_ACK)\n", syn_ack);

  // reading ACK
  int ack;
  read(from_client, &ack, sizeof(ack));
  printf("subserver received: %d (ACK)\n", ack);
  if (ack != syn_ack + 1) err();

  printf("handshake complete\n");

}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;

  int pid = getpid();
  char* PP = (char*) malloc(20 * sizeof(char));
  sprintf(PP, "%d", pid);

  // making PP
  mkfifo(PP, 0644);
  printf("client made PP %s\n", PP);

  // opening WKP
  *to_server = open(WKP, O_WRONLY);
  printf("client opened WKP (WKPid = %d)\n", *to_server);

  // writing PP to WKP
  write(*to_server, PP, strlen(PP));
  printf("client sent: %s\n", PP);

  // opening PP
  from_server = open(PP, O_RDONLY);
  printf("client opened PP (PPid = %d)\n", from_server);

  // deleting PP
  remove(PP);
  printf("client removed PP (PPid = %d)\n", from_server);

  // reading SYN_ACK
  int syn_ack;
  read(from_server, &syn_ack, sizeof(syn_ack));
  printf("client received: %d (SYN_ACK)\n", syn_ack);

  // sending ACK
  int ack = syn_ack + 1;
  write(*to_server, &ack, 4);
  printf("client sent: %d (ACK)\n", ack);

  return from_server;
}
