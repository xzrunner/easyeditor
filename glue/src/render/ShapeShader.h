#ifndef _GLUE_SHAPE_SHADER_H_
#define _GLUE_SHAPE_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"

namespace glue
{

struct Colorf;

class ShapeShader : public IShader
{
public:
	ShapeShader();
	virtual ~ShapeShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void OnSize(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetProjection(int width, int height);
	void SetModelView(const vec2& offset, float scale);

}; // ShapeShader

}

#endif // _GLUE_SHAPE_SHADER_H_