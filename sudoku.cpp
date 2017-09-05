//Randall Krueger
//Fundamentals of Computing Lab Final Project
//Sudoku Puzzle Solver
//Program that allows the user to input any sudoku puzzle and returns
//the solution or the lack thereof to the user
//12/15/16

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include "gfx.h"
#include <fstream>
#include <cctype>

using namespace std;

//Initializing Functions
int isValid(int, int, int, int board[9][9]);
int solveCell(int, int, int board[9][9], int sboard[9][9], int);
int solvePuzzle(int, int, int board[9][9], int sboard[9][9], int);
void dispgfx(int board[9][9], int staticBoard[9][9]);
void message(string, int, int);

int main(int argc, char* argv[]) {

	//Initializing variables including the two arrays for the two boards
	int board[9][9];
	int staticBoard[9][9];
	int anim = 0, notOpen = 1, count = 0;
	string tempstring;
	char tempchar, choice = '0', x, y;

	//If more than one file is passed into the program, it ends.
	if (argc > 2) {
		cout << "Too many arguments." << endl;
		return 0;
	}

	//Fills the board and the static board with 0's
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			board[i][j] = 0;
			staticBoard[i][j] = 0;
		}
	}

	//Opens the gfx window
	gfx_open( 280, 280, "Sudoku Puzzle Solver");

	//Displays opening message upon opening the GFX window
	while(choice != 13){ //13 is the value that the gfx window receives when the return key is pressed
		message("Welcome to the sudoku puzzle solver.", 35, 20);
		message("Please enter the number that you would like to", 2, 40);
		message( "place on the puzzle into the graphics window,", 4, 60);
		message("followed by the X and the Y position in the", 10, 80); 
		message("puzzle from the top left corner.", 47, 100);
		message("Once all numbers have been placed, enter ""s"" to", 6, 130); 
		message("show the solution to the puzzle,", 45, 150);
		message("or ""a"" to see the animation of the puzzle", 22, 170); 
		message("being solved.", 105, 190);
		message("Press return to begin.", 75, 230);

		//If the user presses a key, the choice variable is changed to the key's value
		if (gfx_event_waiting()) {
			choice = gfx_wait();
		}
	}

	//If a file is passed into the program, the program will take the values for the puzzle from the file rather than from user input
	if (argc == 2) {
		ifstream ifs(argv[1]);

		//If the file does not exist, an error is returned to the GFX screen
		if (!ifs) {
			gfx_clear();
			string tempstring2(argv[1]);
			tempstring = "Error opening file " + tempstring2;
			message (tempstring,70,150);

			//Waits for the user to press q to terminate program
			message("Press ""q"" to exit.", 94, 170);
			while(gfx_wait() != 'q') {
			}
			return 0;
		}

		//Getting all characters from the input file and adding numbers to the puzzle
		while (ifs.get(tempchar)) {

			//If the character is an a or an s, changed the animation status accordingly
			if (tempchar == 'a') {
				anim = 1;
			}
			if (tempchar == 's') {
				anim = 0;
			}
			//Skips whitespace
			if (isdigit(tempchar)) {
				//Ignores zero
				if ((tempchar <= '9' && tempchar >= '1')) {
					//Making sure that the first character is the choice and the following two are x and y coordinates
					if (count % 3 == 0) {
						choice = tempchar;
					}
					if (count % 3 == 1) {
						x = tempchar;
					}
					//Every three characters input, the program will attempt to add a number to the puzzle
					if (count % 3 == 2) {
						y = tempchar;   
						//Checks if the number and positioning is valid and puts the number into the puzzle if so
						if (isValid(x - '0' -1,y - '0' - 1,choice - '0',board)) {
							board[y - '0'  - 1][x - '0' - 1] = choice - '0';
							staticBoard[y - '0' - 1][x - '0' - 1] = 1;
						}
						else{
							//If the number is not valid, an error message will be displayed, explaining the situation
							gfx_clear();
							message("File contains invalid input.", 60, 150);
							message("Press ""q"" to exit.", 94, 170);

							while(gfx_wait() != 'q') {
							}
							return 0;
						}
					}
					count++; //Increments the count every character that is taken from the file
				}
			}
		}
	}

	//If there is not a second file, the user input section is used for gathering the numbers
	if (argc == 1) {
		//Displays the board so the user can determine where their number was placed
		dispgfx(board, staticBoard);

		//All message function calls displays the string passed into it to the user
		message("Choose a number to add: ", 72, 250);

		//Waits for the user to choose a number to enter into the puzzle
		choice = gfx_wait();

		//Will continue to allow the user to enter numbers until they enter either s or a for solution or animation, respectively
		while (choice != 's' && choice != 'a') {

			//Only allows the user to enter numbers 1-9 into the puzzle
			if (choice > '0' && choice <= '9') {
				dispgfx(board,staticBoard);
				message( "Coordinates? (x,y): ", 87, 250);
				//Asks the user for the coordinates for their number
				x = gfx_wait();
				y = gfx_wait();

				//Makes sure the user's x coordinate is between 1 and 9, asking them to re-enter the value until it is.
				while (x < '1' || x > '9') {
					dispgfx(board,staticBoard);
					message("X-coordinate must be between 1 and 9.", 37, 250);
					message( "X coordinate: ", 100, 270);
					x = gfx_wait();
				}

				//Same as above but for the y-coordinate
				while (y < '1' || y > '9') {
					dispgfx(board,staticBoard);
					message("Y-coordinate must be between 1 and 9.", 37, 250);
					message( "Y coordinate: ", 100, 270);

					y = gfx_wait();
				}

				//Checks to see if the number and location is valid and adds the number to the board if it is.
				if (isValid(x - '0' -1,y - '0' - 1,choice - '0',board)) {
					board[y - '0'  - 1][x - '0' - 1] = choice - '0';
					staticBoard[y - '0' - 1][x - '0' - 1] = 1; //Static Board is used to determine which numbers were entered by the user and which were solved by the program
					dispgfx(board, staticBoard);
					message("Choose a number to add: ", 72, 250);

				}
				else{
					//If the user inputs an invalid position, the program tells the user and does not add the number
					tempstring = "Invalid position for " + to_string(choice - '0') + ".";
					dispgfx(board, staticBoard);
					message(tempstring, 70, 250);
					message("Choose a number to add: ", 72, 270);

				}


			}
			choice = gfx_wait();
		}
	}

	//If the user chose a, rather than s, the program will animate the solution (Very slow via putty)
	if (choice == 'a') {
		anim = 1;
	}

	//Loops through all of the positions on the puzzle, searching for the first open space
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			//When the first open space is found, the solve puzzle function is called
			if (board[i][j] == 0) {

				//If solveCell returns a 1, the puzzle has been completed, and the program informs the user
				if (solveCell(j,i,board, staticBoard, anim)) {
					dispgfx(board, staticBoard);
					gfx_color(100,255,100);
					message( "Puzzle solved!", 99, 250);
					gfx_color(255,255,255);
					message("Press ""q"" to exit.", 94, 270);
					while (gfx_wait() != 'q') {		  
					}
					return 1;
				}
				else {
					//If solveCell returns a 0, the puzzle is invalid, and the user is informed.
					dispgfx(board, staticBoard);
					gfx_color(255,10,100);
					message( "Invalid Puzzle!", 96, 250);
					gfx_color(255,255,255);
					message("Press ""q"" to exit.", 94, 270);
					while (gfx_wait() != 'q') {
					}
					return 0;
				}
			}
		}
	}

	//If no open places are found, the puzzle will tell the user that they entered a number in every position in the puzzle
	dispgfx(board,staticBoard);
	message( "This puzzle is already solved!", 52, 250);
	message("Press ""q"" to exit.", 94, 270);
	while (gfx_wait() != 'q') {
	}
	return 0;
}

//Function that displays a message to the user
void message(string s, int x, int y) {
	const char* c = s.c_str();
	gfx_text(x,y,c);
}

//Central function that will determine if a cell can be solved - recursively called
int solveCell(int x, int y, int board[9][9], int staticBoard[9][9], int anim) {
	int newx = x + 1;
	int newy = y;

	//If the cell is on the right side, the next cell called will be on the following row on the left side
	if (x == 8) {
		newy = y + 1;
		newx = 0;
	}

	//If the cell is the last cell on the board, and is valid, the board is finalized, and a 1 is returned, which is passed up to the initially called function, ending the solution gathering portion of the program.
	if (x == 8 && y == 8) {
		for (int i = 1; i < 10; i++) {
			if (isValid(x,y,i,board)) {
				board[y][x] = i;
				return 1;
			}
		}
		return 0;
	}

	//If the cell was entered by the user, it is skipped, and the next cell is called
	if (staticBoard[y][x] == 1) {
		if (solveCell(newx,newy,board,staticBoard, anim)) {
			return 1;
		}
		return 0;
	}

	//If the animated options was selected, the program will display
	if (anim == 1) {
		dispgfx(board, staticBoard);
		//Ideally, the usleep method should be used, but as the program is slow enough through putty, it is currently commented out.
		//usleep(20000);
	}

	//goes through all numbers 1-9, and if the number is valid in the certain cell, the function is recursively called on the next cell in line.
	//If a 1 was returned in the base case, a 1 will be recursively returned to the main function
	for (int i = 1; i < 10; i++) {
		if (isValid(x,y,i,board)) {
			board[y][x] = i;
			if (solveCell(newx,newy,board,staticBoard, anim)) {
				return 1;
			}
		}
	}
	//If there is no possible solution in the following cells, the cell is cleared and returns a 0 so that the previous cell increments to the next value
	board[y][x] = 0;
	return 0;
}

//Function that checks to see if a certain number is valid in a certain position
int isValid( int x, int y, int num, int board[9][9]) {
	for (int i = 0; i < 9; i++) {
		//If the x value in the board is not the cell that the program is trying to replace, it checks to see if the number already exists, checking each value in the row
		if (i != x) {
			if (board[y][i] == num) {
				//If an issue is found, a 0 is returned
				return 0;
			}
		}
		//Follows the same structure as above but for the column
		if (i != y) {
			if (board[i][x] == num) {
				return 0;
			}
		}
	}

	//Checks the 9x9 square the number is for the same number
	for (int j = (x / 3) * 3; j < ((x / 3) * 3) + 3; j++) {
		//integer division, x/3*3 will return if the value is in the first second or third third of the board
		for (int k = (y / 3) * 3; k < ((y / 3) * 3) + 3; k++) {
			//Skips the cell that is being checked, itself
			if (j != x || k != y) {
				if (board[k][j] == num) {
					return 0;
				}
			}
		}
	}
	//If none of the issues arose, a 1 is returned
	return 1;
}

//Function that displays the board
void dispgfx(int board[9][9],int  staticBoard[9][9]) {
	string s = " ";
	const char* c = " ";
	gfx_clear();
	//Places all squares on the board
	for (int i = 50; i < 220; i += 20) {
		for (int j = 50; j < 220; j += 20) {
			gfx_line(i,j,i,j+20);
			gfx_line(i,j,i+20,j);
			gfx_line(i+20,j,i+20,j+20);
			gfx_line(i,j+20,i+20,j+20);
		}
	}

	//Displays top text on the board
	gfx_text(80,45,"Sudoku Puzzle Solver");
	gfx_text(178,12,"By: Randy Krueger");

	//Makes third-dividing lines thicker
	gfx_line(111,50,111,230);
	gfx_line(109,50,109,230);
	gfx_line(171,50,171,230);
	gfx_line(169,50,169,230);
	gfx_line(50,111,230,111);
	gfx_line(50,109,230,109);
	gfx_line(50,171,230,171);
	gfx_line(50,169,230,169);
	gfx_line(49,49,49,231);
	gfx_line(49,49,231,49);
	gfx_line(231,49,231,231);
	gfx_line(49,231,231,231);
	gfx_line(48,48,48,232);
	gfx_line(48,48,232,48);
	gfx_line(232,48,232,232);
	gfx_line(48,232,232,232);

	//Runs through the values in the board array and displays the non-zeroes in the display
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			s = to_string(board[j][i]);
			c = s.c_str();

			//Makes user-input values red and program-determined numbers white
			if (staticBoard[j][i] == 1) {
				gfx_color(255,10,100);
			}
			if (staticBoard[j][i] == 0) {
				gfx_color(255,255,255);
			}

			//Places numbers in display
			if (board[j][i] != 0) {
				gfx_text(20*i + 58, 20*j + 65, c);
			}
		}
		gfx_color(255,255,255);
	}
}
