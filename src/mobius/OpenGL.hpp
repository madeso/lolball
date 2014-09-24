#ifndef OPENGL_HPP
#define OPENHL_HPP

void setDisplay2d();
void setDisplay3d();

#define checkOpengl() doCheckOpengl()
void doCheckOpengl();

#ifdef POSITION3_HPP
	void glTranslate(const Position3& pPosition);
	void glTranslateInverse(const Position3& pPosition);
#endif
#ifdef ROTATION_HPP
	void glRotate(const Rotation& pRotation);
	void glRotateInverse(const Rotation& pRotation);
#endif

#ifdef VEC2_HPP
	void glVertex(const vec2& pVertex);
#endif

#ifdef VEC3_HPP
	void glVertex(const vec3& pVertex);
#endif

#endif