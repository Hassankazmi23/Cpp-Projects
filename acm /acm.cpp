#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;


/*
 Daisy Ramos
 CSCI 381 Summer 2014
 Project7
 */




bool isValid(int [], int);

void rotate(int a[], int size);

vector<string> findSeats(int seating[], int n, int &);

bool isDuplicate(string temp, vector<string> v);

string convertString(int b[], int n);


vector<string> findSeats(int seating[], int n, int &validseating)
{
	validseating = 0;
    
	//Create a temporary array to hold the rotated seating, so next permutation is valid
	int *b = new int[n];
    
	//List of valid seating, without duplicates
	vector<string> v;
    
	do
	{
        
		if(isValid(seating, n))
		{
			//copy seating so that it can be rotated to check for duplicates
			for(int i = 0; i < n; i++)
			{
				b[i] = seating[i];
			}
            
			rotate(b,n);
            
			string temp = convertString(b,n);
            
			//check if the rotated seating is a duplicate
			if( !isDuplicate(temp, v) )
			{
				validseating++;
				v.push_back(temp);
			}
            
		}
        
	}
	while( next_permutation( seating,seating+n ) );
    
	delete [] b;
    
	return v;
}//findSeats

//Converts integer array to a string to check duplicates
string convertString(int b[], int n)
{
	string result = "";
    
	ostringstream convert;
    
	for(int i = 0; i < n; i++)
	{
		convert << b[i] << " ";
	}//for
    
	result = convert.str();
    
	return result;
}//convertString

//Returns true if the string already exists in the vector v
bool isDuplicate(string x, vector<string> v)
{
	if(v.empty())
		return false;
    
	if(std::find(v.begin(), v.end(), x) != v.end())
		return true;
    
	return false;
}//isDuplicate

//Returns true if the seating array passes the requirements for valid seating
bool isValid(int seating[], int size)
{
	//Gender check
	for(int i = 0; i < size; i++)
	{
		//Two males sitting next to each other
		if(seating[i] < 0 && seating[((i+1) % size)] < 0 )
			return false;
        
		//Two females sitting next to each other
		if( seating[i] > 0 && seating[((i+1) % size)] > 0 )
			return false;
	}
    
    
	//Check for team mates
	for(int i = 0; i < size; i++)
	{
		//Team mates are sitting next to each other
		if( abs(seating[i]) == abs(seating[((i+1) % size)]) )
			return false;
	}
    
	return true;
    
}//isValid

//Rotates the array until the value 1 is located at a[0]
void rotate(int a[], int size)
{
	int indexLow = -1;
    
	while(indexLow == size || a[++indexLow] != 1);
    
	int *b = new int[size];
    
	for(int i = 0; i < size; i++)
	{
		b[i] = a[indexLow++ % size];
	}
    
	for(int i = 0; i < size; i++)
	{
		a[i] = b[i];
	}
    
	delete []b;
}//rotate



int main()
{
	ofstream outFile("acm-out.txt");
    
	int validseating = 0;
    
	for(int i = 2; i <= 9; i++)
	{
        
		int n = i;
        
		n *= 2;
        
		int *seating = new int[n];
        
		//Fill the seating array.
		int t = (n/2) * -1;
        
		for(int j = 0; j < n/2; j++)
		{
			seating[j] = t++;
		}
        
		t++;
        
		for(int j = n/2; j < n; j++)
		{
			seating[j] = t++;
		}
        
		vector<string> solutions = findSeats(seating, n, validseating);
        
		sort(solutions.begin(), solutions.end());
        
		outFile << "Case " << n/2<< ": " << validseating << endl;
        
        //Print solutions to outFile.
		for (vector<string>::iterator it = solutions.begin(); it != solutions.end(); ++it)
		{
			outFile << *it << endl;
		}
        
		delete []seating;
	}
    
	outFile.close();
    
	return 0;
}



