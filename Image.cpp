#include<fstream>
#include<iostream>
#include<limits>
#include<math.h>
#include<tuple>

#include"Image.h"

using std::endl;
using std::ifstream;
using std::max;
using std::min;
using std::ofstream;
using ::round;
using std::tuple;

void Image::setup(ifstream& istr){

    while(!istr.eof()){
        string s = "";
        istr >> s;
        if(s == "model"){
            Model* o = new Model(istr);
            objects.push_back(o);
        }
        else if(s == "eye"){
            double x, y, z;
            istr >> x >> y >> z;
            c.setEye({x, y, z});
        }
        else if(s == "look"){
            c.setLook(istr);
        }
        else if(s == "up"){
            c.setUp(istr);
        }
        else if(s == "d"){
            c.setD(istr);
        }
        else if(s == "bounds"){
            c.setBounds(istr);
        }
        else if(s == "res"){
            c.setRes(istr);
        }
        else if(s == "sphere"){
            Sphere* sp = new Sphere(istr);
            objects.push_back(sp);
        }
        else if(s == "ambient"){
            double x, y, z;
            istr >> x >> y >> z;
            ambient = {x, y, z};
        }
        else if(s == "light"){
            Light l(istr);
            lights.push_back(l);
        }
        else if(s == "recursionLevel"){
            istr >> recLev;
        }
    }
    //transform models to locations specified in driver file
    for(int i = 0; i < objects.size(); i++){
        objects[i]->Move();
    }
    //instantiate rays:
    c.popRays();

}

void Image::outppm(ofstream& ostr){
    ostr << "P3" << endl << c.getRes()[0] << " " << c.getRes()[1] << " 255" << endl;
    
    for(int i = 0; i < pixColor.size(); i++){
        for(int j = 0; j < pixColor[i].size(); j++){
            for(int k = 0; k < pixColor[i][j].size(); k++){
                ostr << (int)pixColor[i][j][k] << " ";
            }
        }
        ostr << endl;
    }
}

void Image::createImage(){
    //for all the rays in Camera c:
    pixColor.resize(c.getRaysRow());
    for(int i = 0; i < c.getRaysRow(); i++){
        pixColor[i].resize(c.getRaysColumn());
        for(int j = 0; j < c.getRaysColumn(); j++){
            pixColor[i][j] = raytrace(c.getRay(i, j), {0, 0, 0}, {1, 1, 1}, recLev, c.getEye());
        }
    }
}

void Image::colorNorm(){
    for(int i = 0; i < pixColor.size(); i++){
        for(int j = 0; j < pixColor[i].size(); j++){
            int x = max(0, min(255, (int)round(255 * pixColor[i][j][0])));
            int y = max(0, min(255, (int)round(255 * pixColor[i][j][1])));
            int z = max(0, min(255, (int)round(255 * pixColor[i][j][2])));
            pixColor[i][j][0] = x;
            pixColor[i][j][1] = y;
            pixColor[i][j][2] = z;
        }
    }  
}


vector<double> Image::raytrace(const Ray& r, vector<double> accum, const vector<double>& refatt, const int& level, const vector<double>& start){
    vector<double> dist = intersection(r, start);
    double distance = dist[0];
    double objInd = dist[1];
    double findex = dist[2];
    vector<double> intersectionPt = {r.pixpt[0] + r.pixray[0] * distance, r.pixpt[1] + r.pixray[1] * distance, r.pixpt[2] + r.pixray[2] * distance};
    
    //if the ray intersects an objects, calcualte the color of the pixel where there is the intersection
    if(distance > 0){
        vector<double> N = objects[objInd]->getNormal(intersectionPt, findex);
        vector<double> color = {ambient[0] * objects[objInd]->getka(findex)[0], ambient[1] * objects[objInd]->getka(findex)[1], ambient[2] * objects[objInd]->getka(findex)[2]};
        
        //accumulate the color from light on the pixel from each of the lights in the scene
        for(int i = 0; i < lights.size(); i++){
            
            vector<double> toL = {lights[i].loc[0] - intersectionPt[0], lights[i].loc[1] - intersectionPt[1], lights[i].loc[2] - intersectionPt[2]};
            Normalize(toL);

            vector<double> V = {start[0] - intersectionPt[0], start[1] - intersectionPt[1], start[2] - intersectionPt[2]};
            double vd = N[0] * V[0] + N[1] * V[1] + N[2] * V[2];
            
            //if the normal on a face is pointing away from the light, reverse its direction so the color can be calculated
            if(vd < 0){
                N = objects[i]->ReverseNorm(N);
            }
            
            double NdotL = N[0] * toL[0] + N[1] * toL[1] + N[2] * toL[2];
            
            //if the light reaches this point on the object and there is not another object blocking the light on this point
            if((NdotL > 0) && (Shadow(intersectionPt, lights[i]) == false)){
                color = {color[0] + objects[objInd]->getkd(findex)[0] * lights[i].color[0] * NdotL, color[1] + objects[objInd]->getkd(findex)[1] * lights[i].color[1] * NdotL, color[2] + objects[objInd]->getkd(findex)[2] * lights[i].color[2] * NdotL};
                //direction to camera
                vector<double> toC = {r.pixpt[0] - intersectionPt[0], r.pixpt[1] - intersectionPt[1], r.pixpt[2] - intersectionPt[2]};
                Normalize(toC);
                //specular reflection ray
                vector<double> spR = {2 * NdotL * N[0] - toL[0], 2 * NdotL * N[1] - toL[1], 2 * NdotL * N[2] - toL[2]};
                Normalize(spR);
                
                double c = toC[0] * spR[0] + toC[1] * spR[1] + toC[2] * spR[2];
                
                //add specular reflection using phong coefficient if the specular reflection ray is directed toward the camera
                //which happens when the angle between the two vectors is less than 90 degrees
                if(c > 0){
                    //the angle between the reflection ray and the direction to the camera determines the strength of reflection
                    c = pow(c, objects[objInd]->getns(findex));
                    color = {color[0] + (objects[objInd]->getks(findex)[0] * lights[i].color[0]) * c, color[1] + (objects[objInd]->getks(findex)[1] * lights[i].color[1]) * c, color[2] + (objects[objInd]->getks(findex)[2] * lights[i].color[2]) * c};
                }
            }
        }
        
        accum = {accum[0] + refatt[0] * color[0] * objects[objInd]->getko()[0], accum[1] + refatt[1] * color[1] * objects[objInd]->getko()[1], accum[2] + refatt[2] * color[2] * objects[objInd]->getko()[2]};
        
        //if another recursive reflection call needs to be made
        if(level > 0){
            //Ray r, vector<double> N, vector<double>* kr, vector<double> refatt, vector<double> intersectionPt, double level
            
            vector<double> flec = {0, 0, 0};
            vector<double> Uinv = {-1 * r.pixray[0], -1 * r.pixray[1], -1 * r.pixray[2]};
            
            double d = N[0] * Uinv[0] + N[1] * Uinv[1] + N[2] * Uinv[2];
            vector<double> refR = {(2 * d * N[0]) - Uinv[0], (2 * d * N[1]) - Uinv[1], (2 * d * N[2]) - Uinv[2]};
            Normalize(refR);
            
            vector<double> ll = {objects[objInd]->getkr()[0] * refatt[0], objects[objInd]->getkr()[1] * refatt[1], objects[objInd]->getkr()[2] * refatt[2]};
            
            Ray rr(intersectionPt, refR);
            
            flec = raytrace(rr , flec, ll, (level - 1), intersectionPt);
            
            accum = {accum[0] + refatt[0] * objects[objInd]->getko()[0] * flec[0], accum[1] + refatt[1] * objects[objInd]->getko()[1] * flec[1], accum[2] + refatt[2] * objects[objInd]->getko()[2] * flec[2]};
        }
        
        //if another recursive refraction call needs to be made and the object is not solid, calculate refraction 
        if((level > 0) && (objects[objInd]->getko()[0] + objects[objInd]->getko()[1] + objects[objInd]->getko()[2]) < 3){
            
            vector<double> thru = {0, 0, 0};
            Ray fraR = refractExit({-1 * r.pixray[0], -1 * r.pixray[1], -1 * r.pixray[2]}, intersectionPt, objects[objInd]->geteta(), objects[objInd]->getcenter());
            
            if(fraR.pixray.size() > 0){
                thru = raytrace(fraR, thru, {objects[objInd]->getkr()[0] * refatt[0], objects[objInd]->getkr()[1] * refatt[1], objects[objInd]->getkr()[2] * refatt[2]}, (level - 1), fraR.pixpt);
                
                accum = {accum[0] + refatt[0] * (1 - objects[objInd]->getko()[0]) * thru[0], accum[1] + refatt[1] * (1 - objects[objInd]->getko()[1]) * thru[1], accum[2] + refatt[2] * (1 - objects[objInd]->getko()[2]) * thru[2]};
            }
        }
    }
    return accum;
}


Ray Image::refractExit(const vector<double>& W, const vector<double>& pt, const double& eta, const vector<double>& center){
    vector<double> c = {pt[0] - center[0], pt[1] - center[1], pt[2] - center[2]};
    Normalize(c);
    vector<double> T1 = refractTray(W, c, 1 / eta);
    if((T1[0] + T1[1] + T1[2]) == 0){
        Ray refR({}, {});
        return refR;
    }
    else{
        double a = (center[0] - pt[0]) * T1[0] + (center[1] - pt[1]) * T1[1] + (center[2] - pt[2]) * T1[2]; 
        vector<double> exit = {pt[0] + 2 * a * T1[0], pt[1] + 2 * a * T1[1], pt[2] + 2 * a * T1[2]};
        vector<double> Nin = {center[0] - exit[0], center[1] - exit[1], center[2] - exit[2]};
        Normalize(Nin);
        vector<double> T2 = refractTray({-1 * T1[0], -1 * T1[1], -1 * T1[2]}, Nin, eta / 1);
        Ray refR(exit, T2);
        return refR;
    }
}


vector<double> Image::refractTray(const vector<double>& W, const vector<double>& N, const double& etar){
    double a = -1 * etar;
    double wn = W[0] * N[0] + W[1] * N[1] + W[2] * N[2];
    double radsq = (etar * etar) * (wn * wn - 1) + 1;

    if(radsq < 0){
        vector<double> T = {0, 0, 0};
        return T;
    }
    else{
        double b = (etar * wn) - sqrt(radsq);
        vector<double> T = {a * W[0] + b * N[0], a * W[1] + b * N[1], a * W[2] + b * N[2]};
        return T;
    }
}


bool Image::Shadow(const vector<double>& pt, const Light& lt){
    vector<double> L = {lt.loc[0] - pt[0], lt.loc[1] - pt[1], lt.loc[2] - pt[2]};
    double dtl = sqrt(L[0] * L[0] + L[1] * L[1] + L[2] * L[2]);
    
    Normalize(L);
    Ray r(pt, L);
    
    for(int i = 0; i < objects.size(); i++){
            vector<double> dist = objects[i]->calcDistance(r, pt);
            if(dist[0] > 0 && dist[0] < dtl){
                return true;
            }
    }
    return false;
}


void Image::Normalize(vector<double>& v){
    double sum = 0;
    for(int i = 0; i < v.size(); i++){
        sum += (v[i] * v[i]);
    }
    
    sum = sqrt(sum);
    
    for(int i = 0; i < v.size(); i++){
        v[i] = v[i] / sum;
    }
}


vector<double> Image::intersection(const Ray& r, const vector<double>& start){
    double distance = -1;
    double index = 0;
    double findex = 0;
    for(int i = 0; i < objects.size(); i++){
        vector<double> dist = objects[i]->calcDistance(r, start);
        if((((dist[0] != -1) && ((dist[0] < distance))) || (distance == -1))){
            distance = dist[0];
            index = i;
            findex = dist[1];
        }
    }
    return {distance, index, findex};
}


void Image::cleanUp(){
    for(int i = objects.size() - 1; i >= 0; i--){
        delete objects[i];
    }
    objects.clear();
}
    
