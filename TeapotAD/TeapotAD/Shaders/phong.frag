#version 430

// Vector 3 Variables
in vec3 vertPos;
in vec3 N;
in vec3 lightPos;

// Ambient reflectivity
uniform vec3 Ka;         
// Diffuse reflectivity
uniform vec3 Kd;      
// Specular reflectivity
uniform vec3 Ks;    
// Ambient light intensity
uniform vec3 La;       
// Diffuse light intensity
uniform vec3 Ld;       
// Specular light intensity
uniform vec3 Ls;           

// Shininess Float
uniform float Shininess; 

layout( location = 0 ) out vec4 FragColour;

void main() {

   // Calculate the light vector
   vec3 L = normalize(lightPos - vertPos);
    
   // Calculate Diffuse Light Intensity 
   vec4 Id = vec4(Ld,1.0) * max(dot(N,L), 0.0);

   //calculate Ambient Light Intensity
   vec4 Ia = vec4(La, 1.0) * vec4(Ka, 1.0);

   // Calculate Specular Light Intensity
   vec3 R = reflect(-L, N);
   vec4 Is = vec4(Ls,1.0) * pow(max(dot(R, normalize(-vertPos)), 0.0), Shininess);

   // Add all the light components together
   vec4 colour = vec4(Kd,1.0) * Id + Ia + Is;
   FragColour = clamp(colour, 0.0, 1.0);
}
