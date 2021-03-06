/******************************************************************************
 * Student Name    :  Dallas Hall
 * RMIT Student ID :  s3461243
 * COURSE CODE     :  CPT220 / OLA00
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * study period 2, 2019.
 *****************************************************************************/

#include "scoreboard.h"
#include "io.h"
#include "main.h"

const int DEBUGGING_SCOREBOARD = 1;

/**
 * initialise the scores menu with the text and functions that perform the
 * required functionality.
 **/
void init_scores_menu(struct scores_menu_entry scores_menu[])
{
	/*
	 * Copy the menu item description's in.
	 */
	strcpy(scores_menu[SM_PRINT].text, "Print the current scoreboard.");
	strcpy(scores_menu[SM_ADD].text, "Add a new score to the scores list.");
	strcpy(scores_menu[SM_DELETE_ONE].text,
		   "Delete a score from the scores list");
	strcpy(scores_menu[SM_DELETE_ALL].text,
		   "Remove all scores from the scores list.");
	strcpy(scores_menu[SM_SAVE].text,
		   "Save scores back to the current file.");
	strcpy(scores_menu[SM_SAVE_NEW].text, "Save scores to a new file name.");

	/*
	 * Assign the function pointers.
	 */
	scores_menu[SM_PRINT].function = print_scores;
	scores_menu[SM_ADD].function = add_score;
	scores_menu[SM_DELETE_ONE].function = delete_score;
	scores_menu[SM_DELETE_ALL].function = remove_all_scores;
	scores_menu[SM_SAVE].function = resave_scores;
	scores_menu[SM_SAVE_NEW].function = save_scores;
	/*
	 * There is no quit function, just call the main menu print function.
	 */
}

/**
 * manages user input via the scores menu
 **/
void scores_menu(struct game_system* thesystem)
{
	int i;
	int choice;
	enum input_result inputResult;
	BOOLEAN menuResult;

	/*
	 * This is an infinite loop but that is okay because the user will exit
	 * this.
	 */
	while (1) {
		puts("\nScores Menu");
		PUTCHARS('=', strlen("Scores Menu"));
		printf("\n");

		for (i = 0; i < NUM_SCORES_MENU_ITEMS; i++) {
			printf("%d) %s\n", i + 1, thesystem->the_menus.scores_menu[i].text);
		}
		printf("%d) Quit to the main menu.\n", i + 1);

		inputResult = read_int("Enter the number of your choice", &choice);

		if (inputResult == IR_FAILURE) {
			continue;
		}
		else if (inputResult == IR_SKIP_TURN) {
			/*
			 * Will return back the main menu on enter.
			 */
			return;
		}
		else if (inputResult == IR_QUIT) {
			/*
			 * Will return back the main menu on ^D.
			 *
			 * Need clear_buffer here so the ^D isn't sent back to the main
			 * menu and thus exiting the program. This means ^D will only exit
			 * the sub menu instead of the entire program.
			 */
			clear_buffer();
			return;
		}
		else {
			switch (choice) {
				case 1:

					/*
					 * We don't care about the result here because a false
					 * is returned when the scoreboard is empty.
					 *
					 * For everything else we care.
					 */
					menuResult = thesystem->the_menus.scores_menu[SM_PRINT].function(
							thesystem);
					break;
				case 2:
					menuResult = thesystem->the_menus.scores_menu[SM_ADD].function(
							thesystem);

					if (!menuResult) {
						fprintf(stderr, "Couldn't add score to scoreboard.\n");
					}
					else {
						puts("Score added to the scoreboard.");
					}
					break;
				case 3:
					menuResult = thesystem->the_menus.scores_menu[SM_DELETE_ONE].function(
							thesystem);

					if (!menuResult) {
						fprintf(stderr,
								"Couldn't delete score from scoreboard.\n");
					}
					else {
						puts("Score deleted from the scoreboard.");
					}
					break;
				case 4:
					menuResult = thesystem->the_menus.scores_menu[SM_DELETE_ALL].function(
							thesystem);

					if (!menuResult) {
						fprintf(stderr,
								"Couldn't delete all scores from scoreboard.\n");
					}
					else {
						puts("All scores deleted from the scoreboard.");
					}
					break;
				case 5:
					menuResult = thesystem->the_menus.scores_menu[SM_SAVE].function(
							thesystem);

					if (!menuResult) {
						fprintf(stderr,
								"Couldn't save scoreboard to the same file.\n");
					}
					else {
						puts("All scores saved to disk.");
					}
					break;
				case 6:
					menuResult = thesystem->the_menus.scores_menu[SM_SAVE_NEW].function(
							thesystem);

					if (!menuResult) {
						fprintf(stderr,
								"Couldn't save scoreboard to the new file.\n");
					}
					else {
						puts("All scores re-saved to disk.");
					}
					break;
				case 7:
					/*
					 * Will return back the main menu on 7.
					 */
					puts("Returning to main menu.");
					return;
				default:
					fprintf(stderr, "Invalid choice, try again.\n");
			}
		}
	}
}

/**
 * performs the display scores function for the scores menu
 **/
BOOLEAN print_scores(struct game_system* thesystem)
{
	int lineNumber;
	int i;
	int numberOfDigits;
	int numberOfDigitsListSize;
	int nameLength;
	int totalRowLength;
	struct node* currentNode;

	numberOfDigitsListSize = getDigitAmount(thesystem->scoreboard.size);
	numberOfDigits = numberOfDigitsListSize + ROW_NUMBER_EXTRA_CHARS;
	nameLength = NAME_LEN;
	totalRowLength =
			numberOfDigits + NAME_LEN + TABLE_DELIMITER + NAME_LEN +
			TABLE_DELIMITER + WON_BY_AMOUNT;

	/*
	 * Check if the list is empty.
	 */
	if (thesystem->scoreboard.size <= 0) {
		fprintf(stderr, "%s", "[WARNING] Trying to print an empty scoreboard.");
		return FALSE;
	}

	/*
	 * Print the header.
	 * I was thinking about modularising this but I didn't really see a
	 * point making a function for this.
	 * 
	 * Print the first 2 lines
	 *
	 * 1) The Scoreboard
	 * 2) ==============
	 */
	puts("\nThe Scoreboard");
	PUTCHARS('=', strlen("The Scoreboard"));
	printf("\n");

	/*
	 * Print the Winner heading followed by delimiter.
	 * The offset is how many characters the largest row number will
	 * take up when printing.
	 */
	for (i = 0; i < numberOfDigits; i++) {
		printf(" ");
	}

	printf("Winner");

	numberOfDigits = strlen("Winner");
	for (i = 0; i < nameLength - numberOfDigits; i++) {
		printf(" ");
	}

	printf("|");

	/*
	 * Print the Loser heading followed by delimiter.
	 */
	printf("Loser");

	numberOfDigits = strlen("Loser");
	for (i = 0; i < nameLength - numberOfDigits; i++) {
		printf(" ");
	}

	printf("|");

	/*
	 * Print the Amount Won By heading.
	 */
	puts("Amount Won By");

	/*
	 * Print the header underline
	 * --------------------------
	 */
	PUTCHARS('-', totalRowLength);
	printf("\n");

	/*
	 * Print the list.
	 */
	currentNode = thesystem->scoreboard.head;
	lineNumber = 1;
	while (currentNode != NULL) {
		/*
		 * Print the
		 * 1) optional preceding space(s)
		 * 2) row number
		 * 3) )
		 * 4) trailing space.
		 */
		numberOfDigits = getDigitAmount(lineNumber);

		/*
		 * The original printing offset only works for single digit numbers.
		 * As the number of digits grows, the offset needs to be adjusted.
		 * Through trial and error, the adjustment that I found that works with
		 * the provided files (10, 100, 1000, 10000) is below.
		 */
		numberOfDigits = (numberOfDigitsListSize - numberOfDigits);

		for (i = 0; i < numberOfDigits; i++) {
			printf(" ");
		}

		printf("%d) ", lineNumber);

		/*
		 * Print
		 * 1) The winner's name
		 * 2) The spaces
		 * 3) The table delimiter
		 */
		printf("%s", currentNode->data->winner);
		numberOfDigits = (int) strlen(currentNode->data->winner);

		for (i = 0; i < nameLength - numberOfDigits; i++) {
			printf(" ");
		}
		printf("|");

		/*
		 * Print
		 * 1) The losers' name
		 * 2) The spaces
		 * 3) The table delimiter
		 */
		printf("%s", currentNode->data->loser);
		numberOfDigits = (int) strlen(currentNode->data->loser);

		for (i = 0; i < nameLength - numberOfDigits; i++) {
			printf(" ");
		}
		printf("|");

		/*
		 * Print winning margin and newline
		 */
		printf("%d\n", currentNode->data->won_by);

		currentNode = currentNode->next;
		++lineNumber;
	}

	/*
	 * Print the bottom of the table underline
	 * ---------------------------------------
	 */
	PUTCHARS('-', totalRowLength);
	printf("\n");

	return TRUE;
}

/**
 * adds a score to the system via user input. Please note that in a real
 * game score menu this makes no sense but it's here so you can show you
 * can do it.
 **/
BOOLEAN add_score(struct game_system* thesystem)
{
	char input[MAXPROMPTLEN];
	char winnerName[NAME_LEN + FGETS_EXTRA_CHAR];
	char loserName[NAME_LEN + FGETS_EXTRA_CHAR];
	int winningMargin;
	enum input_result inputResult;

	printf("Add a score\n");
	PUTCHARS('-', strlen("Add a score"));
	printf("\n");

	while (1) {
		/*
		 * Clear the buffer
		 */
		memset(input, 0, MAXPROMPTLEN);

		/*
		 * Using Paul's read_string (io.c) so I don't need to reinvent the wheel.
		 * It handles a lot of things like clearing buffers, input length
		 * validation, etcetera..
		 */
		inputResult = read_string("Please enter the winner of the game", input,
								  20);

		/*
		 * ^D is IR_QUIT
		 * Enter is IR_SKIP_TURN
		 *
		 * Both will exit
		 */
		if (inputResult == IR_QUIT || inputResult == IR_SKIP_TURN) {
			fprintf(stderr,
					"[WARNING] Exit condition met, did not update the scoreboard.\n");
			return FALSE;
		}

		/*
		 * If we got an input failure, try again.
		 */
		if (inputResult == IR_FAILURE) {
			continue;
		}

		if (validInputName(input)) {
			/*
			 * Remove garbage
			 */
			memset(winnerName, 0, NAME_LEN + FGETS_EXTRA_CHAR);
			strcpy(winnerName, input);
			break;
		}
	}

	while (1) {
		memset(input, 0, MAXPROMPTLEN);

		inputResult = read_string("Please enter the loser of the game", input,
								  20);

		if (inputResult == IR_QUIT || inputResult == IR_SKIP_TURN) {
			fprintf(stderr,
					"[WARNING] Exit condition met, did not update the scoreboard.\n");
			return FALSE;
		}

		if (inputResult == IR_FAILURE) {
			continue;
		}

		if (validInputName(input)) {
			memset(loserName, 0, NAME_LEN + FGETS_EXTRA_CHAR);
			strcpy(loserName, input);
			break;
		}
	}

	while (1) {
		inputResult = read_int("Please enter the amount the game was won by",
							   &winningMargin);

		if (inputResult == IR_QUIT || inputResult == IR_SKIP_TURN) {
			fprintf(stderr,
					"[WARNING] Exit condition met, did not update the scoreboard.\n");
			return FALSE;
		}

		if (inputResult == IR_FAILURE) {
			continue;
		}

		/*
		 * Check for invalid numbers, if we got it, try again.
		 * If not, break out of the loop.
		 */
		if (winningMargin <= 0 || winningMargin > 15) {
			error_print(
					"Invalid input in the third token, should be between 1 and 15 but was %ld.\n",
					winningMargin);
			continue;
		}
		else {
			break;
		}
	}

	updateScoreboardManually(winnerName, loserName, winningMargin, thesystem);

	return TRUE;
}

/**
 * handles the "delete node" submenu for the system. Again, this would not
 * be a likely part of such a program in the real world but we are getting
 * you do do this so you can show you know how.
 **/
BOOLEAN delete_score(struct game_system* thesystem)
{
	enum input_result inputResult;
	int nodeToDelete;

	print_scores(thesystem);
	while (1) {
		inputResult = read_int("What score do you want to delete",
							   &nodeToDelete);

		if (inputResult == IR_QUIT || inputResult == IR_SKIP_TURN) {
			fprintf(stderr,
					"[WARNING] Exit condition met, did not update the scoreboard.\n");
			return FALSE;
		}

		if (inputResult == IR_FAILURE) {
			continue;
		}

		if (nodeToDelete <= 0 || nodeToDelete > thesystem->scoreboard.size) {
			error_print(
					"Invalid input in the third token, should be between 1 and %d but was %ld.\n",
					thesystem->scoreboard.size, nodeToDelete);
			continue;
		}
		else {
			/*
			 * Try to delete the node.
			 */
			if (deleteNodeViaPosition(&thesystem->scoreboard, nodeToDelete)) {
				return TRUE;
			}
			else {
				puts("Scored deleted.");
				return FALSE;
			}
		}
	}
}

/**
 * clears out the scoreboard to make it an empty list
 **/
BOOLEAN remove_all_scores(struct game_system* thesystem)
{
	BOOLEAN result;

	result = deleteLinkedListNodes(&thesystem->scoreboard);

	return result;
}

/**
 * save scores back to the same file it was loaded from
 **/
BOOLEAN resave_scores(struct game_system* thesystem)
{
	BOOLEAN result;

	result = save_data(thesystem->datafile, &thesystem->scoreboard);

	return result;
}

/**
 * saves the data file to a new file. The datafile attribute of the game system
 * should now point to the path of the file the data has been saved to. This is
 * similar to "save as" functionality in a word processor.
 **/
BOOLEAN save_scores(struct game_system* thesystem)
{
	BOOLEAN result;
	enum input_result inputResult;
	char outputFilePath[PATH_MAX + FGETS_EXTRA_CHAR];
	/*char* absolutePathPtr;*/

	while (1) {
		inputResult = read_string(
				"Please enter the name of the new filename (absolute or relative path is allowed)",
				outputFilePath, PATH_MAX + FGETS_EXTRA_CHAR);

		if (inputResult == IR_FAILURE) {
			continue;
		}
		else if (inputResult == IR_SKIP_TURN) {
			/*
			 * Will return back the main menu on enter.
			 */
			return FALSE;
		}
		else if (inputResult == IR_QUIT) {
			/*
			 * Will return back the main menu on ^D.
			 */
			return FALSE;
		}
		else {
			if (strlen(outputFilePath) > PATH_MAX) {
				error_print(
						"Output file path is too long, must be <= 4096 characters. Try again.\n");
			}
			else {
				/*
				 * Free the old file name as it was made with strdup which
				 * has malloc. So we don't want a memory leak.
				 *
				 *  Need to cast to (void*) avoid a compiler warning.
				 *
				 * Convert to an absolute, this also uses strdup so we need
				 * to free after.
				 */
				free((void*) thesystem->datafile);
				thesystem->datafile = strdup(outputFilePath);
				/*absolutePathPtr = getAbsolutePath(outputFilePath);
				thesystem->datafile = strdup(absolutePathPtr);
				free(absolutePathPtr);*/

				/*
				 * Try to save the file and then return its result.
				 */
				result = save_data(thesystem->datafile, &thesystem->scoreboard);

				return result;
			}
		}
	}
}

/*
 * Tries to create a game result so it can be inserted into the linked list.
 * This expects the game result data to be validated already.
 */
struct game_result*
createGameResult(char* winner, char* loser, int winningMargin)
{
	struct game_result* gameResultPtr;

	/*
	 * malloc tries to allocate memory with the specified bytes.
	 * sizeof will return the size of the data structure, platform
	 * dependent. If successful, malloc returns a void * to the
	 * allocated memory, otherwise returns NULL
	 * We are using struct game_result because that is the object
	 * hat will be stored in the pointer.
	 *
	 * Paul tends to cast the void* returned by malloc.
	 */
	gameResultPtr = malloc(sizeof(struct game_result));

	/*
	 * Same as linkedListPtr == NULL. I tend to use both.
	 */
	if (!gameResultPtr) {
		perror("malloc");
		return NULL;
	}

	/*
	 * memset copies a byte value for n bytes into a specified object
	 * arg 1) the object to copy into
	 * arg 2) the byte to copy
	 * arg 3) how many bytes in the object to copy into
	 *
	 * We are using struct game_result because that is the object that will
	 * be stored in the pointer.
	 *
	 * This is called zeroing out the memory. I believe this means:
	 * int = 0
	 * ptr = NULL
	 * char = '\0'
	 *
	 * Not sure if this is even necessary at this point? Every time I
	 * inspected these variables there were already zeroed.
	 */
	memset(gameResultPtr, 0, sizeof(struct game_result));

	gameResultPtr->winner = winner;
	gameResultPtr->loser = loser;
	gameResultPtr->won_by = winningMargin;

	return gameResultPtr;
}

/*
 * Tries to create a game result and insert it into the scoreboard. This expects
 * the input to be validated already.
 */
BOOLEAN
updateScoreboardManually(char* winnerName, char* loserName, int winningMargin,
						 struct game_system* gameSystem)
{
	struct game_result* gameResultPtr;
	struct node* linkedListNodePtr;
	struct linkedlist* linkedListPtr;
	char* validatedWinnersName;
	char* validatedLosersName;

	linkedListNodePtr = createLinkedListNode();

	if (!linkedListNodePtr) {
		fprintf(stderr, "[ERROR] Couldn't create the linked list node.\n");
		return FALSE;
	}

	/*
	 * We need to free this later as strdup has its own malloc call.
	 */
	validatedWinnersName = strdup(winnerName);
	validatedLosersName = strdup(loserName);

	gameResultPtr = createGameResult(validatedWinnersName, validatedLosersName,
									 winningMargin);

	if (!gameResultPtr) {
		fprintf(stderr, "[ERROR] Couldn't create the game result.\n");
		return FALSE;
	}

	/*
	 * Update the linked list node payload and link members.
	 */
	linkedListNodePtr->data = gameResultPtr;
	linkedListNodePtr->next = NULL;

	/*
	 * Grab the address of the scoreboard from the game_system pointer.
	 * This is the bit that I needed to use the global variable for
	 * game_system.
	 */
	linkedListPtr = &gameSystem->scoreboard;

	/*
	 * Try to update the scoreboard
	 */
	if (!insertNode(linkedListPtr, linkedListNodePtr)) {
		error_print("Couldn't insert into the linked list.\n");
		return FALSE;
	}

	return TRUE;
}

/*
 * Works out how many characters are inside an int.
 * e.g. there are 2 characters in the int 23.
 */
int getDigitAmount(int linkedListSize)
{
	int offset;
	int digits;

	offset = 0;
	digits = linkedListSize;

	if (linkedListSize > 0) {
		/*
		 * Need to set it one here in case there is only 1 digit.
		 */
		offset = 1;

		while (digits > 9) {
			/*
			 * Use division by 10 to remove 1 digit at a time.
			 */
			digits /= 10;
			++offset;
		}
	}

	return offset;
}