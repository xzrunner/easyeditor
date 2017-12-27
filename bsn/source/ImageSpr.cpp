#include "bsn/ImageSpr.h"

#include <bs/Serializer.h>

namespace bsn
{

ImageSpr::ImageSpr(const std::string& filepath)
{
}

size_t ImageSpr::GetBinSize() const
{
	return bs::pack_size(m_filepath);
}

void ImageSpr::StoreToBin(uint8_t** data, size_t& length) const
{

}

void ImageSpr::StoreToJson(Json::Value& val) const
{

}

}