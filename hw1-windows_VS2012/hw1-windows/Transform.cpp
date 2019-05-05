// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
	vec3 _up = glm::normalize(axis);
	float x = _up.x;
	float y = _up.y;
	float z = _up.z;

	glm::mat3 m1(1, 0, 0, 0, 1, 0, 0, 0, 1);
	m1 = glm::transpose(m1);
	glm::mat3 m2(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
	m2 = glm::transpose(m2);
	glm::mat3 m3(0, -z, y, z, 0, -x, -y, x, 0);
	m3 = glm::transpose(m3);
	glm::mat3 m4 = cos(degrees*pi / 180.0)*m1 + (1 - cos(degrees*pi / 180.0))*m2 + sin(degrees*pi / 180.0)*m3;
	// You will change this return call
	return m4;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
	// YOUR CODE FOR HW2 HERE
	// Likely the same as in HW 1.  
	eye = rotate(degrees, up)*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
	// YOUR CODE FOR HW2 HERE 
	// Likely the same as in HW 1.  
	vec3 _w = glm::normalize(eye);
	vec3 _u = glm::normalize(glm::cross(up, _w));
	vec3 _v = glm::cross(_w, _u);
	eye = rotate(-degrees, _u) *eye;
	up = glm::cross(eye, _u);
}

mat4 Transform::lookAt(vec3 eye, vec3 up)
{
	vec3 _w = glm::normalize(eye - vec3(0, 0, 0));
	vec3 _u = glm::normalize(glm::cross(up, _w));
	vec3 _v = glm::normalize(glm::cross(_w, _u));
	mat4 l(_u.x, _u.y, _u.z, 0, _v.x, _v.y, _v.z, 0, _w.x, _w.y, _w.z, 0, 0, 0, 0, 1);
	l = glm::transpose(l);
	mat4 r(1, 0, 0, 0 - eye.x, 0, 1, 0, 0 - eye.y, 0, 0, 1, 0 - eye.z, 0, 0, 0, 1);
	r = glm::transpose(r);
	mat4 fin = l * r;
	// You will change this return call
	return fin;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	
	float d = glm::atan(2.0f / fovy);

	mat4 ret(d / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, d, 0.0f, 0.0f,
		0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -2.0f*zNear*zFar / (zFar - zNear),
		0.0f, 0.0f, -1.0f, 0.0f);
	ret = glm::transpose(ret);
	// YOUR CODE FOR HW2 HERE
	// New, to implement the perspective transform as well.  
	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	mat4 ret(sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ret = glm::transpose(ret);
	// YOUR CODE FOR HW2 HERE
	// Implement scaling 
	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	mat4 ret(0.0f, 0.0f, 0.0f, tx,
		0.0f, 0.0f, 0.0f, ty,
		0.0f, 0.0f, 0.0f, tz,
		0.0f, 0.0f, 0.0f, 1.0f);
	ret = glm::transpose(ret);
	// YOUR CODE FOR HW2 HERE
	// Implement translation 
	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
