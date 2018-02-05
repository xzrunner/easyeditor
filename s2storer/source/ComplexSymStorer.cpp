#include "s2storer/ComplexSymStorer.h"

#include <sprite2/ComplexSymbol.h>

namespace s2storer
{

ComplexSymSaver::ComplexSymSaver(const s2::ComplexSymbol& sym)
{
	InitSym(sym);
}

void ComplexSymSaver::StoreToBin(const std::string& dir, uint8_t** data, size_t& length) const
{

}

void ComplexSymSaver::StoreToJson(const std::string& dir, rapidjson::Value& val, 
	                              rapidjson::MemoryPoolAllocator<>& alloc) const
{

}

void ComplexSymSaver::InitSym(const s2::ComplexSymbol& src)
{
	auto& scissor = src.GetScissor();
	m_sym.scissor[0] = scissor.xmin;
	m_sym.scissor[1] = scissor.ymin;
	m_sym.scissor[2] = scissor.xmax;
	m_sym.scissor[3] = scissor.ymax;

	auto& children = src.GetAllChildren();
	m_sym.children_n = children.size();
	for (auto& child_src : children)
	{
		
	}
}

}