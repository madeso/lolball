#include "ParticleComponent3.hpp"

#include "OpenGL.hpp"
#include "Camera3.hpp"
#include "World3.hpp"


#include "sdl.h"
#include "SDL_opengl.h"

void renderBillboard(const vec3& pUp, const vec3& pRight, const World3& pWorld, const vec3& pPosition, const vec3& pColor, real pAlpha, real pSize) {
	const vec3 up = pUp * pSize;
	const vec3 right = pRight * pSize;
	const vec3 top = pPosition + up;
	const vec3 bottom = pPosition -up;

	glColor4f(pColor.getX(), pColor.getY(), pColor.getZ(), pAlpha);
	glTexCoord2f(1,1); glVertex(top + right);
	glTexCoord2f(0,1); glVertex(top - right);
	glTexCoord2f(0,0); glVertex(bottom - right);
	glTexCoord2f(1,0); glVertex(bottom + right);
}

void setupRenderingForParticles() {
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBegin(GL_QUADS);
}
void restoreRenderingForParticles() {
	glEnd();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_LIGHTING);
}