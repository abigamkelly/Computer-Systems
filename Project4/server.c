/* 
 * Project 4
 * Dr. Sainju
 * server.c - An iterative echo server 
 */ 

#include "csapp.h"

/* function prototypes */
void choice1(char buff[], char notfound[], int connfd);
void *thread(void *vargp);

int main(int argc, char **argv) {
    /* create socket */
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */
    pthread_t tid;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    /* if port is in use */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* listen for connection */
    listenfd = Open_listenfd(argv[1]);

    /* loops and listens and accepts connections from clients */
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);  /* get length of client address */
        connfdp = malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);  /* accept the connection */
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}

void *thread(void *vargp) { 
    /* initialize buffer and rio */
    char buff[MAXLINE];
    char choice;    /* will hold first character in string received from client */
    memset(buff, 0, sizeof(buff));    
    rio_t rio;
    
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self());
    Free(vargp);

    char toappend[MAXLINE];     /* will hold matching names found in file */
    char success[MAXLINE] = {"Record added successfully!!"};    /* success message */
    char notfound[MAXLINE] = {"No record found!!"};     /* no matches found */

    /* initialize network stream */
    Rio_readinitb(&rio, connfd);

    /* read record from client (buffer) */
    Rio_readlineb(&rio, buff, MAXLINE);
    choice = buff[0];   /* first character is the choice */
    memmove(buff, buff+1, strlen(buff));    /* remove the first character
                                               and shift entire string */
    /* will iterate until choice == NULL, the choice will be NULL
       when the user enters 2, since the client will not send a
       buffer with information to the server */
    while(choice != NULL) {
        /* if the choice is 1, search the file for a matching record */
        if(choice == '1') {
            choice1(buff, notfound, connfd);
        }
        else if(choice == '2') {
                rio_writen(connfd, "Connection Closed!!", MAXLINE);
                Close(connfd); /* close the connection */
                break;
        }

        /* read record from client (buffer) */
        Rio_readinitb(&rio, connfd);
        Rio_readlineb(&rio, buff, MAXLINE);
        choice = buff[0];   /* first character is the choice */
        memmove(buff, buff+1, strlen(buff));    /* remove the first character
                                                   and shift entire string */
    }
    printf("Connection terminated by client\n");
    return NULL;
}

/* this function will be called when the user enters '1'
   studentRecords.txt will be searched for a matching
   record and the matching records will be returned to 
   the client */
void choice1(char buff[], char notfound[], int connfd) {
    FILE *fptr;
    fptr = fopen("studentRecords.txt", "r");

    /* if file open unsuccessful, exit program */
    if(fptr == NULL){
        printf("File open error! Program exiting...\n");
        exit(0);
    }

    char contents[MAXLINE];     /* hold contents of file */     
    char matches[MAXLINE];      /* hold matching records */

    int flag = 0;   /* to keep track of if any matches were found*/
    strtok(buff, "\n"); /* removed '\n' from buffer */

    /* read entire file */
    while(fgets(contents, MAXLINE, fptr)){
        /* if the buffer matches the contents of the file */
        if (strncmp(contents, buff, strlen(buff)) == 0) {
            flag = 1;   /* means that a match has been found */
            strcat(matches, contents);  /* add matching record to matches */
        }
    }
    
    /* if not matches have been found, send no records found message back
       to client*/
    if(flag == 0) {
        Rio_writen(connfd, notfound, MAXLINE);
    }   /* if a record has been found, send the matching records to the
           client */
    else {
        Rio_writen(connfd, matches, MAXLINE);
    }
    memset(matches, 0, sizeof(matches));
    fclose(fptr);   /* close the file */
}