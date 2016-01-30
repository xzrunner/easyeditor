#include "GL10.h"
#include "Shader.h"

#define NOMINMAX
#include <gl/glew.h>

namespace ee
{

void GL10::Begin(int mode)
{
	glBegin(mode);
}

void GL10::BindTexture(int target, unsigned int texture)
{
	glBindTexture(target, texture);
}

void GL10::BlendFunc(int sfactor, int dfactor)
{
	// set in shader
//	glBlendFunc(sfactor, dfactor);
}

void GL10::Color3f(float red, float green, float blue)
{
//	glColor3f(red, green, blue);
	Shader::Instance()->color(red, green, blue, 1.0f);
}

void GL10::Color4f(float red, float green, float blue, float alpha)
{
//	glColor4f(red, green, blue, alpha);
	Shader::Instance()->color(red, green, blue, alpha);
}

void GL10::ColorPointer(int size, int type, int stride, const void* pointer)
{
	glColorPointer(size, type, stride, pointer);
}

void GL10::Disable(int cap)
{
	glDisable(cap);
}

void GL10::DisableClientState(int array)
{
	glDisableClientState(array);
}

void GL10::DrawArrays(int mode, int first, int count)
{
	glDrawArrays(mode, first, count);
}

void GL10::Enable(int cap)
{
	glEnable(cap);
}

void GL10::EnableClientState(int array)
{
	glEnableClientState(array);
}

void GL10::End()
{
	glEnd();
}

void GL10::GetTexLevelParameteriv(int target, int level, int pname, int* params)
{
	glGetTexLevelParameteriv(target, level, pname, params);
}

void GL10::LineStipple(int factor, unsigned short pattern)
{
	glLineStipple(factor, pattern);
}

void GL10::LineWidth(float width)
{
	glLineWidth(width);
}

void GL10::PointSize(float size)
{
	glPointSize(size);
}

void GL10::PopMatrix()
{
	glPopMatrix();
}

void GL10::PushMatrix()
{
	glPushMatrix();
}

void GL10::Rotatef(float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}

void GL10::Scalef(float x, float y, float z)
{
	glScalef(x, y, z);
}

void GL10::MultMatrixf(const float* m)
{
	glMultMatrixf(m);
}

void GL10::TexCoord2f(float s, float t)
{
	glTexCoord2f(s, t);
}

void GL10::TexCoordPointer(int size, int type, int stride, const void* pointer)
{
	glTexCoordPointer(size, type, stride, pointer);
}

void GL10::Translatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void GL10::Vertex2f(float x, float y)
{
	glVertex2f(x, y);
}

void GL10::Vertex3f(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

void GL10::VertexPointer(int size, int type, int stride, const void* pointer)
{
	glVertexPointer(size, type, stride, pointer);
}

void GL10::GetTexImage(int target, int level, int format, int type, void* img)
{
	glGetTexImage(target, level, format, type, img);
}
}