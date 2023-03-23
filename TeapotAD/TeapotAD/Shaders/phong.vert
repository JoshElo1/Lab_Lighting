#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 vertPos; //Vertex position in eye coords
out vec3 N; //Transformed normal
out vec3 lightPos; //Light position in eye coords

// Light position 
uniform vec3 LightPosition; 

// Matrix called NormalMatrix
uniform mat3 NormalMatrix;

// Matrix called M, V and P
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

// Main Function
void main()
{

   // vertPos assigned to the vector of the calculation
   vertPos = vec3(V * M * vec4(VertexPosition,1.0)); 
     
   // lightPos assigned to the vector of the calculation
   lightPos = vec3(V * M * vec4(LightPosition,1.0));  

   N = normalize( NormalMatrix * VertexNormal);

   // gl_Position assigned to a vector4 calculation
   gl_Position = gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
