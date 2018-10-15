#include<math.h>
#include<string>
#include<tuple>
#include<vector>

#include"Sphere.h"

using std::string;
using std::tuple;
using std::vector;


vector<double> Sphere::calcDistance(const Ray& r, const vector<double>& eye){
   
    //returns the distance between the ray's pixpt and where it intersects the sphere along the pixray
    //if there is no intersection, it returns -1
    
    vector<double> C = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
    double cc = sqrt(C[0] * C[0] + C[1] * C[1] + C[2] * C[2]);
    double v = r.pixray[0] * C[0] + r.pixray[1] * C[1] + r.pixray[2] * C[2];
    double b = sqrt(cc * cc - v * v);
    //in this case, there is no intersection between the sphere and the ray
    if(b > rad){
        return {-1, -1};
    }
    double d = sqrt(rad * rad - b * b);
    vector<double> di = {r.pixpt[0] - eye[0], r.pixpt[1] - eye[1], r.pixpt[2] - eye[2]};
    double dii = sqrt(di[0] * di[0] + di[1] * di[1] + di[2] * di[2]);
   
    if((v - d - dii) < 0.0000001){
        return {-1, -1};
    }
    return {v - d - dii, -1};
    
}

vector<double> Sphere::getNormal(const vector<double>& intersection, const double& findex) {
    vector<double> n = {intersection[0] - center[0], intersection[1] - center[1], intersection[2] - center[2]};
    double sum = sqrt(n[0] * n[0] + n[1] * n[1]  + n[2] * n[2]);
    return {n[0] / sum, n[1] / sum, n[2] / sum};
}
