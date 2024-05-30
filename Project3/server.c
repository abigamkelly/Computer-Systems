
#include "csapp.h"

/* function prototypes */
void choice1(char buff[], char toappend[], char success[], int connfd);
void choice2(char buff[], char notfound[], int connfd);

int main(int argc, char **argv) 
{
    /* create socket */
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */
    char client_hostname[MAXLINE], client_port[MAXLINE];
    char choice;    /* will hold first character in string received from client */

    /* if port is in use */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* listen for connection */
    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage); 
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        
        /* initialize buffer and rio */
        char buff[MAXLINE];
        memset(buff, 0, sizeof(buff));    
        rio_t rio;

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
           when the user enters 3, since the client will not send a
           buffer with information to the server */
        while(choice != NULL) {
            if(choice == '1') { /* if the choice is 1, add the record to
                                   the file */
                choice1(buff, toappend, success, connfd);
            } /* if the choice is 2, search the file for a matching record */
            else if(choice == '2') {
                choice2(buff, notfound, connfd);
            }

            /* read record from client (buffer) */
            Rio_readinitb(&rio, connfd);
            Rio_readlineb(&rio, buff, MAXLINE);
            choice = buff[0];   /* first character is the choice */
            memmove(buff, buff+1, strlen(buff));    /* remove the first character
                                                       and shift entire string */
        }

        /* if the choice is NULL< close the connection and print message */
        Close(connfd);
        printf("(%s, %s) disconnected\n", client_hostname, client_port);
        //exit(0);
    }
}
/* $end echoserverimain */

/* this function will be called when the user enters '1'
   the record will be added to the studentRecords.txt file */
void choice1(char buff[], char toappend[], char success[], int connfd) {
    strcpy(toappend, buff);     /* copy buffer into toappend */
    strtok(toappend, "\n");     /* remove '\n' */

    FILE *fptr;
    fptr = fopen("studentRecords.txt", "a");

    /* if file open unsuccessful, exit program */
    if(fptr == NULL){
        printf("File open error! Program exiting...\n");
        exit(0);
    }

    /* add record to file */
    fprintf(fptr, "\n%s", toappend);

    /* set buffer to empty and copy success message to buffer */
    memset(buff, 0, sizeof(buff));
    strcpy(buff, success);
    Rio_writen(connfd, buff, MAXLINE);
    fclose(fptr);   /* close the file */
}

/* this function will be called when the user enters '2'
   studentRecords.txt will be searched for a matching
   record and the matching records will be returned to 
   the client */
void choice2(char buff[], char notfound[], int connfd) {
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