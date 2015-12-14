#ifndef _DRAG2D_RVG_SHADER_H_
#define _DRAG2D_RVG_SHADER_H_

#include "IShader.h"

#include "common/Color.h"

namespace d2d
{

class RVGShader : public IShader
{
public:
	RVGShader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset() {}

	void SetModelView(const Vector& offset, float scale);

	void SetColor(const Colorf& col);

protected:
	virtual void LoadShader();

}; // RVGShader

}

#endif // _DRAG2D_RVG_SHADER_H_