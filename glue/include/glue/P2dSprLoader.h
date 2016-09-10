#ifndef _GLUE_P2D_SPR_LOADER_H_
#define _GLUE_P2D_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class Particle2dSprite; }

namespace glue
{

class P2dSprLoader : private cu::Uncopyable
{
public:
	P2dSprLoader(s2::Particle2dSprite* spr);
	~P2dSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::Particle2dSprite* m_spr;

}; // P2dSprLoader

}

#endif // _GLUE_P2D_SPR_LOADER_H_