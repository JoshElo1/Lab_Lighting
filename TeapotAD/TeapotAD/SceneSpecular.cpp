// Include these Classes and Libaries
#include "scenespecular.h"
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
	SceneSpecular::SceneSpecular()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Initialise the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::initScene(QuatCamera camera)
	{
		//Compile and link the shader  
		compileAndLinkShader();

		gl::Enable(gl::DEPTH_TEST);

		//Set up the lighting
		setLightParams(camera);
		
		// Create the plane to represent the ground
		plane = new VBOPlane(100.0, 100.0, 100, 100);

		//A matrix to move the teapot lid upwards
		glm::mat4 lid = glm::mat4(1.0);

		//Create the teapot with translated lid
		teapot = new VBOTeapot(16, lid);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Update not used at present
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::update(float t)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::setLightParams(QuatCamera camera)
	{
		// Vector 3 called worldLight with its position
		vec3 worldLight = vec3(15.0f, 15.0f, 10.0f); // Move the light upwards and to the right

		// Set GLSL Specular Lighting Properties, including World Light
		prog.setUniform("LightPosition", worldLight);

		// Set Specular Intensity
		prog.setUniform("Ls", 0.3f, 0.3f, 0.3f);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////
	// Render the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::render(QuatCamera camera)
	{
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		//First deal with the plane to represent the ground

		//Initialise the model matrix for the plane
		model = mat4(1.0f);

		//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
		setMatrices(camera);

		//Set the plane's material properties in the shader and render
		prog.setUniform("Ks", 0.1f, 0.1f, 0.1f); 

		// Render Plane
		plane->render();

		//Now set up the teapot 
		model = mat4(1.0f);

		setMatrices(camera);

		//Set the Teapot material properties in the shader and render
		prog.setUniform("Ks", 0.29, 0.29, 0.29); 

		// Render Teapot
		teapot->render(); 
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Send the MVP matrices to the GPU
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::setMatrices(QuatCamera camera)
	{
		// mv of type Mat4
		mat4 mv = camera.view() * model;

		prog.setUniform("ModelViewMatrix", mv);
		prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("MVP", camera.projection() * mv);
		mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
		prog.setUniform("M", model);
		prog.setUniform("V", camera.view());
		prog.setUniform("P", camera.projection());

		// Shininess and Attenuation of Material Structure
		struct Material 
		{
			// Float Variables
			float shininess;
			float attenuation;
		} 
		
		materialPlane = { 32.0f, 0.05f }, materialTeapot = { 64.0f, 0.1f };

		// Set Factors of the Shaders
		prog.setUniform("MaterialPlane.shininess", materialPlane.shininess);
		prog.setUniform("MaterialPlane.attenuation", materialPlane.attenuation);
		prog.setUniform("MaterialTeapot.shininess", materialTeapot.shininess);
		prog.setUniform("MaterialTeapot.attenuation", materialTeapot.attenuation);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// resize the viewport
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::resize(QuatCamera camera, int w, int h)
	{
		// Gl ViewPort width and height set
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;
		camera.setAspectRatio((float)w / h);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Compile and link the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneSpecular::compileAndLinkShader()
	{
		// Compile Vertex and Fragment Shaders
		try 
		{
			prog.compileShader("Shaders/phong.vert");
			prog.compileShader("Shaders/phong.frag");
			prog.link();
			prog.validate();
			prog.use();
		}
		catch (GLSLProgramException& e) {
			cerr << e.what() << endl;
			exit(EXIT_FAILURE);
		}
	}

}
