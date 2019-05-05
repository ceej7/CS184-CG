/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

using namespace std; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  
void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
  GLfloat modelview[16]; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview); 

  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0; 
    for (int j = 0 ; j < 4 ; j++) {
      output[i] += modelview[4*j+i] * input[j]; 
    }
  }
}

void display() 
{
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// I'm including the basic matrix setup for model view to 
	// give some sense of how this works.  

	glMatrixMode(GL_MODELVIEW);
	mat4 mv; 

	// Either use the built-in lookAt function or the lookAt implemented by the user.
	if (useGlu) {
		mv = glm::lookAt(eye,center,up); 
	} else {
		mv = Transform::lookAt(eye,center,up); 
	}

	glLoadMatrixf(&mv[0][0]); 

	// Lights are transformed by current modelview matrix. 
	// The shader can't do this globally. 
	// So we need to do so manually.  
	if (numused) {
		glUniform1i(enablelighting,true);

		// YOUR CODE FOR HW 2 HERE.  
		// You need to pass the light positions and colors to the shader. 
		// glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
		// The lightransf[] array in variables.h and transformvec() might also be useful here.
		// Remember that light positions must be transformed by modelview.  
		string sNumused = "numused";
		int handleNumused = glGetUniformLocation(shaderprogram, sNumused.c_str());
		glUniform1i(handleNumused, numused);
		for (int i = 0; i < 10; i++)
		{
			int offset = 4 * i;
			vec4 tmp;
			for (int i = 0; i < 4; i++) {
				tmp[i] = lightposn[offset + i];
			}
			tmp = mv * tmp;
			for (int i = 0; i < 4; i++) {
				lightransf[offset + i] = tmp[i];
			}
			string pos = "lightposn["+std::to_string(i) + "]";
			int handlePos = glGetUniformLocation(shaderprogram, pos.c_str() );
			glUniform4f(handlePos, lightransf[4 * i + 0], lightransf[4 * i + 1], lightransf[4 * i + 2], lightransf[4 * i + 3]);
			string col = "lightcolor[" + std::to_string(i) + "]";
			int handleCol = glGetUniformLocation(shaderprogram, col.c_str());
			glUniform4f(handleCol, lightcolor[4 * i + 0], lightcolor[4 * i + 1], lightcolor[4 * i + 2], lightcolor[4 * i + 3]);

		}
	} else {
		glUniform1i(enablelighting,false); 
	}

	// Transformations for objects, involving translation and scaling 
	mat4 sc(1.0) , tr(1.0), transf(1.0); 
	sc = Transform::scale(sx,sy,1.0); 
	tr = Transform::translate(tx,ty,0.0); 

	// YOUR CODE FOR HW 2 HERE.  
	// You need to use scale, translate and modelview to 
	// set up the net transformation matrix for the objects.  
	// Account for GLM issues, matrix order (!!), etc.  
	transf = mv  * tr* sc;
	glLoadMatrixf(&transf[0][0]); 

	for (int i = 0 ; i < numobjects ; i++) {
		object* obj = &(objects[i]); // Grabs an object struct.

		// YOUR CODE FOR HW 2 HERE. 
		// Set up the object transformations 
		// And pass in the appropriate material properties
		// Again glUniform() related functions will be useful
		glPushMatrix();
		glMultMatrixf(&obj->transform[0][0]);

		string shandle1 = "ambient";
		int handle1 = glGetUniformLocation(shaderprogram, shandle1.c_str());
		glUniform4f(handle1, obj->ambient[0], obj->ambient[1], obj->ambient[2], obj->ambient[3]);

		string shandle2 = "diffuse";
		int handle2 = glGetUniformLocation(shaderprogram, shandle2.c_str());
		glUniform4f(handle2, obj->diffuse[0], obj->diffuse[1], obj->diffuse[2], obj->diffuse[3]);

		string shandle3 = "specular";
		int handle3 = glGetUniformLocation(shaderprogram, shandle3.c_str());
		glUniform4f(handle3, obj->specular[0], obj->specular[1], obj->specular[2], obj->specular[3]);

		string shandle4 = "emission";
		int handle4 = glGetUniformLocation(shaderprogram, shandle4.c_str());
		glUniform4f(handle4, obj->emission[0], obj->emission[1], obj->emission[2], obj->emission[3]);

		string shandle5 = "shininess";
		int handle5 = glGetUniformLocation(shaderprogram, shandle5.c_str());
		glUniform1f(handle5, obj->shininess);

		// Actually draw the object
		// We provide the actual glut drawing functions for you.  
		// Remember that obj->type is notation for accessing struct fields
		if (obj->type == cube) {
			glutSolidCube(obj->size); 
		}
		else if (obj->type == sphere) {
			const int tessel = 20; 
			glutSolidSphere(obj->size, tessel, tessel); 
		}
		else if (obj->type == teapot) {
			glutSolidTeapot(obj->size); 
		}
		glPopMatrix();
	}

	glutSwapBuffers();
}
