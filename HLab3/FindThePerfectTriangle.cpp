/* Name: Sarah Huang
 * Date: 3/28/22
 * Program: FindThePerfectTriangle.cpp
 * Purpose: Return the coordinates of a triangle that has integer coordinates, 
 *			and whose perimeter and area are the given perimeter and area.
 */

#include <string>
#include <cmath>
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

#define VIT(i, v) for (i = 0; i < v.size(); i++) 



class FindThePerfectTriangle {
	public:
		vector <int> constructTriangle(int area, int perimeter);
};

vector <int> FindThePerfectTriangle::constructTriangle(int area, int perimeter){	
	vector <int> rv;							//Triangle coordiantes (x1, y1, x2, y2, x3, y3)
	vector <pair <int, int> > perm[500];		//Coordinate pairs
	vector <pair <int, int> >::iterator pit;
	vector <pair <int, int> >::iterator sit;
	int x, y;
	int p;
	int y2, y3;
	

	//Generate possible pairs
	for(int i = 0; i < 500; i++){
		for(int j = 0; j <= i; j++){
			x = pow(i, 2) + pow(j, 2);		
			y = sqrt(x);

			if(x == pow(y, 2))
				x = y;
			else
				x = -1;


			if(x != -1){
				perm[i].push_back(make_pair(j, x));
				perm[j].push_back(make_pair(i, x));
			}
		}
	}

	//Go through all possible coordinates
	for(int x2 = 1; x2 < 500; x2++){
		for(pit = perm[x2].begin(); pit != perm[x2].end(); pit++){		
			
			if (perimeter >= 2 * pit->second){	
				for (int x3 = 1; x3 < 500; x3++){ 
					for(sit = perm[x3].begin(); sit != perm[x3].end(); sit++){
						p = pit->second + sit->second;
						y2 = pit->first;
						y3 = sit->first;

						//Square of b side
						x = pow((x2-x3),2) + pow((y2-y3), 2);
						y = sqrt(x);
						if(pow(y, 2) == x)
							x = y;
						else
							x = -1;

						//The coordinates (0, 0, x2, y2, x3, y3) give the correct perimeter and area
						if (x != -1 && x+p == perimeter && (x2*y3 - x3*y2) == area*2){
							rv.push_back(0);
							rv.push_back(0);
							rv.push_back(x2);
							rv.push_back(y2);
							rv.push_back(x3);
							rv.push_back(y3);
							return rv;
						}
					}
				}
			}
		}
	}
	//Not possible - empty vector
	return rv;
}
