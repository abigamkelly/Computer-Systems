/* 
 * Project 4
 * Dr. Sainju
 * client.c - An echo client
 */

#include "csapp.h"
#include <stdio.h>

int main(int argc, char **argv) 
{
    char returnMessage[MAXLINE];    /* will hold message returned from server*/
    char input[MAXLINE]; /* will hold user input */
    char buf[MAXLINE];  /* buffer will hold user input concatenated together
                           to send to the server */

    /* initialize file descriptor, hostname, port, and rio variable */
    int clientfd;
    char *host, *port;
    rio_t rio;
    

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    /* create file descriptor and network stream */
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    /* will iterate until the user enters '3' */
    while(1) {
        /* prompt user for choice */
        printf("(1) Search record\n");
        printf("(2) Terminate\n");
        printf("Select an option [1 or 2]: ");
        fgets(input, MAXLINE, stdin);   /* get input */
        strtok(input, "\n");            /* remove '\n' */

         /* if the user enters '2', prompt for the first name and
             last name */
        if(strcmp(input, "1") == 0) {
            strcat(buf, input);         /* add choice to buffer */
            printf("Enter first name: ");
            fgets(input, MAXLINE, stdin);
            strtok(input, "\n");
            strcat(buf, input);         /* add first name to buffer */
            strcat(buf, ",");           /* add ',' to buffer */
            printf("Enter last name: ");
            fgets(input, MAXLINE, stdin);
            strcat(buf, input);         /* add last name to buffer */
        } /* if the user enters '3', close the connection */
        else if (strcmp(input, "2") == 0) {
            printf("Connection Closed!!\n");
            Close(clientfd);            /* close the connection */
            exit(0);
        } /* if the user enters an invalid option, print error message */
        else {
            printf("Invalid option. Please select option 1 or 2.\n");
            continue;
        }

        /* send record to server */
        Rio_writen(clientfd, buf, strlen(buf));

        /* get confirmation that record was sent successfully */
        Rio_readinitb(&rio, clientfd);
        Rio_readnb(&rio, buf, MAXLINE);
        strcpy(returnMessage, buf);     /* copy return message into returnMessage*/
        printf("Message from server:\n");   /* display message from server */
        printf("%s\n\n", returnMessage);

        /* clear buffer for next input */
        memset(buf, 0, MAXLINE);
    }
}
/* $end echoclientmain */