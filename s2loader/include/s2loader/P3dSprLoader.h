#ifndef _S2LOADER_P3D_SPR_LOADER_H_
#define _S2LOADER_P3D_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class Particle3dSprite; }
namespace simp { class NodeParticle3dSpr; }

namespace s2loader
{

class P3dSprLoader : private cu::Uncopyable
{
public:
	P3dSprLoader(s2::Particle3dSprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeParticle3dSpr* node);

private:
	s2::Particle3dSprite& m_spr;

}; // P3dSprLoader

}

#endif // _S2LOADER_P3D_SPR_LOADER_H_
