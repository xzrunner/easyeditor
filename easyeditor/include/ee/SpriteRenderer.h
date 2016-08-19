#ifndef _EASYEDITOR_SPRITE_RENDERER_H_
#define _EASYEDITOR_SPRITE_RENDERER_H_

#include <SM_Matrix.h>
#include <sprite2/RenderParams.h>

namespace ee
{

class Sprite;
class Symbol;
class SpriteBatch;
class FBO;

class SpriteRenderer
{
public:
	static void Draw(const Sprite* spr, 
			         const s2::RenderParams& params = s2::RenderParams());

	static void InvalidRect(const Sprite* spr, 
		                    const sm::mat4& mt = sm::mat4());

private:
	static void DrawAnchor(const Sprite* spr, const s2::RenderParams& params);
	
}; // SpriteRenderer

}

#endif // _EASYEDITOR_SPRITE_RENDERER_H_