#ifndef _GUM_ANIM_SYM_LOADER_H_
#define _GUM_ANIM_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class AnimSymbol; class Sprite; }

namespace gum
{

class AnimSymLoader : private cu::Uncopyable
{
public:
	AnimSymLoader(s2::AnimSymbol* sym);
	~AnimSymLoader();

	void LoadJson(const std::string& filepath);
 
	void LoadLayers(const Json::Value& value, const std::string& dir);

protected:
	virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

private:
	s2::AnimSymbol* m_sym;

}; // AnimSymLoader

}

#endif // _GUM_ANIM_SYM_LOADER_H_
