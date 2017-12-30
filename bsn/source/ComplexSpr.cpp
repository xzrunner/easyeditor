#include "bsn/ComplexSpr.h"
#include "bsn/NodeType.h"

#include <bs/ImportStream.h>
#include <bs/ExportStream.h>

namespace bsn
{

size_t ComplexSpr::GetBinSize() const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);	       // type
	sz += m_base_info.GetBinSize(); // base
	sz += sizeof(uint16_t);        // action
	return sz;
}

void ComplexSpr::StoreToBin(bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(NODE_COMPLEX)); // type
	m_base_info.StoreToBin(es);                    // base
	es.Write(static_cast<uint16_t>(m_action));    // action
}

void ComplexSpr::StoreToJson(Json::Value& val) const
{
	m_base_info.StoreToJson(val);
}

void ComplexSpr::LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	m_base_info.LoadFromBin(alloc, is);
	m_action = is.UInt16();
}

void ComplexSpr::LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val)
{
	m_base_info.LoadFromJson(alloc, val);
}

}