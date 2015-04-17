#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <sstream>
using namespace std;
/*
  CSCI381 Summer 2014
  @Daisy Ramos
  Project 4

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

int main()
{

ifstream infile("input.txt");
ofstream outfile("output.txt");
string line;

/*Intiliazing safe array to A and B*/
SM<int> A, B;

int rowMinA = 0, rowMaxA = 0, colMinA = 0, colMaxA = 0;


while(getline(infile, line)){


    stringstream ss;
    ss << line;


    ss >> rowMinA;
    ss >> rowMaxA;

    if(ss.good()){
        ss >> colMinA;
        ss >> colMaxA;

        /*Intiliazing safe array to Matrix 1*/
        SM<int> m1(rowMinA, rowMaxA, colMinA, colMaxA);
        getline(infile, line);
        stringstream stream;
        stream << line;

        for(int i = rowMinA; i <= rowMaxA; i++){
            for(int k = colMinA; k <= colMaxA; k++){
                stream >> m1[i][k];
            }
        }

        outfile << "Matrix 1 :" << endl;
        outfile << m1;

        A = m1;

    }
    else{
        getline(infile, line);
        stringstream stream;
        stream << line;

        SM<int> m1(rowMinA, rowMaxA);

        for(int i = 0; i < rowMinA; i++){
            for(int k = 0; k < rowMaxA; k++){
                stream >> m1[i][k];
            }
        }
        outfile << "Matrix 1 :" << endl;
        outfile << m1;

        A = m1;
    }

    getline(infile, line);
    stringstream ss2;
    ss2 << line;
    ss2 >> rowMinA;
    ss2 >> rowMaxA;

    if(ss2.good()){
        ss2 >> colMinA;
        ss2 >> colMaxA;

        /*Intializing safe array to Matrix 2*/
        SM<int> m2(rowMinA, rowMaxA, colMinA, colMaxA);
        getline(infile, line);
        stringstream stream2;
        stream2 << line;

        for(int i = rowMinA; i <= rowMaxA; i++){
            for(int k = colMinA; k <= colMaxA; k++){
                stream2 >> m2[i][k];
            }
        }

        outfile << "Matrix 2 :" << endl;
        outfile << m2;

        B = m2;

    }
    else{
        getline(infile, line);
        stringstream stream2;
        stream2 << line;

        SM<int> m2(rowMinA, rowMaxA);

        for(int i = 0; i < rowMinA; i++){
            for(int k = 0; k < rowMaxA; k++){
                stream2 >> m2[i][k];
            }
        }
        outfile << "Matrix 2 :" << endl;
        outfile << m2;

        B = m2;
    }

    outfile << "Matrix 1 * Matrix 2 :" << endl;

    if((A.getCols()) == (B.getRows()))
        outfile << A * B;
    else
        outfile << "IMPOSSIBLE" << endl;
    outfile << endl;
}

infile.close();
outfile.close();
return 0;


}