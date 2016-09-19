#ifndef _GUM_MASK_SYM_LOADER_H_
#define _GUM_MASK_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <string>

namespace s2 { class MaskSymbol; }
namespace simp { class NodeMask; }

namespace gum
{

class MaskSymLoader : private cu::Uncopyable
{
public:
	MaskSymLoader(s2::MaskSymbol* sym);
	~MaskSymLoader();

	void LoadJson(const std::string& filepath);
	void LoadBin(const simp::NodeMask* node);

private:
	s2::MaskSymbol* m_sym;

}; // MaskSymLoader

}

#endif // _GUM_MASK_SYM_LOADER_H_
