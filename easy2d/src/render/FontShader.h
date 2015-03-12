#ifndef _EASY2D_FONT_SHADER_H_
#define _EASY2D_FONT_SHADER_H_

#include "IShader.h"

namespace e2d
{

class FontShader : public IShader
{
public:
	FontShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind() {}
	virtual void Unbind() {}

	virtual void SetProjection(int width, int height) {}

	virtual void Commit() {}

protected:
	virtual void BindAttrib(GLuint prog) {}

}; // FontShader

}

#endif // _EASY2D_FONT_SHADER_H_