#include<math.h>
#include<vector>

#include"Face.h"

using std::vector;

void Face::calcNormal(){

    //calculate the normal for each face and store
    
    vector<double> BA = {vertices[1]->at(0) - vertices[0]->at(0), vertices[1]->at(1) - vertices[0]->at(1), vertices[1]->at(2) - vertices[0]->at(2)};
    
    vector<double> CA = {vertices[2]->at(0) - vertices[0]->at(0), vertices[2]->at(1) - vertices[0]->at(1), vertices[2]->at(2) - vertices[0]->at(2)};
    
    double x = BA[1] * CA[2] - BA[2] * CA[1];
    double y = BA[2] * CA[0] - BA[0] * CA[2];
    double z = BA[0] * CA[1] - BA[1] * CA[0];
    
    double sum = sqrt(x * x + y * y + z * z);
    normal = {x / sum, y / sum, z / sum};
}


