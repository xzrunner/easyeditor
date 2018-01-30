#ifndef _S2LOADER_TEXTURE_SYM_LOADER_H_
#define _S2LOADER_TEXTURE_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class TextureSymbol; }
namespace simp { class NodeTexture; }

namespace s2loader
{

class TextureSymLoader : private cu::Uncopyable
{
public:
	TextureSymLoader(s2::TextureSymbol& sym);

	void LoadJson(const CU_STR& filepath);	
	void LoadBin(const simp::NodeTexture* node);

private:
	s2::TextureSymbol& m_sym;

}; // TextureSymLoader

}

#endif // _S2LOADER_TEXTURE_SYM_LOADER_H_
