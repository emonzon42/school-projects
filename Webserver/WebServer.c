/* 
Eli Monzon 
4.16.20
ICSI 333
WebServer
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <netdb.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MSG_MORE	0x8000 //doesn't compile unless i do this
#define PORT "8000"

sem_t * sem;

typedef struct params{
    int csock;//client socket fd
    char *servepath; //path for files for server
}Param;

void* request(void * param){//handles requests in server
    Param *p = (Param *) param;
    //printf("Hi! I am serving socket#%d\n",p->csock);
    char message[3000];
    int msgSize;
    int err; //for storing errno
    printf("\nincoming message...\n\n");

    sem_wait(sem);
    //while ((msgSize = recv(p->csock, message, sizeof(message)-1, 0)) != 0 && msgSize > -1);
    msgSize = recv(p->csock, message, sizeof(message)-1, 0);
    sem_post(sem);
    if (msgSize < 0){
        err = errno;
        printf("message wasnt recieved: error code %d: %s\n", err, strerror(err));
        close(p->csock);
        return NULL;
    }else if(msgSize == 0){
        printf("message came through with no data. \n");
        close(p->csock);
        return NULL;
    }
    //printf("size: %d\nmessage:\n%s\n",msgSize,message);

    //gets path specified by user by breaking message into tokens and finding the one w address
    char *afterPath = strstr(message, "HTTP/"); //to check if tokenizer passed the path req
    char *saveptr; //ptr to where tokenizer ended
    char* token = strtok_r(message," ", &saveptr);
    while (strncmp(token, afterPath,5) != 0){
        
        char firstChar = '/';
        if (strncmp(token, &firstChar,1) == 0){
            printf("requesting %s\n",token);
            break;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }
    
    char fullpath[230]; //used to open file
    strncat(fullpath,p->servepath,strlen(p->servepath));
    strncat(fullpath, token, strlen(token));

    int file; //file descriptor
    char *sendmsg = (char*) malloc(70 * sizeof(char)); //string for first message
    char contlength[45]; //string for content lengt
    if((file = open(fullpath, O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH)) >= 0){//file found

        struct stat file_stat;
        fstat(file, &file_stat);

        strcpy(sendmsg, "HTTP/1.0 200 OK\n");
        sprintf(contlength, "Content-Length:%d\n\n",(int)file_stat.st_size);
        strncat(sendmsg,contlength,strlen(contlength)+1);
        sendmsg = realloc(sendmsg, strlen(sendmsg));//get rid of unused space

        printf("Result: File Found.\n");
        printf("Sending header...\n");
        
        sem_wait(sem);
        while ((msgSize = send(p->csock, sendmsg, strlen(sendmsg), MSG_MORE)) != strlen(sendmsg) && msgSize > -1);
        sem_post(sem);
        //printf("%s",sendmsg);
        if (msgSize < 0){
            err = errno;
            printf("header wasnt sent: error code %d: %s\n", err, strerror(err));
            free(sendmsg);
            close(file);
            close(p->csock);
            return NULL;
        }
        
        printf("%d bytes were sent.\n", msgSize);

        printf("\nSending data...\n");
        
        char* adr = mmap(NULL, file_stat.st_size, PROT_READ, MAP_SHARED, file, 0);

        sem_wait(sem);
      // while((
        msgSize = send(p->csock,adr,file_stat.st_size,0);
        //) != file_stat.st_size && msgSize > -1);
        sem_post(sem);

        if (msgSize < 0){
                //printf("size: %d\n",msgSize);
                err = errno;
                printf("data wasnt sent: error code %d: %s\n", err, strerror(err));
                free(sendmsg);
                close(file);
                close(p->csock);
                return NULL;
            }

    } else { //file not found
        err = errno;
        if (err == ENOENT || err == EISDIR) {
            
            strcpy(sendmsg, "HTTP/1.0 404 File Not Found\n");
            sprintf(contlength, "Content-Length:%d\n\n",(int)sizeof("404 not found :("));
            strncat(sendmsg,contlength,strlen(contlength)+1);
            sendmsg = realloc(sendmsg, strlen(sendmsg));//get rid of unused space

            printf("Result: File Not Found.\n");
            printf("Sending data...\n");
            //sem_wait(sem);

            msgSize = send(p->csock, sendmsg, strlen(sendmsg), MSG_MORE);
            //sem_post(sem);
            if ((msgSize) != strlen(sendmsg))
                printf("error on send 1\n");
            printf("%d bytes were sent.\n", msgSize);
            
            //sem_wait(sem);
            msgSize = send(p->csock, "404 not found :(", strlen("404 not found :("), 0); //sent instead of file
            if ((msgSize) != sizeof("404 not found :(")-1)
                printf("error on send 2\n");
            //sem_post(sem);
            printf("%d bytes were sent.\n", msgSize);
            
            //printf("%s\n","404 not found :(");

        } else {
            printf("file open failed: error code %d: %s\n", err, strerror(err));
            close(p->csock);
            return NULL;
        }
    }
    printf("Done.\n");
    free(sendmsg);
    //shutdown(p->csock,SHUT_WR);
    close(p->csock);
    //p->csock = -1;
    close(file);
    return NULL;
}

void server(char * path){ //sets up server
    int sockfd, returnVal;
    struct addrinfo hints, *serverInfo, *currNode;
    struct sockaddr_storage clientaddr; 
    socklen_t sin_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    returnVal = getaddrinfo(NULL,PORT,&hints, &serverInfo);
    
    for(currNode = serverInfo; currNode != NULL; currNode = currNode->ai_next) {
		if ((sockfd = socket(currNode->ai_family, currNode->ai_socktype, 0)) == -1) 
            continue;            
		if (bind(sockfd, currNode->ai_addr, currNode->ai_addrlen) == -1) { 
            close(sockfd); 
            continue; 
        }
		break;
	}

    freeaddrinfo(serverInfo);
	if (listen(sockfd, 32) == -1){
        int err = errno;
        printf("listen failed: error code %d: %s\n", err, strerror(err));
        exit(1);
        
    }

    sem = sem_open("lock", O_CREAT, O_RDWR, 1);
    while(1) {  // main accept() loop
        Param p;
        sin_size = sizeof (clientaddr);
        p.csock = accept(sockfd, (struct sockaddr *)&clientaddr, &sin_size); 
        p.servepath = path;
        if (p.csock != -1){
            //printf("time to serve socket#%d\n",p.csock);
            pthread_t id;
            pthread_create(&id, NULL, request, (void*) &p);
        }
	}
    printf("\nserver closing :(\n");
    close(sockfd);
}

int main(int argc, char *argv[]){//entry to program

    char *path;
    if (argc < 2 || argc > 2){
        printf("\nYou entered a invalid # of args.\n");
        printf("Start like this: './WebServer (filepath)\n");
        return 1;
    }else{//user inputted correct amount of args  
        path = argv[1];
        printf("\nYour specified path is %s\n", path);
    }
    
    server(path);

    return 0;
}
