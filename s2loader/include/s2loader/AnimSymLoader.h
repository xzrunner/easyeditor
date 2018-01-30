#ifndef _S2LOADER_ANIM_SYM_LOADER_H_
#define _S2LOADER_ANIM_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <memory>

namespace s2 { class AnimSymbol; }
namespace simp { class NodeAnimation; }

namespace s2loader
{

class SymbolLoader;
class SpriteLoader;

class AnimSymLoader : private cu::Uncopyable
{
public:
	AnimSymLoader(s2::AnimSymbol& sym, 
		const std::shared_ptr<const SymbolLoader>& sym_loader = nullptr,
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr);

	void LoadJson(const CU_STR& filepath);
	void LoadBin(const simp::NodeAnimation* node);
	void LoadSns(const CU_STR& filepath);

private:
	s2::AnimSymbol& m_sym;

	std::shared_ptr<const SymbolLoader> m_sym_loader;
	std::shared_ptr<const SpriteLoader> m_spr_loader;

}; // AnimSymLoader

}

#endif // _S2LOADER_ANIM_SYM_LOADER_H_
