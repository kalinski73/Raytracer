OBJS := main.o Model.o  Matrix.o Camera.o Sphere.o Ray.o Face.o Image.o
CXX := g++
CPPFLAGS := -std=c++11 -g -Wall

all: raytracer

raytracer: $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o raytracer

main.o: main.cpp Image.o
	$(CXX) $(CPPFLAGS) -c main.cpp

Model.o: Model.cpp Model.h Object.h Face.o Matrix.o Material.h
	$(CXX) $(CPPFLAGS) -c Model.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CXX) $(CPPFLAGS) -c Matrix.cpp

Camera.o: Camera.cpp Camera.h Ray.o
	$(CXX) $(CPPFLAGS) -c Camera.cpp

Sphere.o: Sphere.cpp Sphere.h Object.h
	$(CXX) $(CPPFLAGS) -c Sphere.cpp

Ray.o: Ray.cpp Ray.h
	$(CXX) $(CPPFLAGS) -c Ray.cpp

Face.o: Face.cpp Face.h
	$(CXX) $(CPPFLAGS) -c Face.cpp

Image.o: Image.cpp Image.h Camera.o Object.h Model.o Sphere.o Light.h
	$(CXX) $(CPPFLAGS) -c Image.cpp

clean:
	rm -rf *.o raytracer

package:
	tar -cvf Kalinski_Raytracer.tar main.cpp Model.h Model.cpp Matrix.h Matrix.cpp Camera.h Camera.cpp Sphere.h Sphere.cpp Ray.h Ray.cpp Face.h Object.h Image.h Image.cpp Light.h Material.h README.* Makefile
