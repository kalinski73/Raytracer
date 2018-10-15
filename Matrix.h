#ifndef MATRIX_H_INCLUDE
#define MATRIX_H_INCLUDE

#include<vector>

using std::vector;

struct Matrix{

    //constructor
    Matrix(vector<vector<double>> m): m(m){}
    Matrix(){}
    
    //Returns the cross product of 2 matrices, m x n
    //If the dimensions are not compatable, error is printed and n is returned
    Matrix CrossProduct(const Matrix& n);
    
    //Returns a pointed to the specified row in a matrix
    vector<double>* getRow(const int& row);
    
    //prints the dimensions of the matrix
    void Dim();
    
    //Calculates the determinant of 3x3 matricies
    //Returns 0 if the matrix is not 3x3 & prints an error
    double Det3();
    
    //Inverts 3x3 Matrix
    //If the matrix is not 3x3 or is singular, a message is printed and the matrix remains unchanged
    void Invert3(const double& det);
    
    //Returns the transpose of a matrix
    //The original matrix remains unchanged
    Matrix Transpose();


    vector<vector<double>> m;
    
};
#endif
