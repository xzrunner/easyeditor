#ifndef _GLUE_SPRITE_SHADER_H_
#define _GLUE_SPRITE_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"

#include <stdint.h>

namespace glue
{

class SpriteShader : public IShader
{
public:
	SpriteShader();
	virtual ~SpriteShader();

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
	void SetMapColor(uint32_t rmap, uint32_t gmap, uint32_t bmap);

	void Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid);

}; // SpriteShader

}

#endif // _GLUE_SPRITE_SHADER_H_