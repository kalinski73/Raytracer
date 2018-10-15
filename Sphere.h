#ifndef SPHERE_H_INCLUDE
#define SPHERE_H_INCLUDE

#include<fstream>
#include<string>
#include<tuple>
#include<vector>

#include"Object.h"

using std::ifstream;
using std::string;
using std::tuple;
using std::vector;

class Sphere: public Object{
public:
  
    //constructor
     Sphere(ifstream& istr){
        double cx, cy, cz, kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb, kr0, kr1, kr2, kor, kog, kob;
        istr >> cx >> cy >> cz >> rad >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb >> kr0 >> kr1 >> kr2 >> kor >> kog >> kob >> eta;
        center = {cx, cy, cz};
        ka = {kar, kag, kab};
        kd = {kdr, kdg, kdb};
        ks = {ksr, ksg, ksb};
        kr = {kr0, kr1, kr2};
        ko = {kor, kog, kob};
    }
    
  
    //get
    inline vector<double> getka(const double& f){return ka;}
    inline vector<double> getkd(const double& f){return kd;}
    inline vector<double> getks(const double& f){return ks;}
    inline double getns(const double& ){return 16;}
    inline vector<double> getko(){return ko;}
    inline double geteta(){return eta;}
    inline vector<double> getcenter(){return center;}
    inline vector<double> ReverseNorm(const vector<double>& N){return N;}
    
    //set
    inline void setkr(const double& x, const double& y, const double& z){kr = {x, y, z};}
    
    
    //Returns the normal of the sphere relative to the point where it is intersected by a ray
    vector<double> getNormal(const vector<double>& intersection, const double& findex);
    
    //If there is an intersection between the ray and the sphere, returns the distance from the rays location on the image plane to the intersection, -1
    //If there is no intersection, returns -1, "", -1
    vector<double> calcDistance(const Ray& r, const vector<double>& eye);
    
    //destructor
    ~Sphere() = default;
    
    
private:
    //the point at the center of the sphere
    vector<double> center, ka, kd, ks, ko;
    //the radius of the sphere
    double rad, eta;
    
};
#endif
