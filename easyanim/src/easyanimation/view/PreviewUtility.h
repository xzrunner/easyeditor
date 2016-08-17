#ifndef _EASYANIM_PREVIEW_UTILITY_H_
#define _EASYANIM_PREVIEW_UTILITY_H_

#include <vector>

namespace ee { class Sprite; class PlayControl; }

namespace eanim
{

class PreviewUtility
{
public:
	static void GetCurrSprites(const ee::PlayControl& ctrl, std::vector<ee::Sprite*>& sprs);

}; // PreviewUtility

}

#endif // _EASYANIM_PREVIEW_UTILITY_H_