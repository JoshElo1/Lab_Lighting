// Include these Classes and Libaries
#include "scenediffuse.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::endl;

#include "defines.h"

// Defind glm::vec3
using glm::vec3;

// Include these Files
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


namespace imat2908
{
/////////////////////////////////////////////////////////////////////////////////////////////
// Default constructor
/////////////////////////////////////////////////////////////////////////////////////////////
SceneDiffuse::SceneDiffuse()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Initialise the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::initScene(QuatCamera camera)
{
    //Compile and link the shader  
	compileAndLinkShader();

    gl::Enable(gl::DEPTH_TEST);

 	// Set up the lighting
	setLightParams(camera);

	// Create the plane to represent the ground
	plane = new VBOPlane(100.0,100.0,100,100);

	//A matrix to move the teapot lid upwards
	glm::mat4 lid = glm::mat4(1.0);

	//Create the teapot with translated lid
	teapot = new VBOTeapot(16,lid);


}

/////////////////////////////////////////////////////////////////////////////////////////////
//Update not used at present
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::update( float t )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Set up the lighting variables in the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setLightParams(QuatCamera camera)
{
	// Vector 3 called worldLight with it's position
	vec3 worldLight = vec3(10.0f,10.0f,10.0f);
   
	// Set GLSL Specular Lighting Properties, including World Light
	// Set Diffuse Light Intensity and Reflectivity
	prog.setUniform("Ld", 0.9f, 0.9f, 0.9f);
	prog.setUniform("LightPosition", worldLight );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Render the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::render(QuatCamera camera)
{
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Initialise the model matrix for the plane
	model = mat4(1.0f);

	//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
    setMatrices(camera);

	//Set the plane's material properties in the shader and render
	prog.setUniform("Kd", 0.51f, 1.0f, 0.49f); 

	// Render Plane
	plane->render();

	//Now set up the teapot 
	 model = mat4(1.0f);

	 setMatrices(camera);

	 //Set the Teapot material properties in the shader and render
	 prog.setUniform("Kd", 0.46f, 0.29f, 0.0f);

	 // Render Teapot
	 teapot->render();
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Send the MVP matrices to the GPU
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setMatrices(QuatCamera camera)
{
	// mv of type Mat4
    mat4 mv = camera.view() * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", camera.projection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));// What does this line do?
	prog.setUniform("M", model);
	prog.setUniform("V", camera.view() );
	prog.setUniform("P", camera.projection() );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// resize the viewport
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::resize(QuatCamera camera,int w, int h)
{
	// Gl ViewPort width and height set
    gl::Viewport(0,0,w,h);
    width = w;
    height = h;
	camera.setAspectRatio((float)w/h);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Compile and link the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::compileAndLinkShader()
{
   // Compile Vertex and Fragment Shaders
	try {
    	prog.compileShader("Shaders/phong.vert");
    	prog.compileShader("Shaders/phong.frag");
    	prog.link();
    	prog.validate();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

}
