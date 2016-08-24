#ifndef _SPRITE2_ANIM_LERP_H_
#define _SPRITE2_ANIM_LERP_H_

#include <vector>

namespace s2
{

class Sprite;

class AnimLerp
{
public:
	static void Lerp(const std::vector<Sprite*>& begin, const std::vector<Sprite*>& end, 
		std::vector<Sprite*>& tween, float process);

	static void Lerp(Sprite* begin, Sprite* end, Sprite* tween, float process);	

	static bool IsMatched(const Sprite* s0, const Sprite* s1);

}; // AnimLerp

}

#endif // _SPRITE2_ANIM_LERP_H_