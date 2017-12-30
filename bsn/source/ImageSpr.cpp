#include "bsn/ImageSpr.h"
#include "bsn/NodeType.h"

#include <bs/ExportStream.h>

namespace bsn
{

size_t ImageSpr::GetBinSize() const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);	       // type
	sz += m_base_info.GetBinSize(); // base
	return sz;
}

void ImageSpr::StoreToBin(bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(NODE_IMAGE)); // type
	m_base_info.StoreToBin(es);                  // base
}

void ImageSpr::StoreToJson(Json::Value& val) const
{
	m_base_info.StoreToJson(val);
}

void ImageSpr::LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	m_base_info.LoadFromBin(alloc, is);
}

void ImageSpr::LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val)
{
	m_base_info.LoadFromJson(alloc, val);
}

}