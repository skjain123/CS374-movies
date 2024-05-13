/* 
Author: Sunil Jain
Class: Operating Systems 1 or CS 374
Description: A program that parses a CSV for data about movies (title, year released, 5 or less languages 
             the movie was released in, and the rating value of the movie). This program allows the user to search movies by year, 
             languages, and shows the highest rating movie per year (1900-2021).
Date: 4/16/2024
 */

#ifndef PROCESS_MOVIES
#define PROCESS_MOVIES 

/* Movie Structure! */
struct Movie {
    char *title;
    int year;
    char languages[5][20];
    double rating;
    struct Movie *next;
};

/* Functions relating to processing the CSV File */
struct Movie *createMovie(char *currLine);
struct Movie *processFile(char *filePath);

/* Functions relating to printing movies. */
void printMovie(struct Movie *currMovie);
void printMovieList(struct Movie *list);

/* Functions relating to Searching the movies */
void searchMoviesYear(int yearInput, struct Movie *list);
void searchMoviesHighestRating(struct Movie *list, char* dirPath);
void searchMoviesLanguage(char* lang, struct Movie *list);

/* Functions relating to getting user input */
int validInput(int userInput);
int userSelection();
int specifyYear();
void getUserInput();

/* Functions relating to freeing data at the program end. */
void freeList(struct Movie* head);

#endif