#ifndef _EASYEDITOR_SHAPE3_SHADER_H_
#define _EASYEDITOR_SHAPE3_SHADER_H_

#include "IShader.h"

namespace ee
{

class Colorf;

class Shape3Shader : public IShader
{
public:
	Shape3Shader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset() {}

	void SetModelView();

	void SetColor(const Colorf& col);

protected:
	virtual void LoadShader() {}

}; // Shape3Shader

}

#endif // _EASYEDITOR_SHAPE3_SHADER_H_