#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#include<fstream>
#include<tuple>
#include<vector>

#include"Camera.h"
#include"Light.h"
#include"Model.h"
#include"Object.h"
#include"Sphere.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::tuple;
using std::vector;

class Image{
public:
    //constructor
    Image(ifstream& istr): c(), objects(), lights(), ambient(), pixColor(){
        setup(istr);
    }
    
    //Reads and stores the information from the driver file
    //Transforms the objects to their specifies positions
    //Instantiates the rays based on camera specifications
    void setup(ifstream& istr);
    
    //Writes the image data stored in pixColor to the ofstream
    void outppm(ofstream& ostr);
    
    //Calls raytrace on every Ray in c and stores the result in the corresponding pixColor
    void createImage();
    
    //Normalizes the values in pixColor to be from 0-225 instead of 0-1 in preparation for output
    void colorNorm();
    
    //Determines the first object r intersects with and calculates the color of the object at the point of intersection
    vector<double> raytrace(const Ray& r, vector<double> accum, const vector<double>& refatt, const int& level, const vector<double>& start);
    
    //Computes the refraction ray exiting the sphere based upon the ray entering the sphere
    Ray refractExit(const vector<double>& W, const vector<double>& pt, const double& eta, const vector<double>& center);
    
    //Computes the refraction ray based on the ray entering the sphere, the surface normal, and eta of the outside / eta of the inside
    vector<double> refractTray(const vector<double>& W, const vector<double>& N, const double& etar);
    
    //Returns true if there is an object between pt and the location of lt
    bool Shadow(const vector<double>& pt, const Light& lt);
    
    //Normalizes a vector to be unit length
    void Normalize(vector<double>& v);
    
    //returns the distance to the closest object, the index of the object in objects, the index of the face for models or -1 for spheres
    
    //if the distance is -1, the ray didn't intersect any objects
    vector<double> intersection(const Ray& r, const vector<double>& start);
    
    //Deletes all spheres and models
    void cleanUp();
    
    
private:
    Camera c;
    vector<Object*> objects;
    vector<Light> lights;
    vector<double> ambient;
    int recLev;
    vector<vector<vector<double>>> pixColor;
    
};
#endif
