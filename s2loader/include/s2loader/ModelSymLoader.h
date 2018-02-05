#pragma once

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

namespace s2 { class ModelSymbol; }

namespace s2loader
{

class ModelSymLoader : private cu::Uncopyable
{
public:
	ModelSymLoader(s2::ModelSymbol& sym);

	void LoadFromFile(const CU_STR& filepath);

private:
	s2::ModelSymbol& m_sym;

}; // ModelSymLoader

}