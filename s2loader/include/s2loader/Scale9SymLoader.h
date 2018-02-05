#ifndef _S2LOADER_SCALE9_SYM_LOADER_H_
#define _S2LOADER_SCALE9_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

#include <stdint.h>

namespace s2 { class Scale9Symbol; class Sprite; }
namespace simp { class NodeScale9; }
namespace s2s { class NodeSprCommon; }

namespace s2loader
{

class SpriteLoader;

class Scale9SymLoader : private cu::Uncopyable
{
public:
	Scale9SymLoader(s2::Scale9Symbol& sym, 
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr);

	void LoadJson(const CU_STR& filepath);
	void LoadBin(const simp::NodeScale9* node);
	void LoadSns(const CU_STR& filepath);

private:
	std::shared_ptr<s2::Sprite> LoadSprite(uint32_t sym_id, uint16_t dir, uint16_t mirror);
	std::shared_ptr<s2::Sprite> LoadSprite(const s2s::NodeSprCommon& spr_common);

private:
	s2::Scale9Symbol& m_sym;

	std::shared_ptr<const SpriteLoader> m_spr_loader;

}; // Scale9SymLoader

}

#endif // _S2LOADER_SCALE9_SYM_LOADER_H_
