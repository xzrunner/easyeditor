#ifndef _GLUE_SPRITE_SHADER_H_
#define _GLUE_SPRITE_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"

namespace glue
{

struct Colorf;

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

	void SetColor(const Colorf& color, const Colorf& additive);
	void SetMapColor(const Colorf& rmap, const Colorf& gmap, const Colorf& bmap);

	void Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid);

}; // SpriteShader

}

#endif // _GLUE_SPRITE_SHADER_H_