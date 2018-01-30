#ifndef _S2LOADER_TRAIL_SPR_LOADER_H_
#define _S2LOADER_TRAIL_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

namespace s2 { class TrailSprite; }
namespace simp { class NodeTrailSpr; }

namespace s2loader
{

class TrailSprLoader : private cu::Uncopyable
{
public:
	TrailSprLoader(const std::shared_ptr<s2::TrailSprite>& spr);

	void LoadJson(const Json::Value& val, const std::string& dir);
	void LoadBin(const simp::NodeTrailSpr* node);

private:
	std::shared_ptr<s2::TrailSprite> m_spr;

}; // TrailSprLoader

}

#endif // _S2LOADER_TRAIL_SPR_LOADER_H_
