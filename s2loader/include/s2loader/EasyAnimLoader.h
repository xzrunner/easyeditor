#ifndef _S2LOADER_EASYANIM_LOADER_H_
#define _S2LOADER_EASYANIM_LOADER_H_

#include <cu/uncopyable.h>
#include <simp/NodeAnimation.h>
#include <sprite2/AnimSymbol.h>
#include <s2s/AnimSym.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class AnimSymbol; class Sprite; }
namespace simp { class NodeAnimation; }
namespace s2s { class AnimSym; }

namespace s2loader
{

class SpriteLoader;

class EasyAnimLoader : private cu::Uncopyable
{
public:
	EasyAnimLoader(s2::AnimSymbol& sym, 
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeAnimation* node);
	void LoadSns(const s2s::AnimSym* sym, const CU_STR& dir);

private:
	// json
	void LoadLayers(const Json::Value& val, const CU_STR& dir);
 	void LoadActors(const Json::Value& src, s2::AnimSymbol::Frame& dst, const CU_STR& dir);
 	void LoadLerps(const Json::Value& src, s2::AnimSymbol::Frame& dst);

	// bin
	void LoadActors(const simp::NodeAnimation::Frame* src, s2::AnimSymbol::Frame& dst);
	void LoadLerps(const simp::NodeAnimation::Frame* src, s2::AnimSymbol::Frame& dst);

	// s2s
	void LoadActors(const s2s::AnimSym::Frame& src, s2::AnimSymbol::Frame& dst, const CU_STR& dir);
	void LoadLerps(const s2s::AnimSym::Frame& src, s2::AnimSymbol::Frame& dst);

private:
	s2::AnimSymbol& m_sym;

	std::shared_ptr<const SpriteLoader> m_spr_loader;

}; // EasyAnimLoader

}

#endif // _S2LOADER_EASYANIM_LOADER_H_
