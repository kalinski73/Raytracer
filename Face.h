#ifndef FACE_H_INCLUDE
#define FACE_H_INCLUDE

#include<string>
#include<vector>

#include"Material.h"

using std::string;
using std::vector;

struct Face{
    
    //constructor
    Face(int a, int b, int d, string m):a(a), b(b), d(d), mat_name(m){}
    
    //calculates and populates normal, the vector normal to the face
    void calcNormal();
    
    //data
    vector<vector<double>*> vertices;
    vector<double> normal;
    int a, b, d;
    string mat_name;
    Material* mat;
    
};
#endif


