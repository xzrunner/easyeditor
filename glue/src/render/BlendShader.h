#ifndef _GLUE_BLEND_SHADER_H_
#define _GLUE_BLEND_SHADER_H_

#include "IShader.h"
#include "BlendMode.h"

#include "math/Vector.h"

#include <stdint.h>

namespace glue
{

class BlendShader : public IShader
{
public:
	BlendShader();
	virtual ~BlendShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void OnSize(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetProjection(int width, int height);
	void SetModelView(const vec2& offset, float scale);

	void SetColor(uint32_t color, uint32_t additive);
	void SetMode(BlendMode mode);

	void Draw(const vec2 vertices[4], const vec2 texcoords[4], 
		const vec2 texcoords_base[4], int tex_blend, int tex_base);

}; // BlendShader

}

#endif // _GLUE_SPRITE_SHADER_H_