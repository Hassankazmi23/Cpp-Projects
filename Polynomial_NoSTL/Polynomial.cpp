#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;
//declaring the output to file globally.
ostringstream toout;
class PolyNode{

private:
	//private variables.
	int coefficient, exponent;
	PolyNode *next;//next pointer.

public:
	PolyNode(){};//default constructor. 
	void setCoeff(int coeff){coefficient = coeff;}//sets coefficient
	void setExp(int exp){exponent = exp;}//sets exponent.
	void setNext(PolyNode* TheNext){next = TheNext;}//sets next pointer.
	int getCoeff(){return coefficient;}//returns coefficient
	int getExp(){return exponent;}//returns exponent.
	PolyNode* Next(){return next;}//returns pointer.

};

class PolyLList{
private:
	PolyNode* head;//declared a head node.
public:
	//by default
	PolyLList(){
		//make a null header.
		PolyNode *newNode = new PolyNode();
		newNode->setCoeff(0);
		newNode->setExp(0);
		newNode->setNext(NULL);
		head = newNode;
	};
	//destructor
	~PolyLList(){};
	//appends the coeff and exp to many created lists.
	void append(int coeff, int exp){
		//if the coefficient is 0, then declare the exponent to be 0.
		if(coeff == 0){
			exp = 0;
		}
		//creating a new node.
		PolyNode *newNode = new PolyNode();//setting coefficients and exponents to the node.
		newNode->setCoeff(coeff);
		newNode->setExp(exp);
		newNode->setNext(NULL);//set pointer next to NULL.

		//adds the coefficient if exp are the same.
		//This creates a node for polynode. It checks whether p is NULL. During the first call, it won't
		//enter the loop as p will be empty. Then, at the second apperance, it will enter the while loop.
		//It checks whether the exponents are same, and if they are they add the coefficients and returns
		//and exits the loop.
		PolyNode* p = head->Next();
		while(p != NULL){
			if(p->getExp() == exp){
				p->setCoeff(p->getCoeff() + coeff);
				return;
			}
			p = p->Next();
		}
		//After exiting the loop, it sets p to the head node.
		p = head;
		//sorting the list.
		while(p->Next() != NULL){
			//This cehcks the entered node as well as the line node and sorts it into descending order.
			if(newNode->getExp() > p->Next()->getExp()){
				newNode->setNext(p->Next());
				p->setNext(newNode);
				return;
			}
			p = p->Next();
		}
		//sets the newnode as the next node of p.
		p->setNext(newNode);
	}

	string convertInt(int number)
	{
		stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}

	string print(){
		string prints = "";//declare a empty string.
		PolyNode* p = head->Next();//declare a pointer to the next of the head. 
		while(p != NULL){//while p is not null.
			//append the string to the list. It also converts the int to string using the above class.
			if(p->getCoeff() == 0){
				//if coefficient is 0, then the exponent is also 0. 
				prints += convertInt(p->getCoeff()) + " "+ convertInt(p->getCoeff());
			}
			else{
				//otherwise output the answer to the file.
				prints += convertInt(p->getCoeff())+" "+convertInt(p->getExp())+" ";
			}
			p = p->Next();
		}
		//returns the string.
		return prints;
	}
};

void Extract(string str1, string str2){
	//declares the object of the polylist class. 
	PolyLList list1, list2, Add, Sub, Prod;
	int coeff, exp, coeffA, expA;

	//reads in the first line using stringstream.
	stringstream str(str1);
	//breaks the string in to 2 numbers. Considering the input, 0 3 where 0 is the coefficient and 3
	//is the exponent. It sets coeff as 0 and exp as 3.
	while(str >> coeff >> exp){
		//this reads in the second line. This code is only to multiply two different polynomials.
		stringstream thestr(str2);
		while(thestr >> coeffA >> expA){
			//it appends the polynomial term of line1 and the entire term of line2. It multiplies the coefficient
			//and adds the exponent.
			Prod.append(coeff*coeffA, exp+expA);
		}
		//it first appends the list. Then it add the numbers that are appended to the list. And then it subtracts them.
		list1.append(coeff, exp);
		Add.append(coeff, exp);
		Sub.append(coeff, exp);
	}
	//this reads in the second line. 
	stringstream otherstr(str2);
	while(otherstr >> coeff >>exp){
		//appends the second line coefficient and exponents. they are then added and sorted against list1.
		list2.append(coeff, exp);
		Add.append(coeff, exp);
		//the - sign basically acts as a catylist as the append function only adds. Thus it passes in the negative
		//sign for the coefficient along with it.
		Sub.append(-coeff, exp);
	}
	//prints out the stuff to the output file using toout object.
	toout<<"original 1: " <<str1<<endl;
	toout<<"original 2: " <<str2<<endl;
	toout<<"cononical 1: " <<list1.print()<<endl;
	toout<<"cononical 2: " <<list2.print()<<endl;
	toout<<"sum: " <<Add.print()<<endl;
	toout<<"difference: " <<Sub.print()<<endl;
	toout<<"product: " <<Prod.print()<<endl<<endl;
}

int main(){

	//declares a string.
	string line1, line2;
	//read in declaration.
	ifstream readFile;
	

	//the file that you are going to read from.
	readFile.open("input-polynomial.txt");
	//the file that you are going to output to.
	ofstream outputFile("output-polynomial.txt");
	//if the file is there enter the if statement.
	if(readFile.is_open()){
		//while there is something to read in the file, go in the while loop.
		while(readFile.good()){
			//read the first line.
			getline(readFile, line1);
			//read the second line.
			getline(readFile, line2);
			//pass the lines to the extract function which brakes the lines into numbers and 
			//does assigned operations on them.
			Extract(line1, line2);
		}
	}
	//output the file to the directory on the program and the input file.
	outputFile <<toout.str();
	//close the readin file.
	readFile.close();
	//then close the output file. 
	outputFile.close();

	//program terminates.
	return 0;
}