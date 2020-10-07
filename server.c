#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

float getCPUUsage() {
  long double a[4], b[4];
  float loadavg;
  FILE *fp;

  fp = fopen("/proc/stat","r");
  fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
  fclose(fp);
  sleep(1);

  fp = fopen("/proc/stat","r");
  fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
  fclose(fp);

  loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
  return loadavg;
}

/**
 * Server runs in the compute node.
 * Client runs in the master node.
 * Each time when the user starts the client in the master node,
 * the client send a signal to the server.
 * Then the server on the compute node collects the CPU loading
 * and send back to the client on the master node.
 * The client on the master node prints the report.
 */

int main(int argc , char *argv[])

{
    // create socket
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
    }
    // connect socket 
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8700);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    char hostname[64] = {};
    gethostname(hostname, sizeof(hostname));

    /* char inputBuffer[256] = {}; */
    char message[128] = {};
    int op_flag = -1, err = -1;
    float cpu_load;
    while(1){
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        /* err = 10; */
        /* send(forClientSockfd,&err,sizeof(err),0); */
        /* printf("send: %d\n",err); */
        recv(forClientSockfd,&op_flag,sizeof(op_flag),0);
        printf("recv: %d\n",op_flag);
        if (op_flag==1) {
          // Get CPU load and send back
          cpu_load = getCPUUsage();
          err = sprintf(message, "%s: CPU usage = %.2f%%", hostname, cpu_load*100);
          send(forClientSockfd,message,sizeof(message),0);
          printf("send: %s\n",message);
        }
        /* recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); */
    }
    return 0;
}
