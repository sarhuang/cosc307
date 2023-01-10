/* Name: Sarah Huang
 * Date: 2/13/22
 * Program: RadioRange.cpp
 * Topcoder Description:
 *		This problem takes place in a plane. Nikola works at a radio station that is located in the point (0, 0). 
 *		You can hear the station if and only if you are within a circle with center at (0, 0) and some unknown radius.
 *
 *		There are some cities near the station. Each city has the shape of a circle with some positive integer radius. 
 *		Cities may partially or even completely overlap each other, and some of them may contain the radio station. 
 *		You are given the description of all cities as int[]s X, Y, and R. For each valid i, there is a city with center at (X[i], Y[i]) and radius R[i].
 *
 *		The radius of our radio station is bad if there is a city that is only partially covered by the radio signal. Otherwise, the radius is good. 
 *		In other words, if there is a point in a city where you can listen to our radio and another point in the same city where you cannot do so, the radius is bad. 
 *		A radius is good if and only if each city is either covered completely by radio signal, or not covered at all.
 *
 *		You are given a int Z. The radius of our radio station is a real number chosen uniformly at random from the range [0, Z]. 
 *		Return the probability that the radius is good.
 */


#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class RadioRange {
	public:
		//vector <int> X = X coordinate of city
		//vector <int> Y = Y coordinate of city
		//vector <int> R = Radius
		//int Z = Radius of radio station between range 0 to Z
		double RadiusProbability(vector <int> X, vector <int> Y, vector <int> R, int Z){
			double d = 0.0;
			double distance = 0.0;		
			double total = 0.0;		
			int count = 0;			
			double probability = 0.0;
			map <double, int> m;						
			map <double, int>::const_iterator mit;
			

			m.insert(pair<double, int> (1.0 * Z, 2));

			for(unsigned int i = 0; i < X.size(); ++i){
				//Pythagorean theorem
				d = sqrt(pow(X[i], 2) * 1.0 + pow(Y[i], 2) * 1.0) ;
				
				//Insert close value
				m.insert(pair<double, int> (max(0.0, d - R[i]), 0));
				//Insert far value
				m.insert(pair<double, int> (d + R[i], 1));
			}

			//Use keys and values to determine segments of distance from origin that are "good"
			for(mit = m.begin(); mit != m.end(); mit++){
				if(count == 0)
					distance = distance + mit->first - total;
				
				if(mit->second == 2)
					break;
				
				if(mit->second == 0)
					count++;
				else
					count--;
				
				total = mit->first;
			}
			
			probability = distance/Z;
			return probability;
		}
};
