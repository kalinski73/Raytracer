#ifndef OBJECT_H_INCLUDE
#define OBJECT_H_INCLUDE

#include<string>
#include<vector>

#include"Ray.h"

using std::string;
using std::vector;

class Object{
public:
    
    //get
    virtual vector<double> getka(const double& f) = 0;
    virtual vector<double> getkd(const double& f) = 0;
    virtual vector<double> getks(const double& f) = 0;
    virtual double getns(const double& f) = 0;
    virtual vector<double> getko() = 0;
    virtual double geteta() = 0;
    virtual inline vector<double> getcenter(){return {};}
    virtual vector<double> ReverseNorm(const vector<double>& N) = 0;
    inline vector<double> getkr(){return kr;}
    virtual vector<double> getNormal(const vector<double>& intersection, const double& findex) = 0;
    
    //returns the distance from the raypt to the object, or -1 if they don't intersect
    virtual vector<double> calcDistance(const Ray& r, const vector<double>& eye) = 0;
    
    //Transforms the location of the model based on the specifications given in the driver
    //Only used in model
    virtual inline void Move(){};
    
    //destructor
    virtual ~Object() = default;
    

protected:
    //normal is the normal to the surface of the object
    //ka, kd, ks are the ambient, diffuse, and specular lighting coefficents
    
    vector<double> normal, kr;
    
};
#endif
