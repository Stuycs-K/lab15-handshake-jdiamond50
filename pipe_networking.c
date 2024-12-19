#include "pipe_networking.h"
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
  int mkfifoOut = mkfifo(WKP, 644);
  printf("mkfifoOut = %d\n", mkfifoOut);

  // opening WKP
  int WKPid = open(WKP, O_RDONLY);

  // reads PP from WKP
  char* PP = (char*) malloc(20 * sizeof(char));
  read(WKPid, PP, 20);
  printf("server received: %s\n", PP);

  // remove WKP
  remove(WKP);

  // open and return PP
  from_client = open(PP, O_WRONLY);

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
  int from_client;

  server_setup();



  return from_client;
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

  // opening WKP
  int WKPid = open(WKP, O_WRONLY);

  // writing PP to WKP
  write(WKPid, PP, sizeof(WKPid));

  // opening PP
  int PPid = open(PP, O_RDONLY);

  return from_server;
}
