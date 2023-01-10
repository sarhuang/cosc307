/* Name: Sarah Huang
 * Date: 5/11/22
 * Program: BearCries.cpp
 * Purpose: Return the number of says in which you can partition message into crying emoticon subsequences.
 */

#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class BearCries {
	public:
		int count(string message);
};


int BearCries::count(string message){
	const int mod = 1000000007;	//Return answer modulo 1,000,000,007
	unsigned int i, j, k;		
	int val;						
	long long temp;
	
	vector <long long> outer;
	vector <vector <long long> > inner;
	vector <vector <vector <long long> > > mem;

	//Initialize elements of the vector of vector of vectors with 0
	outer.resize(202, 0);	//Max size of messsage is 200 characters
	for(i = 0; i < 202; i++){
		for(j = 0; j < 202; j++){
			inner.push_back(outer);
		}
		mem.push_back(inner);
		inner.clear();
	}

	//Base case - If you have no started emoticons
	mem[0][0][0] = 1;


	//Calculate # of ways you can partition message into crying emoticon subsequences
	//Adds on to the number of ways until final answer is calculated in mem[message.length()][0][0]
	for(i = 0; i < message.length(); i++){
		for(j = 0; j <= i; j++){
			for(k = 0; k <= i; k++){
				val = mem[i][j][k];	

				//There is an underscore '_' in the message
				if(val != 0 && message[i] == '_'){
					temp = (long long) j * val % mod;
					
					//Assign underscores to started emoticon with no underscores
					mem[i+1][j][k] += temp;
					if(mem[i+1][j][k] >= mod)
						mem[i+1][j][k] -= mod;

					//Started emoticon has underscores
					if(k > 0) {
						temp = (long long) k * val % mod;
						mem[i+1][j+1][k-1] += temp;
						if(mem[i+1][j+1][k-1] >= mod)
							mem[i+1][j+1][k-1] -= mod;
					}
				}
				//There is a semi-colon ';' in the message
				else if(val != 0 && message[i] == ';'){
					//Emoticons without underscore
					mem[i+1][j][k+1] += val;
					if(mem[i+1][j][k+1] >= mod)
						mem[i+1][j][k+1] -= mod;

					//Emoticons with underscores
					if(j > 0){
						temp = (long long) j * val % mod;
						mem[i+1][j-1][k] += temp;
						if(mem[i+1][j-1][k] >= mod)
							mem[i+1][j-1][k] -= mod;
					}
				}
			}
		}
	}
	return mem[message.length()][0][0];
}
