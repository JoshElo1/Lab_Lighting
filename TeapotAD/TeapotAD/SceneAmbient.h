#ifndef SCENEAMBIENT_H
#define SCENEAMBIENT_H

// Include Classes and Libaries
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
    // Scene Ambient Class which Inherits from Scene
    class SceneAmbient : public Scene
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

        // Model Matrix
        mat4 model;

        // Set Matrices Function which takes in a Camera Parameter
        void setMatrices(QuatCamera camera);

        // CompileandLinkShader Function
        void compileAndLinkShader();

        // Public Member
    public:
        SceneAmbient(); //Constructor

        void setLightParams(QuatCamera camera); //Setup the lighting

        void initScene(QuatCamera camera); //Initialise the scene

        void update(float t); //Update the scene

        void render(QuatCamera camera);	//Render the scene

        void resize(QuatCamera camera, int, int); //Resize
    };

}

#endif // SCENEAMBIENT_H
