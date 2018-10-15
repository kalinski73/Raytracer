#include<fstream>
#include<math.h>
#include<vector>

#include"Camera.h"

using std::ifstream;
using std::vector;


void Camera::setLook(ifstream& istr){
    double x, y, z;
    istr >> x >> y >>z;
    look = {x, y, z};
}

void Camera::setUp(ifstream& istr){
    double x, y, z;
    istr >> x >> y >>z;
    up = {x, y, z};
}

void Camera::setBounds(ifstream& istr){
    double x, y, z, w;
    istr >> x >> y >> z >> w;
    bounds = {x, y, z, w};
}

void Camera::setRes(ifstream& istr){
    double x, y;
    istr >> x >> y;
    res = {x, y};
}

void Camera::setD(ifstream& istr){
    istr >> d;
}


void Camera::popRays(){
    
    rays.resize(res[1]);
    for(int i = 0; i < rays.size(); i++){
        rays[i].resize(res[0]);
    }
    for(int i = 0; i < res[0]; i++){
        for(int j = 0; j < res[1]; j++){
            double px = i / (res[0] - 1) * (bounds[2] - bounds[0]) + bounds[0];
            double py = j / (res[1] - 1) * (bounds[3] - bounds[1]) + bounds[1];

            
            vector<double> WV = {look[0] - eye[0], look[1] - eye[1], look[2] - eye[2]};
            Normalize(WV);
            
            vector<double> UV = Cross(up, WV);
            Normalize(UV);
            
            vector<double> VV = Cross(WV, UV);
            
            rays[res[1] - 1 - j][res[0] - 1 - i].pixpt = {eye[0] + (d * WV[0]) + (px * UV[0]) + (py * VV[0]), eye[1] + (d * WV[1]) + (px * UV[1]) + (py * VV[1]), eye[2] + (d * WV[2]) + (px * UV[2]) + (py * VV[2])};

            vector<double> pixray = {rays[res[1] - 1 - j][res[0] - 1 - i].pixpt[0] - eye[0], rays[res[1] - 1 - j][res[0] - 1 - i].pixpt[1] - eye[1], rays[res[1] - 1 - j][res[0] - 1 - i].pixpt[2] - eye[2]};
            Normalize(pixray);

            rays[res[1] - 1 - j][res[0] - 1 - i].pixray = pixray;
        }
    }
}


void Camera::Normalize(vector<double>& v){
    double sum = 0;
    for(int i = 0; i < v.size(); i++){
        sum += (v[i] * v[i]);
    }
    
    sum = sqrt(sum);
    
    for(int i = 0; i < v.size(); i++){
        v[i] = v[i] / sum;
    }
}

vector<double> Camera::Cross(const vector<double>& v, const vector<double>& w){
    double x = v[1] * w[2] - v[2] * w[1];
    double y = v[2] * w[0] - v[0] * w[2];
    double z = v[0] * w[1] - v[1] * w[0];
    vector<double> u = {x, y, z};
    return u;
}

