#ifndef _S2LOADER_P2D_SPR_LOADER_H_
#define _S2LOADER_P2D_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class Particle2dSprite; }
namespace simp { class NodeParticle2dSpr; }

namespace s2loader
{

class P2dSprLoader : private cu::Uncopyable
{
public:
	P2dSprLoader(s2::Particle2dSprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeParticle2dSpr* node);

private:
	s2::Particle2dSprite& m_spr;

}; // P2dSprLoader

}

#endif // _S2LOADER_P2D_SPR_LOADER_H_
