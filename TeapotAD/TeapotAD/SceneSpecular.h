#pragma once
#ifndef SCENESPECULAR_H
#define SCENESPECULAR_H

// Include Classes and Libaries
#include "gl_core_4_3.hpp"
#include <glfw3.h>
#include "scene.h"
#include "glslprogram.h"
#include "vboteapot.h"
#include "vboplane.h"
#include <glm.hpp>

// Define glm mat4
using glm::mat4;

namespace imat2908
{
    // Specular Class which Inherits from Scene
    class SceneSpecular : public Scene
    {
        // Private Member
    private:

        // prog of type GLSLProgram
        GLSLProgram prog;

        // Int variables called width and height
        int width, height;

        // Teapot and Plane of type VBO Plane
        VBOTeapot* teapot; 
        VBOPlane* plane;  

        mat4 model; //Model matrix

        // Set Matrices Function which takes in a Camera Parameter
        void setMatrices(QuatCamera camera); 

        // CompileandLinkShader Function
        void compileAndLinkShader(); 

    public:
        SceneSpecular(); //Constructor

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

#endif // SCENESPECULAR_H
