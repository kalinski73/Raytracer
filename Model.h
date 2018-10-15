#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE

#include<map>
#include<string>
#include<fstream>
#include<vector>

#include"Face.h"
#include"Matrix.h"
#include"Material.h"
#include"Object.h"

using std::ifstream;
using std::map;
using std::string;
using std::vector;


class Model: public Object{
public:
    
    //constructor
    Model(ifstream& istr){
        double wx, wy, wz;
        istr >> wx >> wy >> wz >> theta >> scale >> tx >> ty >> tz >> objFile;
        popObj(objFile);
        rot = {wx, wy, wz};
        Normalize(rot);
        kr = {1, 1, 1};
    }
    
    //get
    inline vector<double> getka(const double& f) {return faces[f].mat->ka;}
    inline vector<double> getkd(const double& f) {return faces[f].mat->kd;} 
    inline vector<double> getks(const double& f) {return faces[f].mat->ks;}
    inline double getns(const double& f){return faces[f].mat->ns;}
    inline vector<double> getko(){return {1, 1, 1};}
    inline double geteta(){return 1;}
    
    //Returns the inverse of a vector
    inline vector<double> ReverseNorm(const vector<double>& N) {return {-1 * N[0], -1 * N[1], -1 * N[2]};}
    
    //Returns the normal of the face in the model corresponding to findex
    inline vector<double> getNormal(const vector<double>& intersection, const double& findex) {return faces[findex].normal;}
    
    //If the ray intersects the model, returns the distance from the eye to the model and the index of the face in faces
    //If there is no intersection, returns (-1, -1)
    vector<double> calcDistance(const Ray& r, const vector<double>& eye);
    
    //Parses obj file
    void popObj(const string& objFile);
    
    //Transforms model from origin to location specified in driver file
    void Move();
    
    //Creates the rotation matrix for the model
    Matrix CreateRotation();
    
    //Returns the vector perpendicular to the two input vectors
    vector<double> MakePerpendicular(const vector<double>& W, const vector<double>& M);
    
    //Normalizes a vector to be unit length
    void Normalize(vector<double>& v);
    
    //Parses mtl file corresponding to obj file
    void popMaterials(const string& fileName);
    
    ~Model() = default;
    
private:

    //the axis about which to rotate:
    vector<double> rot;
    
    //degress by which to rotate:
    double theta;
    
    //uniform scaling factor to shrink or grow a model
    double scale;
    
    //defines a model to world translation
    double tx, ty, tz;
    
    //name of the .obj file
    string objFile;

    //All the vertices in the model are used stored in each row of this matrix
    Matrix verMat;
    
    //All the faces that make up the object
    vector<Face> faces;
  
    //All the materials used in the model
    vector<Material> mats;
    
    
};
#endif
