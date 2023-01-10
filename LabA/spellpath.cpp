/* Name: Sarah Huang
 * Date: 5/11/22
 * Program: spellpath.cpp
 * Purpose: Your job is to find the longest path through the cells such that adjacent nodes in the path have values that differ by exactly one.
 */

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
using namespace std;

//Return the maximum path length of the longest path that starts with cell (row, col)
int maxPathLen(int row, int col, vector <vector <char> > &grid){
	//n = cell at (row, col)
	//(r,c) is cell adjacent to n (value is one less or one greater than n's value)
	static int maxPath = 1;	
	int path = 0;
	char n;
	
	
	n = grid[row][col];
	grid[row][col] = '-';


	//They are in the same column and their row numbers differ by one.
	if(row != (int)grid.size()-1 && abs(grid[row+1][col] - n) == 1){
		path += maxPathLen(row+1, col, grid);
		path++;
	}

	//They are in the same row and their column numbers differ by one.
	if(col != (int)grid[0].size()-1 && abs(grid[row][col+1] - n) == 1){
		path += maxPathLen(row, col+1, grid);
		path++;
	}

	//If c is even, then (r,c) is adjacent to (r+1,c+1) and (r+1,c-1).
	if(col % 2 == 0 && row != (int)grid.size()-1 && col != (int)grid[0].size()-1){
		if(abs(grid[row+1][col+1] - n) == 1){
			path += maxPathLen(row+1, col+1, grid);
		}

		if(col != 0 && abs(grid[row+1][col-1] - n) == 1){
			path += maxPathLen(row+1, col-1, grid);
			path++;
		}
	}

	//Similarly, if c is odd, then (r,c) is adjacent to (r-1,c+1) and (r-1,c-1).
	if(col % 2 != 0 && row != 0){
		if(abs(grid[row-1][col+1] - n) == 1){
			path += maxPathLen(row-1, col+1, grid);
			path++;
		}
		
		if(abs(grid[row-1][col-1] - n) == 1){
			path += maxPathLen(row-1, col-1, grid);
			path++;
		}
	}


	//Find the max path with the chosen starting cell
	if(maxPath < path){
		maxPath = path;
		grid[row][col] = n;
	}

	return path;
}



int main(){
	vector <string> letters; //The grid values but as one string
	string input;
	int row, col;
	int answer = 0;
	int temp = 0;
	
	//Read in inputs and convert them from string to char
	while(cin >> input){
		letters.push_back(input);
	}
	row = letters.size();
	col = letters[0].size();
	vector <vector <char> > grid(row, vector <char> (col, '0'));
	

	for(unsigned int i = 0; i < letters.size(); i++){
		for(unsigned int j = 0; j < letters[i].size(); j++){
			grid[i][j] = letters[i][j];
		}
	}


	//Start at every cell and check which one has the greatest path out of all of them
	for(unsigned int r = 0; r < grid.size(); r++){
		for(unsigned int c = 0; c < grid[0].size(); c++){		
			if(grid[r][c] != '-'){
				temp = maxPathLen(r, c, grid);
				if(temp > answer)
					answer = temp;
			}
		}
	}

	if(answer == 0)
		answer++;

	cout << answer << endl;
	return 0;
}
