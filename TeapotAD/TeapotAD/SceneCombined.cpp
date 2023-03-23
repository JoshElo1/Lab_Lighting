// Include these Classes and Libaries
#include "scenecombined.h"
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
	SceneCombined::SceneCombined()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Initialise the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::initScene(QuatCamera camera)
	{
		// Compile and link the shader
		compileAndLinkShader();

		gl::Enable(gl::DEPTH_TEST);

		// Set up the lighting
		setLightParams(camera);

		// Create the plane to represent the ground
		plane = new VBOPlane(100.0, 100.0, 100, 100);

		// Set the plane's material
		// Ambient Light Reflectivity
		prog.setUniform("Ka", 0.51f, 1.0f, 0.49f); 

		// Shininess
		prog.setUniform("Shininess", 32.0f);

		// Set the teapot's material 
		// Ambient Light Reflectivity
		prog.setUniform("Ka", 0.46f, 0.29f, 0.0f);

		// Shininess 
		prog.setUniform("Shininess", 64.0f);

		// Create the teapot
		model = mat4(1.0f);
		setMatrices(camera);
		teapot = new VBOTeapot(16, model);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Update not used at present
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::update(float t)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::setLightParams(QuatCamera camera)
	{
		vec3 worldLight = vec3(15.0f, 15.0f, 10.0f);

		// Set GLSL Specular Lighting Properties, including World Light
		prog.setUniform("LightPosition", worldLight);

		// Set Lighting Intensities
		prog.setUniform("La", 0.3f, 0.3f, 0.3f);
		prog.setUniform("Ld", 0.7f, 0.7f, 0.7f);
		prog.setUniform("Ls", 0.3f, 0.3f, 0.3f);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Render the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::render(QuatCamera camera)
	{
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		// Render the plane
		model = mat4(1.0f);
		setMatrices(camera);

		// Set Reflectivities
		prog.setUniform("Ka", 0.51f, 1.0f, 0.49f); 
		prog.setUniform("Kd", 0.5f, 0.5f, 0.5f); 
		prog.setUniform("Ks", 0.1f, 0.1f, 0.1f); 

		// Render Plane
		plane->render();

		//Set the Teapot material properties in the shader and render
		model = mat4(1.0f);
		setMatrices(camera);

		// Set Reflectivities
		prog.setUniform("Ka", 0.46f, 0.29f, 0.0f);
		prog.setUniform("Kd", 0.5f, 0.5f, 0.5f); 
		prog.setUniform("Ks", 0.29f, 0.29f, 0.29f); 

		// Render Teapot
		teapot->render();
	}



	/////////////////////////////////////////////////////////////////////////////////////////////
	//Send the MVP matrices to the GPU
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::setMatrices(QuatCamera camera)
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

		// Set Shininess and Attenuation
		prog.setUniform("MaterialPlane.shininess", 32.0f);
		prog.setUniform("MaterialPlane.attenuation", 0.05f);
		prog.setUniform("MaterialTeapot.shininess", 64.0f);
		prog.setUniform("MaterialTeapot.attenuation", 0.1f);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////
	// resize the viewport
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneCombined::resize(QuatCamera camera, int w, int h)
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
	void SceneCombined::compileAndLinkShader()
	{
		// Compile Vertex and Fragment Shaders
		try {
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
