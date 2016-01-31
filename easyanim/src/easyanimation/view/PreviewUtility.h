#ifndef _EASYANIM_PREVIEW_UTILITY_H_
#define _EASYANIM_PREVIEW_UTILITY_H_



namespace eanim
{

class PreviewUtility
{
public:
	static void GetCurrSprites(const ee::PlayControl& ctrl, std::vector<ee::Sprite*>& sprites);

}; // PreviewUtility

}

#endif // _EASYANIM_PREVIEW_UTILITY_H_