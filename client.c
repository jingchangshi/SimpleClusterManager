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

int main(int argc , char *argv[])
{

    // create socket
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    // connect socket
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    /* info.sin_addr.s_addr = inet_addr("127.0.0.1"); */
    info.sin_addr.s_addr = inet_addr("192.168.1.100");
    info.sin_port = htons(8700);
    int err;
    err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
    }

    float cpu_load;
    cpu_load = getCPUUsage();

    char hostname[64];
    gethostname(hostname, 64);
    //Send a message to server
    char message[128];
    err=sprintf(message, "%s: CPU usage = %.2f%%", hostname, cpu_load*100);
    char receiveMessage[128] = {};
    send(sockfd,message,sizeof(message),0);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);

    printf("%s",receiveMessage);
    printf("close Socket\n");
    close(sockfd);
    return 0;
}


