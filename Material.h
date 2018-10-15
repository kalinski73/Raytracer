#ifndef MATERIAL_H_INCLUDE
#define MATERIAL_H_INCLUDE

#include<string>
#include<vector>

using std::string;
using std::vector;

struct Material{
    
    //constructor
    Material(string name, double ns, vector<double> ka, vector<double> kd, vector<double> ks): name(name), ns(ns), ka(ka), kd(kd), ks(ks){}
    
    //data
    string name;
    double ns;
    vector<double> ka, kd, ks;
    
    
};
#endif


