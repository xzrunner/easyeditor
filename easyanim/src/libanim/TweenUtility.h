#ifndef _EASYANIM_TWEEN_UTILITY_H_
#define _EASYANIM_TWEEN_UTILITY_H_

#include <drag2d.h>

namespace libanim
{

class TweenUtility
{
public:
	static void GetTweenSprites(const std::vector<d2d::ISprite*>& start, 
		const std::vector<d2d::ISprite*>& end, 
		std::vector<d2d::ISprite*>& tween, float process);

	static bool IsTweenMatched(const d2d::ISprite* s0, const d2d::ISprite* s1);

	static void GetTweenSprite(d2d::ISprite* start, d2d::ISprite* end, 
		d2d::ISprite* tween, float process);	

}; // TweenUtility

}

#endif // _EASYANIM_TWEEN_UTILITY_H_