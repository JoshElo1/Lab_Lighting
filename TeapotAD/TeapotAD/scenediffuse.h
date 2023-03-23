#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

// Include Classes and Libaries.
#include "gl_core_4_3.hpp"
#include <glfw3.h>
#include "scene.h"
#include "glslprogram.h"
#include "vboteapot.h"
#include "vboplane.h"
#include <glm.hpp>

// Define glm matrix
using glm::mat4;

namespace imat2908
{
    // Scene Diffuse Class which Inherits from Scene
class SceneDiffuse : public Scene
{
    // Private Member
private:

    // prog of type GLSLProgram
    GLSLProgram prog;

    // Int variables called width and height
    int width, height;
 
    // Teapot and Plane of type VBO Plane
	VBOTeapot *teapot; 
	VBOPlane *plane; 

    // Model Matrix
    mat4 model;

    // Set Matrices Function which takes in a Camera Parameter
    void setMatrices(QuatCamera camera); 

    // CompileandLinkShader Function
    void compileAndLinkShader(); 

    // Public Member
public:

    // Constructor
    SceneDiffuse(); 

    // SetLightParams Function that takes in Camera Parameter
	void setLightParams(QuatCamera camera);

    // InitScene Function that takes in a camera parameter
    void initScene(QuatCamera camera); 

    // Update Function that takes in a float
    void update(float t); 

    // Render Function that takes in a camera parameter
    void render(QuatCamera camera);	

    // Resize Function that takes in a camera and two integer parameters
    void resize(QuatCamera camera, int, int); 
};

}

#endif // SCENEDIFFUSE_H
