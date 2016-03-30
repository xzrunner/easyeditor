#ifndef _EASYEDITOR_SPRITE_RENDERER_H_
#define _EASYEDITOR_SPRITE_RENDERER_H_

#include "RenderColor.h"
#include "Matrix.h"
#include "Vector.h"
#include "RenderParams.h"

namespace ee
{

class Sprite;
class Symbol;
class SpriteBatch;
class Vector;
class Matrix;
class FBO;

class SpriteRenderer
{
public:
	void Draw(const Sprite* sprite, 
		      const Sprite* root = NULL,
			  const RenderParams& trans = RenderParams(),
			  bool set_shader = true) const;

	void InvalidRect(const Sprite* sprite, const Matrix& mt = Matrix());

	void Draw(const Symbol* symbol, 
			  const RenderParams& trans = RenderParams(),
			  const Vector& pos = Vector(0, 0),
			  float angle = 0.0f, 
			  float xScale = 1.0f, 
			  float yScale = 1.0f,
			  float xShear = 0.0f, 
			  float yShear = 0.0f) const;

	static SpriteRenderer* Instance();

private:
	SpriteRenderer();

	void DrawImpl(const Sprite* sprite, 
		          const Sprite* root,
  				  const RenderParams& trans) const;

private:
	static SpriteRenderer* m_instance;
	
}; // SpriteRenderer

}

#endif // _EASYEDITOR_SPRITE_RENDERER_H_