#pragma once

#include <cu/uncopyable.h>
#include <s2s/ComplexSym.h>
#include <memmgr/LinearAllocator.h>

namespace s2 { class ComplexSymbol; }

namespace s2storer
{

class ComplexSymSaver : private cu::Uncopyable
{
public:
	ComplexSymSaver(const s2::ComplexSymbol& sym);

	void StoreToBin(const std::string& dir, uint8_t** data, size_t& length) const;
	void StoreToJson(const std::string& dir, rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const;

private:
	void InitSym(const s2::ComplexSymbol& src);

private:
	mm::LinearAllocator m_alloc;

	s2s::ComplexSym m_sym;

}; // ComplexSymSaver

}