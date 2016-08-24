#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "S2_Sprite.h"

#include <vector>

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;
	virtual bool Update(const RenderParams& params, float dt);

	void Start();

	void Draw(const RenderParams& params) const;

	void SetLoop(bool loop) { m_loop = loop; }

private:
	void UpdateCurrFrame();

private:
	struct Frame
	{
		std::vector<Sprite*> sprs;

		Sprite* Query(const Sprite* spr);
	};

	struct Layer
	{
		Frame frame;
	};

	struct CurrFrame
	{
		std::vector<Layer> layers;
		int frame;
	};

	VI_DUMMY_FUNC

private:
	float m_time;

	CurrFrame m_curr_frame;

	bool m_loop;

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_