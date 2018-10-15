#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

#include<fstream>
#include<vector>

#include"Ray.h"

using std::ifstream;
using std::vector;

class Camera{
public:
    //constructor
    Camera(): eye(), look(), up(), bounds(), res(), d(), rays(){}
    
    //get
    inline int getRaysRow(){return rays.size();}
    inline int getRaysColumn(){return rays[0].size();}
    inline Ray getRay(const int& i, const int& j){return rays[i][j];}
    inline vector<double> getEye(){return eye;}
    inline vector<double> getRes(){return res;}
    
    //set
    inline void setEye(const vector<double>& v){eye = v;}
    void setLook(ifstream& istr);
    void setUp(ifstream& istr);
    void setBounds(ifstream& istr);
    void setRes(ifstream& istr);
    void setD(ifstream& istr);
    
    
    //Populates the vector rays with rays corresponding to the camera position/direction and the resolution of the image
    void popRays();
    
    //Normalizes a vector to be unit length
    void Normalize(vector<double>& v);
    
    //Returns the cross product of two vectors
    vector<double> Cross(const vector<double>& v, const vector<double>& w);
    
private:
    vector<double> eye, look, up, bounds, res;
    double d;
    vector<vector<Ray>> rays;
       
};
#endif
