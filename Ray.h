#ifndef RAY_H_INCLUDE
#define RAY_H_INCLUDE

#include<vector>

using std::vector;

struct Ray{
 
    //constructor
    Ray(): pixpt(), pixray(){}
    Ray(vector<double> pixpt, vector<double> pixray): pixpt(pixpt), pixray(pixray){}
    
    //data
    vector<double> pixpt;
    vector<double> pixray;

};

#endif
