#include<iostream>
#include <cstdlib>
#include <cassert>
#include<fstream>
#include<sstream>
using namespace std;

/*
  CSCI381 Summer 2014
  @Daisy Ramos
  Project 6

 */


/*Generic Safe Array Class*/

template <class T>
class SA;

template <class T>
class SM;

template <class T>
ostream& operator<<(ostream& os, SA<T> s);

template <class T>
ostream& operator<< (ostream& os, SM<T> s);

template < class T > class SA {
private:
    int low, high;
    T *p;
public:
// default constructor
// allows for writing things like SA a;
    SA() {
        low = 0;
        high = -1;
        p = NULL;
    }
// 2 parameter constructor lets us write
// SA x(10,20);
    SA(int l, int h) {
        if ((h - l + 1) <= 0) {
            cout << "constructor bounds error" << endl;
            exit(1);
        }
        low = l;
        high = h;
        p = new T[h - l + 1];
    }
// single parameter constructor lets us
// create a SA almost like a "standard" one by writing
// SA x(10); and getting an
//array x indexed from 0 to 9
    SA(int i) {
        low = 0;
        high = i - 1;
        p = new T[i];
    }
// copy constructor for pass by value and initialization
    SA(const SA & s) {
        int size = s.high - s.low + 1;
        p = new T[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
    }
// destructor
    ~SA() {
        delete[]p;
    }
//overloaded [] lets us write
//SA x(10,20); x[15]= 100;
    T & operator[](int i) {
        if (i < low || i > high) {
            cout << "index " << i << " out of range" << endl;
            exit(1);
        }
        return p[i - low];
    }
// overloaded assignment lets us assign
// one SA to another
    SA & operator=(const SA & s) {
        if (this == &s)
            return *this;
        delete[]p;
        int size = s.high - s.low + 1;
        p = new T[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
        return *this;
    }
    int* operator+(int i) {
        return &p[low+i];
    }

// overloads << so we
//can directly print SAs


    friend ostream& operator<< <T>(ostream& os, SA<T> s);
};

template <class T>
ostream & operator<<(ostream & os, SA < T > s) {
    int size = s.high - s.low + 1;
    for (int i = 0; i < size; i++)
        os << s.p[i] << endl;
    return os;
}

/*Safe array within safe array*/

template < class T > class SM {
private:

    int row_low;
    int row_high;
    int col_low;
    int col_high;
    SA < SA < T > > matrix;

public:
    SM(int rows, int cols) {
        if (rows <= 0 || cols <= 0) {
            cout << "Please enter a valid row and columns size" << endl;
            exit(1);
        }
        row_low = 0;
        row_high = rows - 1;
        col_low = 0;
        col_high = cols - 1;
        matrix = SA < SA < T > >(rows);
        for (int j = 0; j < rows; j++)
            matrix[j] = SA < T > (cols);
    }
    SM() {
    }
    SM(int row_min, int row_max, int col_min, int col_max) {
        if ((row_max - row_min + 1) <= 0) {
            cerr << "constructor error in Matrix bounds definition" << endl;
            exit(1);
        }
        row_low = row_min;
        row_high = row_max;
        col_low = col_min;
        col_high = col_max;
        matrix = SA < SA < T > >(row_min, row_max);
        for (int i = row_min; i <= (row_max); i++)
            matrix[i] = SA < T > (col_min, col_max);
    }

    SM(int square_size) {
        row_low = 0;
        row_high = square_size - 1;
        col_low = 0;
        col_high = square_size - 1;
        matrix = SA < SA < T > >(square_size);
        for (int j = 0; j < square_size; j++)
            matrix[j] = SA < T > (square_size);
    }
//destructor
    ~SM() {

    }

    SA < T > &operator[](int i) {
        if (i < row_low || i > row_high) {
            cout << "index " << i << " out of range in Matrix" << endl;
            exit(1);
        }
        return matrix[i];
    }
    /*Matrix Multiplication*/

    SM < T > operator*(SM & s) {
        if((col_high - col_low + 1)!= (s.row_high - s.row_low + 1)) {
            return 0;
        }
        int rows = (row_high - row_low + 1);
        int cols = (s.col_high - s.col_low + 1);
        SM < int >result(rows, cols);
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols; c++){
                result[r][c] = 0;
            }//for
        }//for
        for (int r = 0; r < rows; r++) {

            for (int c = 0; c < cols; c++) {

                for (int i = 0; i < (s.row_high - s.row_low + 1); i++) {
                    result[r][c] += ((*this)[r + row_low][i + col_low]) * (s[i + s.row_low][c + s.col_low]);
                }
            }
        }
        return result;
    }

    int getRows(){
        return row_high - row_low + 1;
    }

    int getCols(){
        return col_high - col_low + 1;
    }

    SA < T >* operator+(int i) {
        return &matrix[i];
    }
    friend ostream& operator<< <T>(ostream& os, SM<T> s);

};

template <class T>
ostream & operator<<(ostream & os, SM < T > s) {
    for (int i = s.row_low; i <= s.row_high; i++) {
        for (int j = s.col_low; j <= s.col_high; j++) {
            os << s.matrix[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

void printMatrix(int **matrix, int rows, int columns)
{
    for (int i = 0 ; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << '\n';
    }
}


/*Class VNT ("Very Neat Table") to test with safe matrix class by handling an m x n matrix such that 
the entries of each row are in sorted order from left to 
right and the entries of each column are in sorted order from top to bottom.
*/
class VNT{
	public:
		SM<int> vnt;
		int p;
        int x;
        int y;
        int temp_x;
        int temp_y;
		
	public:		
		VNT(int a,int b){
		vnt = SM<int>(a,b);
		 x = a - 1;
		 y = b - 1;
		 temp_x = 0;
		 temp_y = 0;
		for(int i = 0 ; i < a ; i++)
			for(int j = 0 ; j < b ; j++)
				vnt[i][j] = 0;
		}
		
		
  friend ostream & operator << (ostream & os, VNT  &a) {
	for (int i = a.x; i != -1; i--) {
	    for (int j = a.y; j != -1; j--) {
		if(a.vnt[i][j] != 0)
		cout << a.vnt[i][j] << " ";

	    }
	    cout << endl;
	}
	return os;

}//<<


//adds to a non full vnt
    void add(int a){
		if(temp_y < y){
		
		temp_x++;
		
		if(temp_x < x)
			{
			cout << "full";
			return;
			}
		else{
			
		temp_y = 0;
			
		vnt[temp_x][temp_y] = a;
	
		
	for(int i = 0 ; i <= y ; i++)	
			for(int j = 0 ; j <= y ; j++)
					if(vnt[temp_x][i]>vnt[temp_x][j])
						swap(vnt[temp_x][i],vnt[temp_x][j]);	
	
		for(int i = 0 ; i <= x ; i++)	
			for(int j = 0 ; j <= x ; j++)
					if(vnt[i][temp_y] > vnt[j][temp_y])
						swap(vnt[i][temp_y],vnt[j][temp_y]);	


		
	
		temp_y++;
		return;
		}
}
		vnt[temp_x][temp_y] = a;
	
	
		for(int i = 0 ; i <= y ; i++)	
			for(int j = 0 ; j <= y ; j++)
					if(vnt[temp_x][i] > vnt[temp_x][j])
						swap(vnt[temp_x][i] , vnt[temp_x][j]);	
	
		for(int i = 0;i <= x;i++)	
			for(int j=0;j <= x;j++)
					if(vnt[i][temp_y] > vnt[j][temp_y])
						swap(vnt[i][temp_y],vnt[j][temp_y]);	

		temp_y++;
		
	}//add
	
	//extract smallest element 
	int getMin(){
	for(int i=x;i!=-1;i--)	
	for(int j=y;j!=-1;j--)	
					if(vnt[i][j]!=0)
					return vnt[i][j];
					
		
	
	}//getMin
	
	//generic sort n x n 
	static void sort(int k[], int size){
		for(int i = 0 ; i < size ; i++)	
			for(int j = 0 ; j < size ; j++)
				if(k[i] < k[j])
					swap(k[i],k[j]);	
		
}//sort

    //returns true if element is in vnt
    bool find(int h){
	for(int i=0;i<=x;i++)	
	for(int j=0;j<=y;j++)	
					if(vnt[i][j]==h)
					return true;
					return false;
		
	}//find

   
};

int main(){

  string line; int m, n, size;
	//ifstream makes an input stream from vnt-in.txt
	ifstream myfile ("matrix-in.txt");
	if (myfile.is_open()){
	    while ( getline (myfile,line) ) {
		myfile >> m;
		myfile >> n;
		myfile >> size;
		int *array = new int[size];
		for(int i=0; i<size; i++) {
			myfile >> array[i];
		}
		cout << "Original Array: ";
		for(int i =0; i<size; i++) {
			cout <<array[i] << " ";
		} cout << endl;
		
		cout << "Sorted Array: ";
		VNT::sort(array,size);
		for(int i = 0;i < size;i++) {
			cout << array[i] << " ";
			if(i<0) 
			cout << "Impossible";
		}
		cout << endl;
		cout << endl;
	    }
	    myfile.close();
	  }	
      else 
      cout << "Unable to open file"; 

	return 0;
	
	
}
