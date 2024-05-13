/* 
Author: Sunil Jain
Class: Operating Systems 1 or CS 374
Description: A program that parses a CSV for data about movies (title, year released, 5 or less languages 
             the movie was released in, and the rating value of the movie). This program allows the user to search movies by year, 
             languages, and shows the highest rating movie per year (1900-2021).
Date: 4/16/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "movies.h"

int main(int argc, char *argv[]) {
    
    /* getting the file to open. */
    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info.csv\n");
        return EXIT_FAILURE;
    }

    /* processing the CSV into Movie structures */
    struct Movie *list = processFile(argv[1]);

    /* getting valid user input until the answer is 4 (termination) */
    getUserInput(list);

    /* Freeing all the memory */
    freeList(list);

    return EXIT_SUCCESS;
}
