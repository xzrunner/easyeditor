#ifndef _S2LOADER_ICON_SYM_LOADER_H_
#define _S2LOADER_ICON_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>
#include <SM_Vector.h>

#include <json/json.h>

namespace s2 { class IconSymbol; class Icon; }
namespace simp { class NodeIcon; }

namespace s2loader
{

class IconSymLoader : private cu::Uncopyable
{
public:
	IconSymLoader(s2::IconSymbol& sym);

	void LoadJson(const CU_STR& filepath);	
	void LoadBin(const simp::NodeIcon* node);
	void LoadSns(const CU_STR& filepath);

private:
	static std::unique_ptr<s2::Icon> CreateIcon(
		int type, const CU_VEC<sm::vec2>& vertices);

private:
	s2::IconSymbol& m_sym;

}; // IconSymLoader

}

#endif // _S2LOADER_ICON_SYM_LOADER_H_
