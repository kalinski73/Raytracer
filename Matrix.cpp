#include<iostream>
#include<vector>

#include"Matrix.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::vector;


Matrix Matrix::CrossProduct(const Matrix& n){
    if(m[0].size() != n.m.size()){
        cerr << "Matrix dimensions not compatable" << endl;
        return n;
    }
    else{
        vector<vector<double>> mn;
        for(int i = 0; i < m.size(); i++){
            vector<double> vec;
            for(int j = 0; j < n.m[0].size(); j++){
                double d = 0;
                for(int k = 0; k < m[0].size(); k++){
                    d += m[i][k]*n.m[k][j];
                }
                vec.push_back(d);
            }
            mn.push_back(vec);
        }
        Matrix mm(mn);
        return mm;
    }
}


vector<double>* Matrix::getRow(const int& row){
    return &m[row];
}


void Matrix::Dim(){
    
    cout << " # of rows: " << m.size() << endl;
    cout << " # of columns: " << m[0].size() << endl;
    
}


double Matrix::Det3(){
    if((m.size() != 3) || (m[0].size() != 3)){  
        cerr << "Det3: Matrix is not 3x3" << endl;
        return 0;
    }
    else{
        double i = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
        double j = m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]);
        double k = m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
        i += j + k;
        return i;
    }
}


void Matrix::Invert3(const double& det){
    
    if((m.size() != 3) || (m[0].size() != 3)){  
        cerr << "Invert3: Matrix is not 3x3" << endl;
    }
    else if(det == 0){
        cerr << "Matrix is Singular" << endl;
    }
    else{
        double a = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / det;
        double b = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / det;
        double c = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / det;
        
        double d = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / det;
        double e = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
        double f = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / det;
        
        double g = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
        double h = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / det;
        double i = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / det;
    
        m = {{a, b, c}, {d, e, f}, {g, h, i}};
        Matrix M = this -> Transpose();
        m = M.m;
    }
}


Matrix Matrix::Transpose(){
    vector<vector<double>> t;
    
    for(int i = 0; i < m[0].size(); i++){
        vector<double> v;
        for(int j = 0; j < m.size(); j++){
            v.push_back(m[j][i]);
        }
        t.push_back(v);
    }
    Matrix T(t);
    return T;
}


ostream& operator<< (ostream& os, const Matrix& obj) {
    for(int i = 0; i < obj.m.size(); i++){
        for(int j = 0; j < obj.m[0].size(); j++){
            os << obj.m[i][j] << " ";
        }
        os << endl;
    }
    os << endl;
    return os;
}

