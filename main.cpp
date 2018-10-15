#include<fstream>
#include<iostream>

#include"Image.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


//takes two command line arguments: driver file name, name of output image
int main(int argc, char* argv[]){
    
    //check for the correct number of arguments
    //the first argument is a driver file containing the camera description and a list of models along with their model-to-world transformations
    //The second argument is the name of the image your program will write back to the disk in the same folder where your executable is located
    if (argc !=3){
        cerr << "Incorrect number of parameters" << endl;
        cerr << "Usage:" << endl;
        cerr <<"  ./raytracer <driver_file> <output_image>" << endl;
        return -1;
    }
    
    //creating the object of type istream called istr
    ifstream istr(argv[1]);
    if(istr.fail()){
        cerr << "istream failed" << endl;
        return-1;
    }    
    
    //using methods in image class
    //instantiate camera, models, spheres and lights using istr
    Image I(istr);
    istr.close();
    
    I.createImage();
    I.colorNorm();
    
    //destruct models and spheres
    I.cleanUp();
    
    //write out to image file named argv[2]
    ofstream ostr(argv[2]);
    I.outppm(ostr);
    ostr.close();
    
    return 0;
}
