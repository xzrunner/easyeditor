#ifndef _GUM_ICON_SYM_LOADER_H_
#define _GUM_ICON_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class IconSymbol; }
namespace simp { class NodeIcon; }

namespace gum
{

class IconSymLoader : private cu::Uncopyable
{
public:
	IconSymLoader(s2::IconSymbol* sym);
	~IconSymLoader();

	void LoadJson(const std::string& filepath);	
	void LoadBin(const simp::NodeIcon* node);

private:
	s2::IconSymbol* m_sym;

}; // IconSymLoader

}

#endif // _GUM_ICON_SYM_LOADER_H_