#ifndef _EASYANIM_TWEEN_UTILITY_H_
#define _EASYANIM_TWEEN_UTILITY_H_

#include <drag2d.h>

namespace libanim
{

class TweenUtility
{
public:
	static void GetTweenSprites(const std::vector<d2d::Sprite*>& start, 
		const std::vector<d2d::Sprite*>& end, 
		std::vector<d2d::Sprite*>& tween, float process);

	static bool IsTweenMatched(const d2d::Sprite* s0, const d2d::Sprite* s1);

	static void GetTweenSprite(d2d::Sprite* start, d2d::Sprite* end, 
		d2d::Sprite* tween, float process);	

}; // TweenUtility

}

#endif // _EASYANIM_TWEEN_UTILITY_H_