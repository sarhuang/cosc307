/* Name: Sarah Huang
 * Date: 3/20/22
 * Program: sb-play.cpp
 * Purpose: However, now its job is to print a single move as would be accepted as input for the sb-player program. 
 *			In other words, it needs to output a SWAP or SCORE line with legal values. 
 */

#include "disjoint_set.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
using plank::Disjoint_Set;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))



class Superball {
	public:
		Superball(int argc, char** argv);
		int r;		//row
		int c;		//column
		int mss;	//number of continous cells that have to be together in order to score	
		int empty;	//number of empty cells on board
		vector <int> board;		//The element in [i,j] is in entry board[i*c+j]
		vector <int> goals;		//goals[i] = 1 --> the cell is goal cell
		vector <int> colors;	//colors['p'] = 2 --> Purple scores 2 points
};

void usage(const char* s) {
	fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
	if (s != NULL) fprintf(stderr, "%s\n", s);
	exit(1);
}

Superball::Superball(int argc, char** argv) {
	int i, j;
	string s;

	if (argc != 5) usage(NULL);

	if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
	if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
	if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

	colors.resize(256, 0);

	for (i = 0; i < (int)strlen(argv[4]); i++) {
		if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
		if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
		if (colors[argv[4][i]] != 0) usage("Duplicate color");
		colors[argv[4][i]] = 2 + i;
		colors[toupper(argv[4][i])] = 2 + i;
	}

	board.resize(r * c);
	goals.resize(r * c, 0);

	empty = 0;

	for (i = 0; i < r; i++) {
		if (!(cin >> s)) {
			fprintf(stderr, "Bad board: not enough rows on standard input\n");
			exit(1);
		}
		if ((int)s.size() != c) {
			fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
			exit(1);
		}
		for (j = 0; j < c; j++) {
			if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
				fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
				exit(1);
			}
			board[i * c + j] = s[j];
			if (board[i * c + j] == '.') empty++;
			if (board[i * c + j] == '*') empty++;
			if (isupper(board[i * c + j]) || board[i * c + j] == '*') {
				goals[i * c + j] = 1;
				board[i * c + j] = tolower(board[i * c + j]);
			}
		}
	}
}




//Create disjoint sets out of given Superball board
void createSets(Superball*& s, Disjoint_Set& d) {
	const vector < list <int> >* vl;	//Elements in disjoint set
	list <int>::const_iterator lit;
	int index;	//Disjoint set id

	//Loop through all squares on board
	for (int i = 0; i < s->r * s->c; i++) {
		//Looking at one square, combining the one on its right into a disjoint set if they are the same color
		if (s->board[i] != '.' && s->board[i] != '*' && (i % s->c != s->c - 1 || i == 0)) {
			if (s->board[i] == s->board[i + 1] || abs(s->board[i] - s->board[i + 1]) == 32) {
				d.Union(i + 1, d.Find(i));
			}
		}
	}
	vl = d.Get_Elements();

	//Loop through all squares EXCEPT last row
	for (int i = 0; i < (s->r - 1) * s->c; i++) {
		if (s->board[i] != '.' && s->board[i] != '*') {
			//Looking at one square, combine the one underneath into disjoint set if same color
			if (s->board[i] == s->board[i + s->c] || abs(s->board[i] - s->board[i + s->c]) == 32) {
				index = d.Find(i);
				lit = find(vl->at(index).begin(), vl->at(index).end(), i + s->c);
				if (lit == vl->at(index).end())
					d.Union(d.Find(i + s->c), d.Find(i));
			}
		}
	}
}


//Compile scoring sets into map
void analyzeSuperball(Superball*& s, Disjoint_Set& d, map <int, vector <int> >& ss) {
	const vector <int>* vi;			//Disjoint set size
	const vector <int>* sid;		//Disjoint set ids
	const vector < list <int> >* vl;//Elements in disjoint set
	list <int>::const_iterator lit;
	int index;	//Disjoint set id
	vector <int> temp;

	vi = d.Get_Sizes();
	sid = d.Get_Set_Ids();
	vl = d.Get_Elements();

	//Store each scoring set >= min score size in map
	for (unsigned int j = 0; j < sid->size(); j++) {
		if (vi->at(sid->at(j)) >= s->mss) {
			index = sid->at(j);
			lit = vl->at(index).begin();

			while (lit != vl->at(index).end()) {
				if (s->goals[*lit] && s->board[*lit] != '*') {
					temp.push_back(s->board[*lit]);
					temp.push_back(*lit / s->c);
					temp.push_back(*lit % s->c);
					ss[vi->at(sid->at(j))] = temp;
					temp.clear();
					break;
				}
				lit++;
			}
		}
	}
}



int main(int argc, char** argv) {
	Superball* s;
	Disjoint_Set d;
	map <int, vector <int> > scoringSets;			//map <size, <char, row, col> >
	map <int, vector <int> >::reverse_iterator rit;	

	const vector <int>* vi;
	const vector <int>* sid;
	const vector < list <int> >* vl;
	list <int>::const_iterator lit;

	int r1 = -1, c1 = -1, r2 = -1, c2 = -1;		//row1, col1, row2, col2
	char color = '!';							//Color ('p' or 'P')
	int colorIndex = 69;						//Index of 1st square
	int temp;									
	int index = -1;								//Index of 2nd square
	int best = -1, bestSize = 0;				//The best element's index and size
	map <int, int> possible;					//All possible swaps of 2nd square knowing 1st square
	map <int, int>::iterator it;
	map <int, map <char, int> > bestSwaps;		//All of the best swaps after going through all possibilities
	map <int, map <char, int> >::iterator bit;

	s = new Superball(argc, argv);
	d.Initialize(s->r * s->c);


	//Analyze board and create disjoint sets
	createSets(s, d);
	analyzeSuperball(s, d, scoringSets);
	vi = d.Get_Sizes();
	sid = d.Get_Set_Ids();
	vl = d.Get_Elements();


	//Claim score
	if (!scoringSets.empty()) {
		rit = scoringSets.rbegin();
		printf("SCORE %d %d\n", rit->second[1], rit->second[2]);
	}
	else {
		//Swap randomly to end game
		if (s->empty < 5) {
			printf("SWAP %d %d %d %d\n", 0, 0, 0, 1);
		}
		//Calculate what two squares to swap
		else {
			for (unsigned int set = 0; set < sid->size(); set++) {
				for (unsigned int j = 0; j < sid->size(); j++) {
					//Grab first square that is NOT connected to something
					if (color == '!' && vi->at(sid->at(set)) == 1 && s->board[sid->at(set)] != '.' && s->board[sid->at(set)] != '*') {
						index = sid->at(set);
						color = s->board[index];
						colorIndex = index;
						r1 = index / s->c;
						c1 = index % s->c;
					}
					//Swap with second square and hopefully make a disjoint set bigger
					else if (color != '!' && j != set && vi->at(sid->at(j)) >= 1 && vi->at(sid->at(j)) < s->mss && s->board[sid->at(j)] != '.' && s->board[sid->at(j)] != '*') {
						index = sid->at(j);
						//Not the same color as the first square
						if (s->board[index] != color && abs(s->board[index] - s->board[colorIndex]) != 32) {
							lit = vl->at(index).begin();
							
							//Go through elements in disjoint set - disregards goals
							while (lit != vl->at(index).end()) {
								temp = *lit;

								//To the right
								if (s->board[temp + 1] == color || abs(color - s->board[temp + 1]) == 32) 
									possible.insert(pair <int, int>(temp, 1));
								//To the left
								if (temp > 0 && (s->board[temp - 1] == color || abs(color - s->board[temp - 1]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
									else
										possible.insert(pair <int, int>(temp, 1));
								}
								//Below it
								if (temp < (s->r - 1) * s->c && (s->board[temp + s->c] == color || abs(color - s->board[temp + s->c]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
									else
										possible.insert(pair <int, int>(temp, 1));
								}
								//Above it
								if (temp - s->c != colorIndex && temp >= s->c && (s->board[temp - s->c] == color || abs(color - s->board[temp - s->c]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
									else
										possible.insert(pair <int, int>(temp, 1));
								}
								//Do diagonal above right
								if (temp - s->c + 1 != colorIndex && temp >= s->c && (s->board[temp - s->c + 1] == color || abs(color - s->board[temp - s->c + 1]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
								}
								//Diagonal above left
								if (temp - s->c - 1 != colorIndex && temp >= s->c && (s->board[temp - s->c - 1] == color || abs(color - s->board[temp - s->c - 1]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
								}
								//Diagonal below right
								if (temp < (s->r - 1) * s->c && (s->board[temp + s->c + 1] == color || abs(color - s->board[temp + s->c + 1]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
								}
								//Diagonal below left
								if (temp < (s->r - 1) * s->c && (s->board[temp + s->c - 1] == color || abs(color - s->board[temp + s->c - 1]) == 32)) {
									it = possible.find(temp);
									if (it != possible.end())
										possible[temp] += 1;
								}
								//Backup
								r2 = temp / s->c;
								c2 = temp % s->c;
								lit++;
							}
						}
					}
				}
				for (it = possible.begin(); it != possible.end(); it++) {
					if (it->second > bestSize) {
						best = it->first;
						bestSize = it->second;
					}
				}
				if (best != -1) {
					r2 = best / s->c;
					c2 = best % s->c;
				}
				bestSwaps[best].insert(pair <char, int>(s->board[best], bestSize));
			}

			//Choosing which swap combo is the best based on the most points
			int bestFinal = -1, bestFinalSize = 0;
			for (bit = bestSwaps.begin(); bit != bestSwaps.end(); bit++) {
				if (s->board[bit->first] == 'g') bit->second[s->board[bit->first]] += 2;
				if (s->board[bit->first] == 'r') bit->second[s->board[bit->first]] += 1;
				if (s->board[bit->first] == 'y') bit->second[s->board[bit->first]] += 1;
		
				if (bit->second[s->board[bit->first]] > bestFinalSize) {
					bestFinal = bit->first;
					bestFinalSize = bit->second[s->board[bit->first]];
				}
			}
			if (bestFinal != -1) {
				r2 = bestFinal / s->c;
				c2 = bestFinal % s->c;
			}

			//Final print
			printf("SWAP %d %d %d %d\n", r1, c1, r2, c2);
		}
	}
	return 0;
}


