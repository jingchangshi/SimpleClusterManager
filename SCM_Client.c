#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>

#define YELLOW "\e[0;33m"
#define BRIGHTYELLOW "\e[0;93m"
#define RESET "\e[0m"

void* collectInfo1Node(int* sockfd) {

  char receiveMessage[128] = {};
  int op_flag = -1;
  op_flag = 1;

  send(*sockfd,&op_flag,sizeof(op_flag),0);
  /* printf("send: %d\n",op_flag); */
  recv(*sockfd,receiveMessage,sizeof(receiveMessage),0);
  printf("%s\n",receiveMessage);
  pthread_exit(NULL);

}

int main(int argc , char *argv[]) {

  printf("Simple Cluster Manager (SCM) shows CPU loading of compute nodes.\n");
  printf("SCM is implemented by Jingchang Shi.\n");
  printf("Go to https://github.com/desperadoshi/SimpleClusterManager/issues for issues.\n");
  printf(BRIGHTYELLOW);
  printf("It is possible that due to the bad network, you have to wait for max 2 seconds of timeout.\n");
  printf(RESET);

  int n_socket = 8;
  // create socket
  int sockfd[n_socket];
  for(int i=0;i<n_socket;i++) {
    sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd[i] == -1){
      printf("Fail to create a socket.");
    }
  }

  // connect socket
  struct sockaddr_in info;
  bzero(&info,sizeof(info));
  info.sin_family = PF_INET;

  struct timeval tv;
  socklen_t lon;
  int valopt;
  fd_set myset;
  long arg;
  int err;
  bool conn_ok;
  char ip_str[15] = {};
  int n_socket_conn = 0, n_socket_not_conn = 0;
  int socket_conn[n_socket];
  int socket_not_conn[n_socket];
  for(int i=0;i<n_socket;i++) {
    // Set non-blocking socket
    if ( (arg = fcntl(sockfd[i], F_GETFL, NULL)) < 0 ) {
      fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
      exit(0);
    }
    arg |= O_NONBLOCK; 
    if ( fcntl(sockfd[i], F_SETFL, arg) < 0 ) {
      fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
      exit(0);
    }
    // Connect
    err = sprintf(ip_str, "192.168.1.%d", 101+i);
    info.sin_port = htons(8701+i);
    info.sin_addr.s_addr = inet_addr(ip_str);
    err = connect(sockfd[i],(struct sockaddr *)&info,sizeof(info));
    // Check the status
    if(err<0){
      if (errno == EINPROGRESS) {
        /* fprintf(stderr, "EINPROGRESS in connect() - selecting\n"); */
        do {
          tv.tv_sec = 2; 
          tv.tv_usec = 0; 
          FD_ZERO(&myset); 
          FD_SET(sockfd[i], &myset); 
          err = select(sockfd[i]+1, NULL, &myset, NULL, &tv);
          if (err < 0 && errno != EINTR) {
            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
            exit(0);
          } else if (err > 0) {
            // Socket selected for write
            lon = sizeof(int);
            if (getsockopt(sockfd[i], SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
              fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
              exit(0);
            }
            // Check the value returned
            if (valopt) {
              /* fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt)); */
              /* exit(0); */
              conn_ok = false;
              break;
            }
            conn_ok = true;
            break;
          } else {
            // ??? Should I continue?
            /* fprintf(stderr, "Timeout in select() - Cancelling!\n"); */
            /* exit(0); */
            conn_ok = false;
            break;
          }
        } while (1);
      } else {
        /* fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); */
        /* exit(0); */
        conn_ok = false;
      }
    } else {
      conn_ok = true;
    }
    //
    if (conn_ok) {
      socket_conn[n_socket_conn] = i;
      n_socket_conn += 1;
    } else {
      socket_not_conn[n_socket_not_conn] = i;
      n_socket_not_conn += 1;
    }
    // Set to blocking mode again... 
    if ( (arg = fcntl(sockfd[i], F_GETFL, NULL)) < 0 ) {
      fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
      exit(0);
    }
    arg &= (~O_NONBLOCK);
    if( fcntl(sockfd[i], F_SETFL, arg) < 0) {
      fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
      exit(0);
    }
  } // for(int i=0;i<n_socket;i++)

  //Send a message to server
  /* Serial version */
  /* char receiveMessage[n_socket][128]; */
  /* int op_flag = -1; */
  /* op_flag = 1; */
  /* for(int i=0;i<n_socket;i++) { */
  /*   send(sockfd[i],&op_flag,sizeof(op_flag),0); */
  /*   printf("send: %d\n",op_flag); */
  /*   [> recv(sockfd,&err,sizeof(err),0); <] */
  /*   [> printf("recv: %d\n",err); <] */
  /*   recv(sockfd[i],receiveMessage[i],sizeof(receiveMessage[i]),0); */
  /*   printf("%s\n",receiveMessage[i]); */
  /* } */

  /* Serial version of pthread: BAD */
  /* pthread_t id; */
  /* for(int i=0;i<n_socket;i++) { */
  /*   int ret = pthread_create(&id, NULL, collectInfo1Node, &sockfd[i]); */
  /*   if(ret!=0){ */
  /*     printf ("Create pthread error!n"); */
  /*     exit (1); */
  /*   } */
  /*   pthread_join(id, NULL); */
  /* } */

  pthread_t threads[n_socket_conn];
  for(int j=0;j<n_socket_conn;j++) {
    int i = socket_conn[j];
    int ret = pthread_create(&threads[i], NULL, collectInfo1Node, &sockfd[i]);
    if(ret!=0){
      printf ("Create pthread error!n");
      exit (1);
    }
  }

  printf("=== Explanations begin ===\n");
  printf("CPU usage is in [0, 100%%]\n");
  printf("If CPU usage is about 85%% on a node,\n");
  printf("that node is probably running all its 40 CPU cores.\n");
  printf("If CPU usage is less than 1%% on a node,\n");
  printf("that node is probably occupied by nobody.\n");
  printf("=== Explanations end   ===\n");
  //
  for (int j=0; j<n_socket_not_conn; j++) {
    int i = socket_not_conn[j];
    printf(YELLOW);
    printf("node%d: Not connected!\n",i+1);
    printf(RESET);
  }
  pthread_exit(NULL);
  /* printf("close Socket\n"); */
  /* close(sockfd); */
  /* return 0; */
} // int main


