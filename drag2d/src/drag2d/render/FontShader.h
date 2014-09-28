#ifndef _DRAG2D_FONT_SHADER_H_
#define _DRAG2D_FONT_SHADER_H_

#include "IShader.h"

namespace d2d
{

class FontShader : public IShader
{
public:
	FontShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind() {}
	virtual void Unbind() {}

	virtual void SetModelView(const Vector& offset, float scale) {}
	virtual void SetProjection(int width, int height) {}

	virtual void Commit() {}

protected:
	virtual void BindAttrib(GLuint prog) {}

}; // FontShader

}

#endif // _DRAG2D_FONT_SHADER_H_