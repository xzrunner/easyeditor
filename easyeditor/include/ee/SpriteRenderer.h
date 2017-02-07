#ifndef _EASYEDITOR_SPRITE_RENDERER_H_
#define _EASYEDITOR_SPRITE_RENDERER_H_

#include <CU_Singleton.h>
#include <SM_Vector.h>
#include <sprite2/RenderParams.h>

namespace s2 { class Sprite; class Symbol; }

namespace ee
{

class SpriteRenderer
{
public:
	void Draw(const s2::Sprite* spr,
		      const s2::RenderParams& params = s2::RenderParams(),
			  bool scissor = true);

	void Draw(const s2::Symbol* sym, 
		      const s2::RenderParams& params = s2::RenderParams(),
			  const sm::vec2& pos = sm::vec2(0, 0),
			  float angle = 0,
			  const sm::vec2& scale = sm::vec2(1, 1),
			  const sm::vec2& shear = sm::vec2(0, 0));

	//static void InvalidRect(const Sprite* spr, 
	//	                    const sm::mat4& mt = sm::mat4());

private:
	
	SINGLETON_DECLARATION(SpriteRenderer)

}; // SpriteRenderer

}

#endif // _EASYEDITOR_SPRITE_RENDERER_H_