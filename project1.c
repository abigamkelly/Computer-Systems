/* Abigail Kelly 
 * Project 1
 * CSCI 3240-001
 * This program will mimic the "xxd <filename>" command and produce
 * the hex values of the characters in the <filname> and display the
 * file as well */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE* ptr;                      /* pointer to file */
    char c;                         /* each character in file */
    int counter = 0;                /* keep count of characters */
    int row = 0;                    /* keep count of rows */
    int i;                          /* loop variable */
    char characters[16];            /* holds line of 16 characters */

    ptr = fopen(argv[1], "r");      /* open the file */
 
    if (ptr == NULL)                /* test if the file can be opened */
    {
        printf("The file cannot be opened!\n");
    }
    
    c = fgetc(ptr);                 /* get the first character in the file */
    printf("%08x:", row);          /* print the first line number */

    while(c != EOF)                 /* iterate thru file until the end is reached */
    {
        characters[counter] = c;
        if(counter % 2 == 0)        /* if the number of ascii values are even, print a space */
        {
            printf(" ");
        }

        printf("%02x", c);          /* print the ascii value */
        
        counter++;                  /* increment variables */
        row++;

        if(counter == 16)           /* once 16 characters are read, print them and go to next line*/
        {
            counter = 0;
            printf("  ");
            for(i = 0; i < 16; i++)
            {
                 if(isprint(characters[i]) != 0)    /* if character is not a control character, print it*/
                {
                    printf("%c", characters[i]);
                }
                else                /* if character is control character, print a '.' instead */
                {
                    printf(".");
                }
            }

    
        }
        
        c = fgetc(ptr);     /* get next character from file */

        if(c != EOF && counter == 0) {  /* print the line numbers */
            printf("\n");
            printf("%08x:", row);  /* print the line number */
        }
    }

    if(counter != 16)   /* if last line of output is not "full" of hex ascii */
    {
        int temp = counter;
        int spaces = 41;    /* number of characters (including spaces) that fill up a line*/
        int c = 0;          /* loop control variable */
        int j = 1;          /* loop control variable */

        while(j <= counter) {   /* calculate how many spaces there needs to be */
            if(j % 2 == 0) {
                spaces -= 3;
            }
            else {
                spaces -= 2;
            }

            j++;
        }

        while(c < spaces) {     /* print spaces */
            printf(" ");
            c++;
        }

        if(counter % 2 == 0) {  /* if the number of pairs of hex digits are even, print extra space*/
            printf(" ");
        }
        
        for(i = 0; i < temp; i++)   /* print last line of characters */
        {
            if(isprint(characters[i]) != 0)
            {
                printf("%c", characters[i]);
            }
            else                    /* if character is control variable, print "." instead */
            {
                printf(".");
            }
        }
    }

    printf("\n");
    fclose(ptr);                    /* close the file */

    return 0;
}