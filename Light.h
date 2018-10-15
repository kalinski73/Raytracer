#ifndef LIGHT_H_INCLUDE
#define LIGHT_H_INCLUDE

#include<fstream>
#include<vector>

using std::ifstream;
using std::vector;


struct Light{
    
    //constructor
    Light(ifstream& istr): loc(), color(){
        double x, y, z, w;
        istr >> x >> y >> z >> w;
        loc = {x, y, z, w};
        
        istr >> x >> y >> z;
        color = {x, y, z};
    }

    //data
    vector<double> loc, color;
    
};
#endif
