/* Name: Sarah Huang
 * Date: 5/3/22
 * Program: dond.cpp
 * Purpose: A contestent will be given a s-sided die numbered from 0 to s-1, and a certain number of times that they must roll that die. Call that number t. 
 *			On each roll after the first, if the contestant rolls a number that equals the previous number that they rolled, or is one away from the number that they previously rolled, they lose. 
 *			If they roll the die t times without losing, they win a certain amount of prize money.
 */


#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
using namespace std;


void rollDie(double sides, int count, int times, int prevRoll, deque <double> &prob){
	//sides = s
	//times = t
	//count = starts at 2, adds up to times
	//prevRoll = last_roll
	
	double sum;					
	vector <double> answers;	


	for(unsigned int i = 0; i < prob.size(); i++){
		sum = 0.0;

		//last_roll = 0
		if(i == 0){
			for(unsigned int j = 2; j < prob.size(); j++){
				sum += prob[j];
			}
		}
		//last_roll = s-1 (last side)
		else if(i == prob.size()-1){
			for(unsigned int k = 0; k < prob.size()-2; k++){
				sum += prob[k];
			}
		}
		//last_roll = any side not the first or last side
		else{
			for(unsigned int l = 0; l < prob.size(); l++){
				if(i != l && i-1 != l && i+1 != l){
					sum += prob[l];
				}
			}
		}

		answers.push_back(sum/sides);
	}

	
	for(unsigned int m = 0; m < answers.size(); m++){
		prob.push_back(answers[m]);
	}
	//Removes the old probabilities that were used to calculate the new probabilities
	while(prob.size() != (unsigned int) sides){
		prob.pop_front();
	}


	//Exit the program if no more probabilities need to be calculated
	if(count == times || (count == times-1 && prevRoll == -1))
		return;
	else
		rollDie(sides, count+1, times, prevRoll, prob);
}






int main(int argc, char** argv){
	int s;				//s-sided die numbered from 0 to s-1
	int t;				//# of times the die is rolled
	int last_roll;		//Last rolled value
	vector <int> input;	//The values of argv
	int temp_int;		
	string temp_str;
	stringstream ss;
	double result = 0.0;	//The final answer


	//Check if there are enough arguments
	if(argc != 4){
		return -1;
	}

	//Read in command line arguments
	for(int i = 1; i < argc; i++){		
		temp_str = argv[i];
		ss << temp_str;
		ss >> temp_int;
		input.push_back(temp_int);
		ss.clear();
	}
	s = input[0];
	t = input[1];
	last_roll = input[2];

	deque <double> p(s, 0); //p[i] is the probability of winning if last_roll = i


	//If the die is rolled once and no last_roll, all the sides are winners 
	if(t == 1 && last_roll == -1)
		cout << "1" << endl;
	else{
		//Calculate probability of die being rolled once and every side being chosen
		for(unsigned int i = 0; i < p.size(); i++){
			if(i == 0 || i == p.size()-1)
				p[i] = (1/(double) s * (s-2));
			else
				p[i] = (1/(double) s * (s-3));
		}

		//The probability is already calculated unless the parameters are beyond t=2 and last_roll=-1
		if(t == 1 || (t == 2 && last_roll == -1)){}
		else
			rollDie(s, 2, t, last_roll, p);
		
			
		//Print the probability if last_roll is -1, or if it's 0 to s-1
		if(last_roll == -1){
			for(unsigned int j = 0; j < p.size(); j++)
				result += p[j];

			result /= (double) s;
			cout << result << endl;
		}
		else
			cout << p[last_roll] << endl;
	}
		
	return 0;
}
