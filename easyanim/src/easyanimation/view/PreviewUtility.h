#ifndef _EASYANIM_PREVIEW_UTILITY_H_
#define _EASYANIM_PREVIEW_UTILITY_H_

#include <drag2d.h>

namespace eanim
{

class PreviewUtility
{
public:
	static void GetCurrSprites(const d2d::PlayControl& ctrl, std::vector<d2d::Sprite*>& sprites);

}; // PreviewUtility

}

#endif // _EASYANIM_PREVIEW_UTILITY_H_