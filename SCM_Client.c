#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define YELLOW "\e[0;33m"
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

int main(int argc , char *argv[])
{

  printf("Simple Cluster Manager (SCM) shows CPU loading of compute nodes.\n");
  printf("SCM is implemented by Jingchang Shi.\n");
  printf("Go to https://github.com/desperadoshi/SimpleClusterManager/issues for issues.\n");

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

  /* info.sin_addr.s_addr = inet_addr("127.0.0.1"); */

  int err;
  char ip_str[15] = {};
  int n_socket_conn = 0, n_socket_not_conn = 0;
  int socket_conn[n_socket];
  int socket_not_conn[n_socket];
  for(int i=0;i<n_socket;i++) {
    err = sprintf(ip_str, "192.168.1.%d", 101+i);
    info.sin_port = htons(8701+i);
    info.sin_addr.s_addr = inet_addr(ip_str);
    err = connect(sockfd[i],(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
      /* printf("Connection error\n"); */
      socket_not_conn[n_socket_not_conn] = i;
      n_socket_not_conn += 1;
    } else {
      socket_conn[n_socket_conn] = i;
      n_socket_conn += 1;
    }
  }

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
}


