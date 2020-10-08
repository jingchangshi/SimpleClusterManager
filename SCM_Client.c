#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

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
  for(int i=0;i<n_socket;i++) {
    err = sprintf(ip_str, "192.168.1.%d", 101+i);
    info.sin_port = htons(8701+i);
    info.sin_addr.s_addr = inet_addr(ip_str);
    err = connect(sockfd[i],(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
      printf("Connection error");
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

  pthread_t threads[n_socket];
  for(int i=0;i<n_socket;i++) {
    int ret = pthread_create(&threads[i], NULL, collectInfo1Node, &sockfd[i]);
    if(ret!=0){
      printf ("Create pthread error!n");
      exit (1);
    }
  }
  pthread_exit(NULL);

  printf("close Socket\n");
  close(sockfd);
  return 0;
}


