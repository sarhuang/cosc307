/* Name: Sarah Huang
 * Date: 3/5/22
 * Program: matrix_enum.cpp
 * Purpose: Perform enumerations on square matrices.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class Matrix{
	public:
		int W;						//Word size = # of rows/cols in matrices
		int E;						//Extra non-zero entries = # between 0 and W^2-W
		char P;                     //This is 'x' or 'h' = different printing modes
		vector <int> Perm;          //Permutation of 0 .. (W-1), for the 'X' elements. 
		vector <int> Non_X;         //This is the row/col id of each of the non-X elements. 
		vector <int> E_ID;          //This is the row/col id of the E elements 
		void Print();               //Print the matrix defined by W, Perm and E_ID 
		void Permute(int index);    //This is the recursive permuting method. 
		void Choose(int index, int npeople);     //This is the recursive n-choose-k method. 
};


void Matrix::Print(){
	vector<int>::const_iterator vit;	//Vector iterator
	int toHex;							//Matrix row as int

	if(P == 'x'){
		for(int row = 0; row < W; row++){
			for(int col = 0; col < W; col++){
				vit = find(E_ID.begin(), E_ID.end(), (row*W+col));

				//Check if there is a X in the row
				if(col == Perm[row])
					cout << "X";
				//Check if there is an E  
				else if(vit != E_ID.end())
					cout << "E";			
				else
					cout << ".";
			}
			cout << endl;
		}
		cout << endl;
	}

	else if(P == 'h'){
		for(int row = 0; row < W; row++){
			for(int col = 0; col < W; col++){
				vit = find(E_ID.begin(), E_ID.end(), (row*W+col));

				//Change current bit to 1
				if(col == Perm[row] || vit != E_ID.end())
					toHex = toHex | (1 << col);
				else
					toHex = toHex | (0 << col);
			}
			cout << hex << toHex << endl;
			toHex = 0;
		}
		cout << endl;
	}
}



void Matrix::Permute(int index){
	int temp;
	
	//Base case - we're done
	if(index == (int) Perm.size()){
		//Add what spaces are not X's
		for(int row = 0; row < W; row++){
			for(int col = 0; col < W; col++){
				if(col != Perm[row]){
					Non_X.push_back(row * W + col);
				}
			}
		}
		Choose(0, E);
		Non_X.clear();
		return;
	}

	for(unsigned int i = index; i < Perm.size(); i++){
		//Swap Perm[index] with Perm[i]
		temp = Perm[i];
		Perm[i] = Perm[index];
		Perm[index] = temp;

		Permute(index+1);

		//Swap back
		temp = Perm[i];
		Perm[i] = Perm[index];
		Perm[index] = temp;
	}
}



void Matrix::Choose(int index, int npeople){
	//n = W^2 - W
	//k = E = npeople
	
	//Base case -- if there are no more people to add
	if(npeople == 0){
		Print();
		return;
	}

	//This is a second base case -- if there are fewer people left to add
	//than there are places left on the team, then it's impossible to finish
	if(npeople > (int)Non_X.size() - index){
		return;
	}
	
	E_ID.push_back(Non_X[index]);
	Choose(index+1, npeople-1);
	E_ID.pop_back();

	Choose(index+1, npeople);
}




int main(int argc, char* argv[]){
	Matrix matt;
	stringstream ss;
		
	if(argc != 4){
		return -1;	
	}

	ss << argv[1];	//Reading in W
	ss >> matt.W;
	ss.clear();
	ss << argv[2];	//Reading in E
	ss >> matt.E;
	ss.clear();
	ss << argv[3];	//Reading in 'x' or 'h'
	ss >> matt.P;
	
	for(int i = 0; i < matt.W; i++)
		matt.Perm.push_back(i);

	matt.Permute(0);
	return 0;
}
