#ifndef _DRAG2D_SCREEN_SHADER_H_
#define _DRAG2D_SCREEN_SHADER_H_

#include "IShader.h"

namespace d2d
{

class ScreenShader : public IShader
{
public:
	ScreenShader();
	virtual ~ScreenShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	void Draw(int texid) const;

protected:
	virtual void BindAttrib(GLuint prog) {}

private:
	

}; // ScreenShader

}

#endif // _DRAG2D_SCREEN_SHADER_H_