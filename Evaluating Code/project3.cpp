/*CSCI 381 Summer 2014
  Project 3
  @Daisy Ramos
  
*/
/*The candidate you are currently interviewing shows you the following c++ 
expression: 
 
((n&(n-1)) == 0) 
 
As you scratch your head, she says “this returns true if n is either zero or a power 
of 2.” 
Is she correct in her claim? 
 
Prove or disprove.
*/

/*
	Proof by contradiction:
    This code uses the & operator where it takes in n as a number
    then it considers (n-1). It is checking whether n is a power of 2 or equal to 0
    We can consider the binary form to have a more visual 
    representation of the code. 
	Let's assume the statement is false:
    Consider n=2 in binary we get 0010 & 0001 which is 0000. 
    Now let's consider n=3 we get 0011 & 0010 which is 0010. (false)
    Now let's consider n=4 we get 0100 & 0011 which is 0000.
    It is clear that if fails for n not equal to a power of 2.
	Conclusion: The claim of the candidates' is true that it returns true if n is either zero or a power of 2
	and works for cases if and only if n >=0.
	*/

#include<iostream>
using namespace std;

int main(){

	int n;
	cout<<"enter a number and we'll tell you whether it's a power of 2 or not:";
	cin>>n;

	((n&n-1)==0)? cout<<n<<" is power of 2\n" : cout<<n<<" is not power of 2\n";


	return 0;

	
}
