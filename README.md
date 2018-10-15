# Raytracer

Compile using the makefile

To execute the program, type "./raytracer *.txt *.ppm" in the terminal, where *.txt is the driver file name and *.ppm is the name of the image to be written out.

For example, to execute the program with driver file driver00.txt and write to image file driver00.ppm type: ./raytracer driver00.txt driver00.ppm



Specifications for driver files:

The following must be included:

    #The location of the focal point (camera)
    eye (X) (Y) (Z)
 
    #The point at which the eye is facing
    look (X) (Y) (Z)
    
    #The direction defined as up 
    up (X) (Y) (Z)

    #The focal length (distance from the focal point to the image plane)
    d (length)

    # The minimum and maximum extend of the bounded image rectangle on the infinite image plane in the camera horizontal and vertical directions
    bounds (Left) (Bottom) (Right) (Top)

    #The pixel sampling resolution across the horizontal and vertical dimensions
    res (horizontal) (vertical)

    #The number of recursive reflections to include in the scene
    #If not included, it will default to 0
    recursionLevel (level)

    #The amount of low level illumination in the scene. All values must be between 0 and 1, with 0 being no light
    ambient (Red) (Green) (Blue)



Any number of lights, models, or spheres may optionally be included:

    #Defines a light to be included in the scene. (X, Y, Z, W) is the coordinates of the light, where W is usually 1, but set to 0 if the light is at infinity
    #Red, Green, and Blue values for the light are on a scale of 0 to 1
    light (X) (Y) (Z) (W) (Red) (Green) (Blue)

    #Defines a model to be included in the scene. You must also have a corresponding *.obj file and optionally can have a material (*.mtl) file.
    #(WX, WY, WZ) is the axis about which to rotate the model, (Theta) is the degrees to rotate the model
    #(Scale) is the uniform scaling factor to shrink or grow the model, (TX, TY, TZ) is the amount to translate the model (from 0,0,0)
    #(*.obj) is the object file containing the model
    model (WX) (WY) (WZ) (Theta) (Scale) (TX) (TY) (TZ) (*.obj)

    #Defines a sphere to be included in the scene. It is defined with the coordinates of the center, radius, and material properties.
    #KA are the ambient coefficients, KD are the diffuse coefficients, KS are the specular coefficients, KR are the attenuation coefficients, KO are the opacity coefficients, and eta is the index of refraction.
    sphere (X) (Y) (Z) (Radius) (KA Red) (KA Green) (KA Blue) (KD Red) (KD Green) (KD Blue) (KS Red) (KS Green) (KS Blue) (KR Red) (KR Green) (KR Blue) (KO Red) (KO Green) (KO Blue) (Eta)

    #Comments can also be included by starting the line with #
    
    

Example Driver File:

######################################################################################

eye 85 250 85
look 0 0 0
up 0 1 0
d 200
bounds -2 -2 2 2
res 512 512
recursionLevel 3
ambient 0.1 0.1 0.1
light 200 100 -200 1 0.5 0.5 0.5
light -200 100 200 1 0.5 0.5 0.5
model 1.0 0.0 0.0 90 1 0 0 0 checker_colored.obj
sphere 0 1 0 0.5 0.1 0.1 0.1 0.9 0.9 0.9 0.7 0.7 0.7 1 1 1 0.5 0.5 0.5 2.0
sphere 0 0.7 0 0.4 0.1 0.1 0.1 0.9 0.9 0.9 0.7 0.7 0.7 1 1 1 0.5 0.5 0.5 2.0

#######################################################################################
