#ifndef _GLUE_MASK_SYM_LOADER_H_
#define _GLUE_MASK_SYM_LOADER_H_

#include <CU_Uncopyable.h>

namespace s2 { class MaskSymbol; }

#include <string>

namespace glue
{

class MaskSymLoader : private cu::Uncopyable
{
public:
	MaskSymLoader(s2::MaskSymbol* sym);
	~MaskSymLoader();

	void LoadJson(const std::string& filepath);

private:
	s2::MaskSymbol* m_sym;

}; // MaskSymLoader

}

#endif // _GLUE_MASK_SYM_LOADER_H_