#ifndef _GLUE_FILTER_SHADER_H_
#define _GLUE_FILTER_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"

#include <stdint.h>

namespace glue
{

class FilterShader : public IShader
{
public:
	FilterShader();
	virtual ~FilterShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void OnSize(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetProjection(int width, int height);
	void SetModelView(const vec2& offset, float scale);

	void Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid);

}; // FilterShader

}

#endif // _GLUE_FILTER_SHADER_H_