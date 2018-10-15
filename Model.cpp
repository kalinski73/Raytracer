#include<fstream>
#include<iostream>
#include<map>
#include<math.h>
#include<sstream>
#include<string>
#include<vector>

#include"Model.h"

using std::cerr;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

#define PI 3.14159265


vector<double> Model::calcDistance(const Ray& r, const vector<double>& eye){
    double dist = -1;
    double index = -1;
    
    for(int i = 0; i < faces.size(); i++){
       
        vector<double> vx = {faces[i].vertices[0]->at(0) - faces[i].vertices[1]->at(0), faces[i].vertices[0]->at(0) - faces[i].vertices[2]->at(0), r.pixray[0]};
        vector<double> vy = {faces[i].vertices[0]->at(1) - faces[i].vertices[1]->at(1), faces[i].vertices[0]->at(1) - faces[i].vertices[2]->at(1), r.pixray[1]};
        vector<double> vz = {faces[i].vertices[0]->at(2) - faces[i].vertices[1]->at(2), faces[i].vertices[0]->at(2) - faces[i].vertices[2]->at(2), r.pixray[2]};
        
        Matrix M({vx, vy, vz});
        double det = M.Det3();
        
        //Matrix isn't singular
        if(det != 0){
            Matrix Y({{faces[i].vertices[0]->at(0) - r.pixpt[0]}, {faces[i].vertices[0]->at(1) - r.pixpt[1]}, {faces[i].vertices[0]->at(2) - r.pixpt[2]}});
            Matrix M1({{Y.m[0][0], M.m[0][1], M.m[0][2]}, {Y.m[1][0], M.m[1][1], M.m[1][2]}, {Y.m[2][0], M.m[2][1], M.m[2][2]}});
            Matrix M2({{M.m[0][0], Y.m[0][0], M.m[0][2]}, {M.m[1][0], Y.m[1][0], M.m[1][2]}, {M.m[2][0], Y.m[2][0], M.m[2][2]}});
            Matrix M3({{M.m[0][0], M.m[0][1], Y.m[0][0]}, {M.m[1][0], M.m[1][1], Y.m[1][0]}, {M.m[2][0], M.m[2][1], Y.m[2][0]}});
            
            double de = M.Det3();
            double beta = M1.Det3() / de;
            double gamma = M2.Det3() / de;
            double t = M3.Det3() / de;
            
            if(beta >= 0 && gamma >= 0 && (beta + gamma) <= 1.0000001){
                if(dist > t || dist == -1){
                    dist = t;
                    index = i;
                }
            }
        }
    }

    if(dist > 0.000001){
        return {dist, index};
    }
    return {-1, index};
}


void Model::popObj(const string& objFile){
    ifstream istr(objFile);
    if(istr.fail()){
        cerr << "Vmatrix ifstream failed" << endl;
    }
    string s, c, mtl, name;
    while(getline(istr, s)){
        istringstream i(s);
        i >> c;
        
        if(c == "v"){
            double x, y, z;
            i >> x >> y >> z;
            vector<double> d = {x, y, z, 1};
            //add d to matrix of vertices
            verMat.m.push_back(d);
        }
        else if(c == "f"){
            //populate face struct, store in vector<Face> faces
            vector<double> x;
            i >> c;
            istringstream s0(c);
            int a, b, d;
            s0 >> a;
            i >> c;
            istringstream s1(c);
            s1 >> b;
            i >> c;
            istringstream s2(c);
            s2 >> d;
         
            Face f(a - 1, b - 1, d - 1, mtl);
            faces.push_back(f);
        }
        else if(c == "mtllib"){
            //open mtl file, stream into materials vector<string>
            i >> name;
            popMaterials(name);
        }
        else if(c == "usemtl"){
            i >> mtl;
        }
    }
    for(int i = 0; i < faces.size(); i++){
        for(int j = 0; j < mats.size(); j++){
            if(mats.at(j).name == faces.at(i).mat_name){
                faces.at(i).mat = &mats.at(j);
            }
        }
    }
}


void Model::Move(){
    
    //rotate, scale , and translate the model
    //get rotation matrix
    Matrix mr = CreateRotation();
   
    //create scaling matrix
    vector<double> sa = {scale, 0, 0, 0};
    vector<double> sb = {0, scale, 0, 0};
    vector<double> sc = {0, 0, scale, 0};
    vector<double> sd = {0, 0, 0, 1};
    vector<vector<double>> s1 = {sa, sb, sc, sd};
    Matrix ms(s1);

    //create translation matrix    
    vector<double> ta = {1, 0, 0, tx};
    vector<double> tb = {0, 1, 0, ty};
    vector<double> tc = {0, 0, 1, tz};
    vector<double> td = {0, 0, 0, 1};
    vector<vector<double>> t1 = {ta, tb, tc, td};
    Matrix mt(t1);
   
    //multiply translation*scaling*rotation*original verticies  = new verticies
    Matrix ts = mt.CrossProduct(ms);
    Matrix tsr = ts.CrossProduct(mr);

    verMat.m = ((tsr.CrossProduct(verMat.Transpose())).Transpose()).m;

    //calculate face pointers to vertices
    //calculate normal for each face in model
    for(int i = 0; i < faces.size(); i++){
        vector<double>* u = verMat.getRow(faces[i].a);
        vector<double>* v = verMat.getRow(faces[i].b);
        vector<double>* w = verMat.getRow(faces[i].d);
        vector<vector<double>*> V = {u, v, w};
        faces[i].vertices = V;
        faces[i].calcNormal();
    }
}


Matrix Model::CreateRotation(){
    //pick axis M, not parallel to rot
    //pick the smallest term in w, set it to 1, and renormalize to create m
    vector<double> M = rot;
    if(abs(rot[1]) < abs(rot[2]) && abs(rot[1]) < abs(rot[2])){
        M[1] = 1;
    } else if(abs(rot[2]) < abs(rot[0])){
        M[2] = 1;
    } else{
        M[0] = 1;
    }
    Normalize(M);
    
    //create perpendicular vector U by U = rotxM
    vector<double> U = MakePerpendicular(rot, M);
    Normalize(U);

    //now create vector V perpendicularto U and rot by V = rotxU
    vector<double> V = MakePerpendicular(rot, U);
    
    //now put these together to create Romega
    vector<double> va = {U[0], U[1], U[2], 0};
    vector<double> vb = {V[0], V[1], V[2], 0};
    vector<double> vc = {rot[0], rot[1], rot[2], 0};
    vector<double> vd = {0, 0, 0, 1};
    vector<vector<double>> v1 = {va, vb, vc, vd};
    Matrix RO(v1);
    
    vector<double> za = {cos(theta * PI / 180.0), (-1) * sin(theta * PI / 180.0), 0, 0};
    vector<double> zb = {sin(theta * PI / 180.0), cos(theta * PI / 180.0), 0, 0};
    vector<double> zc = {0, 0, 1, 0};
    vector<double> zd = {0, 0, 0, 1};
    vector<vector<double>> z1 = {za, zb, zc, zd};
    Matrix Rz(z1);
    
    Matrix ROTr = RO.Transpose();
    
    Matrix mr = ROTr.CrossProduct(Rz);
    mr = mr.CrossProduct(RO);

    return mr;
}

vector<double> Model::MakePerpendicular(const vector<double>& W, const vector<double>& M){
    double Ux = W[1]*M[2] - W[2]*M[1]; 
    double Uy = W[2]*M[0] - W[0]*M[2]; 
    double Uz = W[0]*M[1] - W[1]*M[0]; 
    vector<double> U = {Ux, Uy, Uz};
    return U;
}    
    

void Model::Normalize(vector<double>& v){
    double sum = 0;
    for(int i = 0; i < v.size(); i++){
        sum += (v[i] * v[i]);
    }
    sum = sqrt(sum);
    
    for(int i = 0; i < v.size(); i++){
        v[i] = v[i] / sum;
    }
}


void Model::popMaterials(const string& fileName){
    ifstream istr(fileName);
    if(istr.fail()){
        cerr << "Material File ifstream failed" << endl;
        return;
    }
    string s, c;
    string name = "";
    vector<double> ka, kd, ks;
    double ns, x, y, z;
    
    while(getline(istr, s)){
        istringstream i(s);
        i >> c;
        if(c == "newmtl"){
            if(name != ""){
                Material m(name, ns, ka, kd, ks);
                mats.push_back(m);
            }
            i >> name;
        }
        if(c == "Ns"){
            i >> ns;
        }
        if (c == "Ka"){
            i >> x >> y >> z;
            ka = {x, y, z};
        }
        if(c == "Kd"){
            i >> x >> y >> z;
            kd = {x, y, z};
        }
        if(c == "Ks"){
            i >> x >> y >> z;
            ks = {x, y, z};
        }
    }
    Material m(name, ns, ka, kd, ks);
    mats.push_back(m);
}
