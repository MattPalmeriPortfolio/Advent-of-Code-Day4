//algorithm: program will require three arrays: one of indeterminate size (a vector) to read in the "random" numbers; one array 5x5 of int, to hold the numbers; marked numbers will be remembered by setting the value of the index to -1
//the following counters are required per iteration: currentBoard, drawsToWin, lastNumber, totalUnmarked;
//the following counters are required to mark the solution: bestBoard (determined by drawsToWin), bestDraws (changed if drawsToWin < bestDraws), 
//bestLastNumber (last number of best board), bestTotalUnmarked(the sum of all unmarked numbers for the best board)
//The following should happen before looping: The first set of "random" numbers is read in; bestDraws is set to 100 (an easy to beat sentinel value)
//The following should happen each major iteration: 1. A board is read in from the file 2. currentBoard is iterated 3. totalUnmarked and drawsToWin are cleared
//The following should happen each minor iteration: 1. the next "random" number is read in and drawsToWin is iterated 2. the board is searched for that number; if found, the spot is marked with -1 3. each column and row are checked for completion
//The following should happen once a board is completed: if its drawsToWin < bestDraws - take the last random number and store it in bestLastNumber; find and add together all the unmarked numbers and store in bestTotalUnmarked; set bestBoard to currentBoard
//If its drawsToWin >= bestDraws, move to the next loop.
//4.2:
//same as above, just with conditional changed to find the worst board (if bestDraws < drawsToWin)
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
void bingoReader(int bingoBoard[][5], std::ifstream& inStream);
//Reads in a 5x5 set of numbers into bingoBoard
void bingoSearch(int bingoBoard[][5], int draw);
//Searches bingo board for draw; if found, changes the value where its found to -1
bool bingoWin(const int bingoBoard[][5]);
//Searches each row and column for a win state; returns true if won, false otherwise

//main
int main()
{
	using namespace std;
	//initialize variables
	vector<int> randomList(100);
	int bingoBoard[5][5];
	int drawsToWin, lastNumber, totalUnmarked;
	int bestDraws = 100, bestLastNumber, bestTotalUnmarked;
	int worstDraws = 1, worstLastNumber, worstTotalUnmarked;
	ifstream inStream("input4.txt");
	if (inStream.fail())
		exit(1);
	//read in and clean string of random numbers
	char next = 'a';
	string dirtyInput;
	int vectorIndex = 0;
	while (!(isspace(next)))
	{
		dirtyInput = "";
		inStream.get(next);
		while (next != ',' && next != '\n')
		{
			dirtyInput.push_back(next);
			inStream.get(next);
		}
		randomList[vectorIndex] = stoi(dirtyInput);
		vectorIndex++;
	}
	//find the best board
	while (inStream.eof() == false)
	{
		totalUnmarked = 0;
		bingoReader(bingoBoard, inStream);
		for (int i = 0; i < 5; i++) //impossible to win until 5th draw, so no need to check bingoWin
		{
			lastNumber = randomList[i];
			bingoSearch(bingoBoard, lastNumber);
		}
		drawsToWin = 5;
		while (bingoWin(bingoBoard) == false)
		{
			lastNumber = randomList[drawsToWin];
			bingoSearch(bingoBoard, lastNumber);
			drawsToWin++;
		}
		if (drawsToWin < bestDraws)
		{
			bestDraws = drawsToWin;
			bestLastNumber = lastNumber;
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (bingoBoard[i][j] > 0)
						totalUnmarked += bingoBoard[i][j];
			bestTotalUnmarked = totalUnmarked;
		}
		else if (worstDraws < drawsToWin)
		{
			worstDraws = drawsToWin;
			worstLastNumber = lastNumber;
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (bingoBoard[i][j] > 0)
						totalUnmarked += bingoBoard[i][j];
			worstTotalUnmarked = totalUnmarked;
		}
	}
	inStream.close();
	cout << "Best score: " << bestLastNumber * bestTotalUnmarked
		<< "\nWorst score: " << worstLastNumber * worstTotalUnmarked << endl;
	return 0;
}
//define functions
void bingoReader(int bingoBoard[][5], std::ifstream& inStream)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			inStream >> bingoBoard[i][j];
	return;
}
void bingoSearch(int bingoBoard[][5], int draw)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (bingoBoard[i][j] == draw)
			{
				bingoBoard[i][j] = -1;
				return;
			}
	return;
}
bool bingoWin(const int bingoBoard[][5])
{
	//rows
	for (int i = 0; i < 5; i++)
		if (bingoBoard[i][0] < 0 && bingoBoard[i][1] < 0 && bingoBoard[i][2] < 0 && bingoBoard[i][3] < 0 && bingoBoard[i][4] < 0)
			return true;
	//columns
	for (int i = 0; i < 5; i++)
		if (bingoBoard[0][i] < 0 && bingoBoard[1][i] < 0 && bingoBoard[2][i] < 0 && bingoBoard[3][i] < 0 && bingoBoard[4][i] < 0)
			return true;
	return false;
}

