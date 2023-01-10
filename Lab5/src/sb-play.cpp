/* Name: Sarah Huang
 * Date: 3/
 * Program: 
 * Purpose: 
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
		Superball(int argc, char **argv);
		int r;		//row
		int c;		//column
		int mss;	//number of continous cells that have to be together in order to score	
		int empty;	//number of empty cells on board
		vector <int> board;		//The element in [i,j] is in entry board[i*c+j]
		vector <int> goals;		//goals[i] = 1 --> the cell is goal cell
		vector <int> colors;	//colors['p'] = 2 --> Purple scores 2 points
};

void usage(const char *s) {
	fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
	if (s != NULL) fprintf(stderr, "%s\n", s);
	exit(1);
}

Superball::Superball(int argc, char **argv){
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
		colors[argv[4][i]] = 2+i;
		colors[toupper(argv[4][i])] = 2+i;
	}

	board.resize(r*c);
	goals.resize(r*c, 0);

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
			board[i*c+j] = s[j];
			if (board[i*c+j] == '.') empty++;
			if (board[i*c+j] == '*') empty++;
			if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
				goals[i*c+j] = 1;
				board[i*c+j] = tolower(board[i*c+j]);
			}
		}
	}
}




//Create disjoint sets out of given Superball board
void createSets(Superball* &s, Disjoint_Set &d){
	const vector < list <int> > *vl;	//Elements in disjoint set
	list <int>::const_iterator lit;		
	int index;	//Disjoint set id

	//Loop through all squares on board
	for (int i = 0; i < s->r*s->c; i++){
		//Looking at one square, combining the one on its right into a disjoint set if they are the same color
		if (s->board[i] != '.' && s->board[i] != '*' && (i%s->c != s->c-1 || i == 0)) {
			if(s->board[i] == s->board[i+1] || abs(s->board[i] - s->board[i+1]) == 32){
				d.Union(i+1, d.Find(i));
			}
		}
	}

	vl = d.Get_Elements();

	//Loop through all squares EXCEPT last row
	for (int i = 0; i < (s->r-1)*s->c; i++){
		if (s->board[i] != '.' && s->board[i] != '*') {
			//Looking at one square, combine the one underneath into disjoint set if same color
			if(s->board[i] == s->board[i+s->c] || abs(s->board[i] - s->board[i+s->c]) == 32){
				index = d.Find(i);
				lit = find(vl->at(index).begin(), vl->at(index).end(), i+s->c);
				if(lit == vl->at(index).end())
					d.Union(d.Find(i+s->c), d.Find(i));
			}
		}
	}
}


//Print out scoring sets
void analyzeSuperball(Superball* &s, Disjoint_Set &d, map <int, vector <int> > &ss){
	const vector <int> *vi;			//Disjoint set size
	const vector <int> *sid;		//Disjoint set ids
	const vector < list <int> > *vl;//Elements in disjoint set
	list <int>::const_iterator lit;
	int index;	//Disjoint set id
	vector <int> temp;

	vi = d.Get_Sizes();
	sid = d.Get_Set_Ids();
	vl = d.Get_Elements();


	//Print each scoring set >= min score size
	for (unsigned int j = 0; j < sid->size(); j++) {
		if(vi->at(sid->at(j)) >= s->mss){
			index = sid->at(j);

			lit = vl->at(index).begin();
			while(lit != vl->at(index).end()){
				if(s->goals[*lit] && s->board[*lit] != '*'){

					temp.push_back(s->board[*lit]);	//character
					temp.push_back(*lit/s->c);		//row
					temp.push_back(*lit%s->c);		//col
					ss[vi->at(sid->at(j))] = temp;

					temp.clear();
					break;
				}
				lit++;
			}
		}
	}
}





int main(int argc, char **argv){
	Superball *s;					
	Disjoint_Set d;	
	map <int, vector <int> > scoringSets;
	map <int, vector <int> >::reverse_iterator rit;
	map <int, vector <int> > ranking;
	map <int, vector <int> >::reverse_iterator rait;

	const vector <int> *sid;
	list <int>::const_iterator lit;

	int r1 = -1, c1 = -1, r2 = -1, c2 = -1;


	s = new Superball(argc, argv);
	d.Initialize(s->r*s->c);

	//Analyze board and create disjoint sets
	createSets(s, d);
	analyzeSuperball(s, d, scoringSets);


	sid = d.Get_Set_Ids();

	//Get score
	int score = 0, bestScore = 0;
	if(!scoringSets.empty()){
		for(rit = scoringSets.rbegin(); rit != scoringSets.rend(); rit++){
			if(rit->second[0] == 'g') score = 6;
			if(rit->second[0] == 'r') score = 5;
			if(rit->second[0] == 'y') score = 4;
			if(rit->second[0] == 'b') score = 3;
			if(rit->second[0] == 'p') score = 2;
			score = score * rit->first;

			if(score > bestScore){
				bestScore = score;
				r1 = rit->second[1];
				c1 = rit->second[2];
			}
		}
		printf("SCORE %d %d\n", r1, c1);
	}
	else{
		//Swap randomly to end game
		if(s->empty < 5){
			printf("SWAP %d %d %d %d\n", 0, 0, 0, 1);
		}
		else{
			int first, second;
			char tempColor;
			
			
			for(unsigned int set = 0; set < sid->size(); set++){
				first = sid->at(set);
				if(s->board[first] != '*' && s->board[first] != '.'){
					r1 = first/s->c;
					c1 = first%s->c;

					for(unsigned int sec = 0; sec < sid->size(); sec++){
						second = sid->at(sec);
						
						if(s->board[second] != '*' && s->board[second] != '.'){
						//The squares are not the same one
						if(first != second){
							tempColor = s->board[second];
							s->board[second] = s->board[first];
							s->board[first] = tempColor;

							Disjoint_Set k;
						    k.Initialize(s->r*s->c);
							createSets(s, k);
							analyzeSuperball(s, k, scoringSets);

							if(!scoringSets.empty()){
								bestScore = 0;
								vector <int> tempo;

								for(rit = scoringSets.rbegin(); rit != scoringSets.rend(); rit++){
									if(rit->second[0] == 'g') score = 6;
									if(rit->second[0] == 'r') score = 5;
									if(rit->second[0] == 'y') score = 4;
									if(rit->second[0] == 'b') score = 3;
									if(rit->second[0] == 'p') score = 2;
									score = score * rit->first;

									if(score > bestScore){
										bestScore = score;
										tempo.push_back(rit->second[1]);
										tempo.push_back(rit->second[2]);
										tempo.push_back(r1);
										tempo.push_back(c1);
									}
							//		r2 = rit->second[1];
							//		c2 = rit->second[2];
								}
								ranking[bestScore] = tempo;
							}
							tempColor = s->board[second];
							s->board[second] = s->board[first];
							s->board[first] = tempColor;
						}
						}
					}
				}
			}
			//Outside checking all the combos - time to decide
			if(!ranking.empty()){
				rit = ranking.rbegin();
				r1 = rit->second[2];
				c1 = rit->second[3];
				r2 = rit->second[0];
				c2 = rit->second[1];
			}
			else{
				for(int i = 0; i < sid->size(); i++){
					if(s->board[i] != '*' && s->board[i] != '.'){
						r2 = sid->at(i) / s->c;
						c2 = sid->at(i) % s->c;
						break;
					}
				}
			}

			printf("SWAP %d %d %d %d\n", r1, c1, r2, c2);
		}
	}
	return 0;
}
