#ifndef _DRAG2D_ISHADER_H_
#define _DRAG2D_ISHADER_H_

typedef unsigned int GLuint;

#include "common/Vector.h"
#include "sketch/Quaternion.h"

namespace d2d
{
	
class IShader
{
public:
	IShader();
	virtual ~IShader() {}
	
	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void SetModelView(const Vector& offset, float scale) = 0;
	virtual void SetModelView(const Quaternion& ori) = 0;
	virtual void SetProjection(int width, int height) = 0;

	virtual void Commit() = 0;

	GLuint GetProgram() const { return m_prog; }

protected:
	virtual void BindAttrib(GLuint prog) = 0;

	void InitShader(const char* FS, const char* VS);

private:
	static GLuint CompileShader(const char* source, GLuint type);

protected:
	GLuint m_prog; 

#if 0
#define FLOAT_PRECISION \
	"#ifdef GL_FRAGMENT_PRECISION_HIGH  \n" \
	"precision highp float;  \n" \
	"#else  \n" \
	"precision lowp float;  \n" \
	"#endif  \n"
#else
#define FLOAT_PRECISION \
	"precision lowp float;  \n"
#endif
}; // IShader

}

#endif // _DRAG2D_ISHADER_H_