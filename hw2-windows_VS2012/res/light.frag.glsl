# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

in vec4 color;
in vec3 mynormal; 
in vec4 myvertex; 

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec3 lightCalculation(int i)
{
	vec3 N = normalize(mynormal);
	vec3 L = normalize(vec3(lightposn[i])-vec3(myvertex));
	vec3 V = normalize(vec3(0.0)-vec3(myvertex));
	if(lightposn[i].a==0)
		L = normalize(vec3(lightposn[i]));
	else
		L = normalize(lightposn[i].xyz/lightposn[i].w-vec3(myvertex));
	vec3 H = normalize(L+V);
	vec3 diff = vec3(diffuse);
	vec3 spec = vec3(specular);
	diff = max(dot(N,L),0)*diff;
	spec = pow(max(dot(N,H),0),shininess)*spec;

	return vec3(lightcolor[i])*(spec+diff);
}

vec3 test(int i)
{
	vec3 N = normalize(mynormal);
	vec3 L = normalize(vec3(lightposn[i])-vec3(myvertex));
	vec3 V = normalize(vec3(0.0)-vec3(myvertex));
	if(lightposn[i].a==0)
		L = normalize(vec3(lightposn[i]));
	vec3 H = normalize(L+V);
	vec3 diff = vec3(diffuse);
	vec3 spec = vec3(specular);
	diff = max(dot(N,L),0)*diff;
	spec = pow(max(dot(N,H),0),shininess)*spec;

	return vec3(lightcolor[i])*(spec+diff);
}

void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor=vec4(0,0,0,1); 
		finalcolor =finalcolor + vec4(ambient);
		finalcolor =finalcolor + vec4(emission);
		for(int i=0;i<numused;i++)
		{
			finalcolor+=vec4(lightCalculation(i),0);
		}
        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        // Color all pixels black for now, remove this in your implementation!

        gl_FragColor = finalcolor; 
    } else {
        gl_FragColor = color; 
    }
}
