#include "Position3.hpp"
#include "Rotation.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include "OpenGL.hpp"

#include "DataPointer.hpp"
#include "Resolution.hpp"
#include "math.hpp"

#include <iostream>
#include <string>

#include "sdl.h"
#include "SDL_opengl.h"

void doCheckOpengl() {
	GLenum error = glGetError();
	if( error != GL_NO_ERROR ) {
		std::string value ="???";
#define ERROR_VALUE(x) case x: value = #x; break
		switch(error) {
			ERROR_VALUE(GL_INVALID_ENUM);
			ERROR_VALUE(GL_INVALID_VALUE);
			ERROR_VALUE(GL_INVALID_OPERATION);
			ERROR_VALUE(GL_STACK_OVERFLOW);
			ERROR_VALUE(GL_STACK_UNDERFLOW);
			ERROR_VALUE(GL_OUT_OF_MEMORY);
		}
#undef ERROR_VALUE
		std::cerr << "Opengl error: " << error << value << std::endl;
	}
}

void setDisplay2d() {
	// setup 2d
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void setDisplay3d() {
	DataPointer<Resolution> resolution("resolution");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// http://www.sjbaker.org/steve/omniv/love_your_z_buffer.html
	gluPerspective(45.0f, float(resolution.get().width)/float(resolution.get().height), 0.5f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void glTranslate(const Position3& pPosition) {
	glTranslatef(pPosition.getCurrentPosition().getX(), pPosition.getCurrentPosition().getY(), pPosition.getCurrentPosition().getZ());
}
void glTranslateInverse(const Position3& pPosition) {
	glTranslatef(-pPosition.getCurrentPosition().getX(), -pPosition.getCurrentPosition().getY(), -pPosition.getCurrentPosition().getZ());
}
void glRotate(const Rotation& pRotation) {
	vec3 axis(0,0,0);
	float angle;
	pRotation.getCurrentRotation().toAxisAngle(&axis, &angle);
	glRotatef( math::radToDeg(angle), axis.getX(), axis.getY(), axis.getZ());
}
void glRotateInverse(const Rotation& pRotation) {
	vec3 axis(0,0,0);
	float angle;
	pRotation.getCurrentRotation().getConjugate().toAxisAngle(&axis, &angle);
	glRotatef( math::radToDeg(angle), axis.getX(), axis.getY(), axis.getZ());
}

void glVertex(const vec2& pVertex) {
	glVertex2f(pVertex.getX(), pVertex.getY());
}
void glVertex(const vec3& pVertex) {
	glVertex3f(pVertex.getX(), pVertex.getY(), pVertex.getZ());
}