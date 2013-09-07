/*
 * File: SudokuSolver.cpp
 * ----------------------
 * Name: Charles Guan
 * This program solves Sudoku puzzles and
 * scaled-size variants of Sudoku puzzles.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "grid.h"
using namespace std;

/* Constants */

const int PUZZLE_SIZE = 9; // Edge length of puzzle.
const int SUBGRID_SIZE = 3; // == sqrt(PUZZLE_SIZE)
const int UNKOWN_VALUE = 0; // Default value for unknown squares
const int NUM_SPACES = 1; // Num spaces between values in infile.

 /* Function prototypes */

void addValues(Grid<int>& puzzle);
string promptUserForFile(ifstream& infile, string str);
void printGrid(Grid<int>& puzzle);
bool solve(Grid<int>& puzzle);
bool attempt(Grid<int>& puzzle, int r, int c);
bool isValid(Grid<int>& puzzle, int r, int c);
bool isDuplRow(Grid<int>& puzzle, int r, int c);
bool isDuplCol(Grid<int>& puzzle, int r, int c);
bool isDuplSubGrid(Grid<int>& puzzle, int r, int c);
void nextPos(int& r, int& c);
void prevPos(int& r, int& c);

/* Main program */

int main() {
	cout << "This program solves Sudoku puzzles." << endl;
	Grid<int> puzzle(PUZZLE_SIZE, PUZZLE_SIZE);
	addValues(puzzle);
	cout << endl << "Starting Sudoku grid:" << endl;
	printGrid(puzzle);
	cout << endl << endl << "Solution:" << endl;
	if (!solve(puzzle)) cout << "No solution found." << endl;
	printGrid(puzzle);
	return 0;
}

 /*
  * Function: solve
  * Usage: solve(puzzle);
  * ---------------------
  * Solves a Sudoku puzzle using recursive backtracking.
  * Returns true if a solution is found.
  */
bool solve(Grid<int>& puzzle) {
	return attempt(puzzle, 0, 0);
}

/*
 * Function: attempt
 * Usage: attempt(puzzle, row, col);
 * -----------------------------
 * Attempts to solve Sudoku puzzle and does real work for solve.
 * Returns true if a solution is found. Each recursive call 
 * increments the position in row-major order.
 */
bool attempt(Grid<int>& puzzle, int r, int c) {
	if (!puzzle.inBounds(r, c)) {
		if (isValid(puzzle, PUZZLE_SIZE - 1, PUZZLE_SIZE - 1)) return true;
		return false;
	}
	bool solved = false;
	if (puzzle[r][c] == UNKOWN_VALUE) {
		for (int i = 1; i <= PUZZLE_SIZE; i++) {
			if (!solved) {
				puzzle[r][c] = i;
				if (isValid(puzzle, r, c)) {
					nextPos(r, c);
					solved |= attempt(puzzle, r, c);
					prevPos(r, c);
				}
			}
		}
		if (!solved) puzzle[r][c] = UNKOWN_VALUE;
	} else {
		nextPos(r, c);
		solved |= attempt(puzzle, r, c);
	}
	return solved;
}

/*
 * Function: isValid
 * Usage: if(isValid(puzzle, row, column)) ...
 * -------------------------------------------
 * Returns true if the value at (r, c) is a valid placement
 * for the puzzle, as determined by the usual Sudoku rules.
 */
bool isValid(Grid<int>& puzzle, int r, int c) {
 	return !(isDuplRow(puzzle, r, c)
 		|| isDuplCol(puzzle, r, c)
 		|| isDuplSubGrid(puzzle, r, c));
}

/*
 * Function: isDuplRow
 * Usage: return !(isDuplRow(puzzle, row, col) ...);
 * -------------------------------------------------
 * Returns true if there are two elements in the given
 * row with duplicate value puzzle[row][col].
 */
bool isDuplRow(Grid<int>& puzzle, int r, int c) {
	for (int i = 0; i < PUZZLE_SIZE; i++) {
		if (puzzle[r][i] == puzzle[r][c] && i != c) return true;
	}
	return false;
}

/*
 * Function: isDuplCol
 * Usage: return !(isDuplCol(puzzle, row, col) ...);
 * -------------------------------------------------
 * Returns true if there are two elements in the given
 * column with duplicate value puzzle[row][col].
 */
bool isDuplCol(Grid<int>& puzzle, int r, int c) {
	for (int i = 0; i < PUZZLE_SIZE; i++) {
		if (puzzle[i][c] == puzzle[r][c] && i != r) return true;
	}
	return false;
}

/*
 * Function: isDuplSubGrid
 * Usage: return !(isDuplSubGrid(puzzle, row, col) ...);
 * -----------------------------------------------------
 * Returns true if there are two elements in the sub-grid
 * with duplicate value puzzle[row][col]. Does not check
 * elements that are also in the same row or column.
 */
bool isDuplSubGrid(Grid<int>& puzzle, int r, int c) {
	int subGridRow = r / SUBGRID_SIZE;
	int subGridCol = c / SUBGRID_SIZE;
	for (int row = subGridRow * SUBGRID_SIZE; row < (subGridRow + 1) * SUBGRID_SIZE; row++) {
		for (int col = subGridCol * SUBGRID_SIZE; col < (subGridCol + 1) * SUBGRID_SIZE; col++) {
			if (r != row && c != col && puzzle[r][c] == puzzle[row][col]) return true;
		}
	}
	return false;
}

/*
 * Function: addValues
 * Usage: addValues(puzzle);
 * ------------------------
 * Reads in Sudoku text file and adds values to the grid.
 */
void addValues(Grid<int>& puzzle) {
	ifstream infile; string line;
	promptUserForFile(infile, "Sudoku file: ");
	for (int r = 0; r < PUZZLE_SIZE; r++) {
		getline(infile, line);
		for (int c = 0; c < PUZZLE_SIZE; c++) {
			puzzle[r][c] = line[(NUM_SPACES + 1) * c] - '0';
		}
	}
}

/*
 * Function: printGrid
 * Usage: printGrid(puzzle);
 * -------------------------
 * Prints out the values of the puzzle.
 */
 void printGrid(Grid<int>& puzzle) {
 	for (int r = 0; r < PUZZLE_SIZE; r++) {
 		for (int c = 0; c < PUZZLE_SIZE; c++) {
 			char ch = puzzle[r][c] + '0';
 			cout << ch << " ";
 		}
 		cout << endl;
 	}
 }

/*
 * Function: nextPos
 * Usage: nextPos(row, col);
 * ------------------------------
 * Increments row and col value to next position in grid.
 */
void nextPos(int& r, int& c) {
	c++;
	if (c == PUZZLE_SIZE) {
		c = 0;
		r++;
	}
}

/*
 * Function: prevPos
 * Usage: prevPos(row, col);
 * ------------------------------
 * Decrements row and col value to previous position in grid.
 */
 void prevPos(int& r, int& c) {
 	c--;
 	if (c == -1) {
 		c = PUZZLE_SIZE - 1;
 		r--;
 	}
 }