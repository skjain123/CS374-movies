/* 
Author: Sunil Jain
Class: Operating Systems 1 or CS 374
Description: A program that parses a CSV for data about movies (title, year released, 5 or less languages 
             the movie was released in, and the rating value of the movie). This program allows the user to search movies by year, 
             languages, and shows the highest rating movie per year (1900-2021).
Date: 4/16/2024
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "movies.h"

#define MAX_LANG_LENGTH 100

/**
 * Name: createMovie - creates movies and returns it to be added in a linked list.
 * Input: a single line from a CSV that holds movie data
 * Output: a movie filled with data from the CSV line.
 * PreConditions: The line is parsed using commas, and the languages are formatted as [1;2;3].
 * PostConditions: None.
*/
struct Movie *createMovie(char *currLine) {
    /* allocate memory to movie */
    struct Movie *currMovie = malloc(sizeof(struct Movie));

    char *saveptr;
    char *token;

    /* get the title */
	token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = strdup(token);

    /* get the year */
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    /* get the language(s) */
    token = strtok_r(NULL, ",", &saveptr);
    token++; /* get rid of the bracket [ */
    token[strlen(token) - 1] = '\0'; /* get rid of bracket ] */
    char *langToken = strtok(token, ";");
    int i = 0;
    char* blank = " "; /* initialize values */
    while (i < 5) { /* each movie can have up to 5 languages */
        strcpy(currMovie->languages[i], blank); /* initialize */
        if (langToken != NULL) {
            strcpy(currMovie->languages[i], langToken);
            langToken = strtok(NULL, ";");
        }
        i++;
    }

    /* get the rating */
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = atof(token);

    currMovie->next = NULL;

    return currMovie;
}

/**
 * Name: processFile - goes through the entire CSV file, creates movies, and adds them to a linked list.
 * Input: a file path going to a CSV file.
 * Output: a linked list of movies.
 * PreConditions: The filePath HAS to lead to a CSV file with proper formatting.
 * PostConditions: The linkedlist uses the movie structure itself.
*/
struct Movie *processFile(FILE *filePath) {
    /* open file */
    FILE *movieFile = fopen(filePath, "r");
    if (movieFile == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    /* for getline */
    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;

    struct Movie *head = NULL;
    struct Movie *tail = NULL;
	
    /* boolean flag that tells us if the program went through the first line or not. */
	int skipFirstLine = 0;

    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
        /* using the flag. */
		if (skipFirstLine) {
            /* initialize movie, parse data, insert data into movie */
			struct Movie *newNode = createMovie(currLine);

            /* covering cases if the list is empty. if not then set as the tail. */
			if (head == NULL) {
				head = newNode;
				tail = newNode;
			} else {
                /* insert normally */
				tail->next = newNode;
				tail = newNode;
			}
		}
        /* set flag */
		skipFirstLine = 1;
        
    }
    /* free allocated memory after we are done and close the file. */
    free(currLine);
    fclose(movieFile);

    /* return the linked list */
    return head;
}

/**
 * Name: printMovie - prints individual movies
 * Input: a movie.
 * Output: Printing the movie data to the console (title, year, languages, rating).
 * PreConditions: Movie is initialized and filled with values.
 * PostConditions: All data is presented in a readable way.
 * Comment: This function is not used but included for earlier debugging purposes.
*/
void printMovie(struct Movie *currMovie) {
    printf("Title: %s\n", currMovie->title);
    printf("Year: %d\n", currMovie->year);
    printf("Languages: ");
    for (int i = 0; i < 5 && strlen(currMovie->languages[i]) > 0; i++) {
        printf("%s", currMovie->languages[i]);
        if (strlen(currMovie->languages[i + 1]) > 0) {
            printf(" ");
        }
    }
    printf("\n");
    printf("Rating: %.1f\n\n", currMovie->rating);
}

/**
 * Name: printMovieList - goes through entire linked list and prints individual movies
 * Input: a linked list of movies that are properly initialized by createMovie() and processFile()
 * Output: Prints every movie's data.
 * PreConditions: the linked list uses the movie structure itself.
 * PostConditions: all data presented is readable and understandable.
*/
void printMovieList(struct Movie *list) {
    /* go through entire list and print */
    while (list != NULL) {
        printMovie(list);
        list = list->next;
    }
}

/**
 * Name: searchMoviesYear - searches the movie linked list by year and prints out movies that match.
 * Input: a integer year from the user, and the movie linked list.
 * Output: movie titles that were released during the inputtted year.
 * PreConditions: the integer year is a 4 digit year within the bounds of 1900-2021.
 * PostConditions: None.
*/
void searchMoviesYear(int yearInput, struct Movie *list) {
    /* did we find at least one movie? */
    int flag = 0;

    while (list != NULL) {
        /* go through the list and compare years if they match print the title.*/
        if (list->year == yearInput) {
            flag = 1;
            printf("%s\n", list->title);
        }

        /* iterate */
        list = list->next;
    }

    /* if no movies were found release statement */
    if (flag == 0) {
        printf("No data about movies released in the year %d\n", yearInput);
    }
}

/**
 * Name: searchMoviesHighestRating - prints out the highest rated movies of their year.
 * Input: a linked list of movies
 * Output: movies with the highest rating of thier year.
 * PreConditions: linked list is initialized, movies' years are within the bounds of 1900-2021.
 * PostConditions: Presented data is readable and within a decent format.
*/
void searchMoviesHighestRating(struct Movie *list) {
    struct Movie* current; /* move through the list */
    struct Movie* highestMovie; /* keep track of highest movie for that year's iteration thru the LL */
    double highest = -1.0; /* keep track of the highest score to compare */

    /* if we dont find any movies in the LL then print message. */
    int flag = 0;

    int i;
    /* for all years between 1900-2021, go thru the LL and find the highest corresponding rating for that year. */
    for (i = 1900; i <= 2021; i++) {
        current = list;
        highest = -1.0;
        while (current != NULL) {
            if (current->year == i) {
                if (current->rating > highest) {
                    highest = current->rating;
                    highestMovie = current;
                }
            }
            current = current->next;
        }
        if (highest >= 0.0) { /* if we didnt find any movie for that year dont print */
            flag = 1;
            printf("%d %.1f %s\n", highestMovie->year, highestMovie->rating, highestMovie->title);
        }
    }

    if (flag == 0) {
        printf("No Movies!\n");
    }
}

/**
 * Name: searchMoviesLanguage - prints out movies that match the inputted language
 * Input: a string of characters, and a linked list of movies
 * Output: the year and title of movies that are released in the user-specified language.
 * PreConditions: linked list is initialized, lang is less than 20 chars. 
 * PostConditions: Presented data is readable and in a decent format.
*/
void searchMoviesLanguage(char* lang, struct Movie *list) {
    /* did we find a corressponding movie */
    int flag = 0;

    int i; 
    /* go through list and search through each movie's langauges */
    while (list != NULL) {
        for (i = 0; i < 5 && list->languages[i]; i++) {
            if (strcmp(list->languages[i], lang) == 0) { /* if it matches print out */
                flag = 1;
                printf("%d %s\n", list->year, list->title);
            }
        }
        
        list = list->next;
    }

    if (flag == 0) { /* no movies applicable */
        printf("No data about movies released in %s\n", lang);
    }
}

/**
 * Name: validInput - checks for valid input and returns a boolean
 * Input: an integer user-input.
 * Output: if the input is an integer AND 1-4, return 1. ELSE: return 0.
 * PreConditions: User typed something, not empty character.
 * PostConditions: Follows proper boolean logic.
*/
int validInput(int userInput) {

    /* if the number is 0 or anything else than 1-4, return 0-bad, otherwise return 1-good */
    if (userInput == 0) {
        printf("Not a valid number. Please try again.\n\n");
        return 0;
    } else if (userInput < 1 || userInput > 4) {
        printf("You entered an incorrect choice. Try again.\n\n");
        return 0;
    } else {
        return 1;
    }

}

/**
 * Name: userSelection - prompts the user for an input, checks for validity, and returns it
 * Input: none
 * Output: a VALID user input.
 * PreConditions: The line is parsed using commas, and the languages are formatted as [1;2;3].
 * PostConditions: None.
*/
int userSelection() {

    int userInput;
    int c;

    do
    {
        /* get user input */
        printf("1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\n\nEnter a selection from 1-4: ");
        scanf("%d", &userInput);
        
        /* reset scanf */
        while ((c = getchar()) != '\n' && c != EOF);
    } while (validInput(userInput) == 0); /* repeatedly ask for an input until they give a valid input */
    
    return userInput;
}

/**
 * Name: specifyYear - gets the year input
 * Input: none
 * Output: an integer from the user
 * PreConditions: user enters a 4 digit number.
 * PostConditions: user enters a 4 digit number.
*/
int specifyYear() {
    
    int yearInput = 0;
    int c;
    /* get year */
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &yearInput);
    while ((c = getchar()) != '\n' && c != EOF); /* reset scanf */

    return yearInput;
}

/**
 * Name: specifyLanguage - gets the language input
 * Input: none
 * Output: a string of characters from the user
 * PreConditions: the max length of the language input is 100 characters.
 * PostConditions: user enters a 4 digit number.
*/
char* specifyLanguage() {

    /* assume the max characters of the language is 100 */
    char* input = malloc(MAX_LANG_LENGTH * sizeof(char)); 
    assert(input);

    // Prompt the user to enter a string
    printf("Enter the language for which you want to see movies: ");

    /* get the string input */
    fgets(input, MAX_LANG_LENGTH, stdin);

    /* remove any extra input */
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    return input;
}

/**
 * Name: getUserInput - gets the user input until the user inputs 4 (termination choice.)
 * Input: a linked list of movies
 * Output: movie information based on what the user wants to search by.
 * PreConditions: linked list is initialized.
 * PostConditions: all data is readable
*/
void getUserInput(struct Movie *list) {
    int userInput = 0;

    /* if the user enters 4, exit the program.  */
    while (userInput != 4) {
        userInput = userSelection();

        printf("\n");
        
        if (userInput == 1) { /* first selection */
            searchMoviesYear(specifyYear(), list);
        } else if (userInput == 2) { /* second selection */
            searchMoviesHighestRating(list);
        } else if (userInput == 3) { /* third selections */
            char* language = NULL;
            language = specifyLanguage();
            searchMoviesLanguage(language, list);
            free(language);
        } else if (userInput == 4) { /* end */
            printf("Good Bye.\n");
        }
        printf("\n");
    }
}

/**
 * Name: freeList - frees all data used
 * Input: none
 * Output: none
 * PreConditions: linked list uses malloc to create data
 * PostConditions: all data from all movies are freed.
*/
void freeList(struct Movie *list) { /* frees all aspects of a movie */
    struct Movie *nextNode;
    while (list != NULL) {
        nextNode = list->next;
        free(list->title);
        free(list);
        list = nextNode;
    }
}