#include<iostream>

#include"Ray.h"

using std::cout;
using std::endl;
using std::ostream;

ostream& operator<< (ostream& os, const Ray& r){
    os << "Pixpt: " << r.pixpt[0] << ", " << r.pixpt[1] << ", " << r.pixpt[2] << endl;
    os << "Pixray: " << r.pixray[0] << ", " << r.pixray[1] << ", " << r.pixray[2] << endl;
    return os;
}

