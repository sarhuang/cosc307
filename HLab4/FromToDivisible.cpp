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


class FromToDivisible {
	public:
		//Finding the Greatest Common Denominator of x and y
		long int gcd(long int x, long int y){
			long int a = x % y;	
			if(a == 0)
				return y;
			else
				return gcd(y, a);
		}

		int shortest(int N, int S, int T, vector <int> a, vector <int> b);
};


/*
  Graph with N nodes, numbered 1 through N
  You are given two vectors (a and b), which have exactly M elements
	- Each element of a and b is between 1 to N
  Starting node S and ending node T
  If there is no path from S to T, return -1
*/
int FromToDivisible::shortest(int N, int S, int T, vector <int> a, vector <int> b){
	int path[1000];	//The vectors a and b could have 1000 elements	
	int edge[1000];	
	int node = 0;
	int count = 0;
	int x;
	long int A; //For using GCD, it has to be long int because N can be 1,000,000,000
	

	for(unsigned int i = 0; i < a.size(); i++){
		//If S is a multiple of a[i], there is an edge from S to node i
		if(S % a[i] == 0){
			path[i] = 1;
			edge[node] = i;
			node++;
		}
		else
			path[i] = -1;
	}

	//Check the specific amount of nodes to find edges and form a path
	while(count < node){
		x = edge[count];
		
		//If T is a multiple of b[x], there is an edge from node x to T
		if(T % b[x] == 0)
			return path[x];

		for(unsigned int j = 0; j < a.size(); j++){
			//Use GCD to create the graph
			A = b[x] / gcd(b[x], a[j]) * a[j];
			
			//Connect edges and increase path
			if(path[j] == -1 && A <= N){
				path[j] = path[x] + 1;
				edge[node] = j;
				node++;
			}
		}
		count++;
	}
	//If there is no path from S to T
	return -1;
}
