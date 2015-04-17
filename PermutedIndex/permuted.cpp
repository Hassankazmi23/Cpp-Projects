#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
/*
    Daisy Ramos
    CSCI 381 Summer 2014
    Project 8
 */


//allows to print to file
ostringstream toPrint;
class Node{
private:
	string word;
	int wordcount, lineNum;
	Node *next;//pointer to the next.
public:
	Node(){};//default constructor.
	void setWord(string w){word = w;}
	void setWordCounter(int n){wordcount = n;}
	void setLine(int l){lineNum = l;}
	void setNext(Node* wn){next = wn;}
	string getWord(){return word;}
	int getWordCounter(){return wordcount;}
	int getLine(){return lineNum;}
	Node* Next(){return next;}
};

//Implementation of inked list to define class List
class List{
private:
	Node* head;
public:
	List(){//head node
		Node *node = new Node();
		node->setWord("");
		node->setWordCounter(0);
		node->setLine(0);
		node->setNext(NULL);//sets pointer to null.
		head = node;
	};

	~List(){};//Destructor

    /*
     Function append returns a void.
     Count initialized to 1 in order to set correct number of times each word appears.
     The pointer is then set to null when nothing is in front. 
     A new node is take from append. 
     The second while loop checks if p is not the next element. 
     Then continues to check if the previous word is less than the next word then adds the node to the list.
     
     */
    
	void append(string w, int line){

		int count = 1;
		Node *node = new Node();
		node->setWord(w);
		node->setWordCounter(count);
		node->setLine(line);
		node->setNext(NULL);

		Node* p = head->Next();
		while(p!= NULL){
			if(p->getWord() == w){
				if(p->getLine() == line){
					count = count + 1;
					p->setWordCounter(count);
				}
			}
			p = p->Next();
		}
		p = head;
        
		while(p->Next() != NULL){
	
			if(node->getWord() < p->Next()->getWord()){
				node->setNext(p->Next());
				p->setNext(node);
				return;
			}
			p = p->Next();
		}
		
		p->setNext(node);
	}
    
    /*
     Function handle (), purpose to organize the number of times the word appears in each line.
     Sets temp as a pointer to which equals the first value of head.
     */

	void handle(){
        
		Node *temp = head->Next();
		
		while(temp != NULL && temp->Next()!= NULL){
			temp->getWord();

         ifEqual:
			if(temp->getWord() == temp->Next()->getWord()){
				//if the line number is equal to the next words line number.
				if(temp->getLine() == temp->Next()->getLine()){
					//if the word cout of the current word is greater then the next word.
					if(temp->getWordCounter() > temp->Next()->getWordCounter()){
            
						temp->Next()->setLine(0);
					}
                    
					if(temp->getWordCounter() < temp->Next()->getWordCounter()){
						temp->setLine(0);
					}
				}
				else{
					temp->getLine();
				}
				
				temp = temp->Next();
		        //repeat
				goto ifEqual;
			}
			temp = temp->Next();
		}
	}
//convertInt returns a string with the contents of a stream to  be called
	string convertInt(int number)
	{
		stringstream ss;
		ss << number;
		return ss.str();
	}

	string print(){
        
		Node *p = head->Next();
		string print = "";
		while(p!=NULL && p->Next()!=NULL){
			
			print += "\n" + p->getWord() + ": ";//appends word to string
         nextWord:
			if(p->getWord() == p->Next()->getWord()){
				//if 2 words are equal then continue.
				if(p->getWordCounter() > 1){
					if(p->getLine() == 0){
						
						print += "";
					}
					else{
						//else print the line number and the count.
						print += convertInt(p->getLine()) + "(" + convertInt(p->getWordCounter()) + ")";
					}
				}
				else{
					if(p->getLine()==0){print += ",";}//
					else{print += convertInt(p->getLine()) + ", ";}//
				}
				p = p->Next();
				goto nextWord;
			}
			//check for repitition
			if(p->getWord() != p->Next()->getWord()){
				if(p->getLine()==0){print += "";}//
				else{print += convertInt(p->getLine()) + "\n";}//
			}
			p = p->Next();
		}
		print += "\n"+p->getWord() + ": " + convertInt(p->getLine()) + "\n";
		//returns the string.
		return print;
	}

};

int main(int argc, char* argv[]){

    string fileName;
	ifstream inFile;
    ofstream outFile;
    string line1;
	int line = 1;


	List list;
    
    cout<< "Enter the name of the input file:";
    getline(cin, fileName);
    outFile.open("permuted-out.txt");
	inFile.open(fileName.c_str());
    if(inFile.fail())
    {
		cout << "Unable to find input file" << endl;
     	exit(1);
    }
	//if the file exists
	if(inFile.is_open()){
        
		while(inFile.good()){
			//reads the first line.
			getline(inFile, line1);
			stringstream one(line1);
			string s;
			while(one >> s){
				//appends the word and the line number. The line by default is set to 1.
				list.append(s, line);
			}
			line++;
		}
	}
	//calls the handle function.
	list.handle();
	toPrint<<list.print();
	//outputs to the file.
	outFile<<toPrint.str();
    cout<< "Output successful!";
    cout<< endl;
	inFile.close();
	outFile.close();
	return 0;
}