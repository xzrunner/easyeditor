#ifndef _EASYEDITOR_SPRITE_RENDERER_H_
#define _EASYEDITOR_SPRITE_RENDERER_H_

#include "RenderColor.h"
#include "RenderParams.h"

namespace ee
{

class Sprite;
class Symbol;
class SpriteBatch;
class FBO;

class SpriteRenderer
{
public:
	static void Draw(const Sprite* sprite, 
		             const Sprite* root = NULL,
			         const RenderParams& trans = RenderParams());

	static void InvalidRect(const Sprite* sprite, 
		                    const sm::mat4& mt = sm::mat4());

	static void Draw(const Symbol* symbol, 
		             const RenderParams& trans = RenderParams(),
			         const sm::vec2& pos = sm::vec2(0, 0),
			         float angle = 0.0f, 
			         float xScale = 1.0f, 
			         float yScale = 1.0f,
			         float xShear = 0.0f, 
			         float yShear = 0.0f);

private:
	static void DrawImpl(const Sprite* sprite, 
		                 const Sprite* root,
  				         const RenderParams& trans);
	
}; // SpriteRenderer

}

#endif // _EASYEDITOR_SPRITE_RENDERER_H_