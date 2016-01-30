#ifndef _EASYEDITOR_SCREEN_SHADER_H_
#define _EASYEDITOR_SCREEN_SHADER_H_

#include "IShader.h"

namespace ee
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

	virtual void Reset() {}

	void Draw(int texid) const;

private:
	

}; // ScreenShader

}

#endif // _EASYEDITOR_SCREEN_SHADER_H_