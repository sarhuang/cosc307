#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class BlackWhiteRectangles {
	public:
		int blackCount(vector <string> rectangles);
};


int BlackWhiteRectangles::blackCount(vector <string> R){
	vector <string>::const_iterator vit;	//Vector iterator
	vector <int> param(5);					//Vector to store 5 parameters: x1, x2, y1, y2, and type
	vector <int> xs;						//Vector of all Xs
	vector <int> ys;						//Vector of all Ys
	vector <vector <int> > rectangle;		//Vector of all given rectangles
	int x1, x2, y1, y2;						//The specific x1, x2, y1, y2 for a rectangle
	int numSquares = 0;						//Total number of black squares
	int color[2][2];						//2x2 grid that represents how each rectangle is colored
	int xSet, ySet;							//Cell with column i and row j in the pattern is set
	int blackX, blackY;						//Area with black cells


	//Read in strings x1, y1, x2, y2, and type and convert to integers
	//Store all Xs, Ys, and given rectangles in separate vectors
	for(vit = R.begin(); vit != R.end(); vit++){
		sscanf(vit->c_str(), "%d %d %d %d %d", &param[0], &param[1], &param[2], &param[3], &param[4]);
		xs.push_back(param[0]);
		xs.push_back(param[2]);
		ys.push_back(param[1]);
		ys.push_back(param[3]);
		rectangle.push_back(param);
	}
	sort(xs.begin(), xs.end());
	sort(ys.begin(), ys.end());
	

	//Go through each given rectangle
	for(int x = 0; x < xs.size() - 1; x++){
		for(int y = 0; y < ys.size() - 1; y++){
			x1 = xs[x];
			x2 = xs[x+1]; 
			y1 = ys[y]; 
			y2 = ys[y+1];
			
			//Determine how many black squares are set
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					color[i][j] = 0;
					
					for(int index = 0; index < rectangle.size(); index++){
						//If the cell corresponding to column i and row j in the pattern is set
						if(rectangle[index][0] <= x1 && rectangle[index][2] >= x2 && rectangle[index][1] <= y1 && rectangle[index][3] >= y2){
							xSet = (x1 - rectangle[index][0] + i) % 2;
							ySet = (y1 - rectangle[index][1] + j) % 2;

							
							/* Type 1 = Blackout
							 * Type 2 = Every other row is black
							 * Type 3 = Every other col is black
							 * Type 4 = Checkerboard	*/
							if (rectangle[index][4] == 1 || (rectangle[index][4] == 2 && ySet == 1)
							  || (rectangle[index][4] == 3 && xSet == 1) || (rectangle[index][4] == 4 && (xSet ^ ySet) != 1))	
								color[i][j] = 1;
						}
					}
					//Count number of black cells in rectangle and add to total
					//White cells (color[i][j] = 0) do not count	
					blackX = (x2-x1)/2;
					blackY = (y2-y1)/2;

					if(i != 1 && (x2-x1)%2 == 1) blackX++;
					if(j != 1 && (y2-y1)%2 == 1) blackY++;

					if(color[i][j] == 1)
						numSquares = numSquares + (blackX * blackY);
				}
			}
		}
	}
	return numSquares;
}
