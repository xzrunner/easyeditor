#ifndef _GUM_P2D_SPR_LOADER_H_
#define _GUM_P2D_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class Particle2dSprite; }
namespace simp { class NodeParticle2dSpr; }

namespace gum
{

class P2dSprLoader : private cu::Uncopyable
{
public:
	P2dSprLoader(s2::Particle2dSprite* spr);
	~P2dSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);
	void LoadBin(const simp::NodeParticle2dSpr* node);

private:
	s2::Particle2dSprite* m_spr;

}; // P2dSprLoader

}

#endif // _GUM_P2D_SPR_LOADER_H_
