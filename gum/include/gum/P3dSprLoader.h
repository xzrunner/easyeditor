#ifndef _GUM_P3D_SPR_LOADER_H_
#define _GUM_P3D_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class Particle3dSprite; }

namespace gum
{

class P3dSprLoader : private cu::Uncopyable
{
public:
	P3dSprLoader(s2::Particle3dSprite* spr);
	~P3dSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::Particle3dSprite* m_spr;

}; // P3dSprLoader

}

#endif // _GUM_P3D_SPR_LOADER_H_
