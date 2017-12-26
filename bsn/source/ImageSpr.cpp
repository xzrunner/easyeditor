#include "bsn/NodeImage.h"

#include <bs/Serializer.h>

namespace bsn
{

NodeImage::NodeImage(const std::string& filepath)
{
}

size_t NodeImage::GetByteArraySize() const
{
	return bs::pack_size(m_filepath);
}

void NodeImage::LoadFromByteArray(const byte* data)
{
	bs::unpack(m_filepath, &data);
}

void NodeImage::StoreToByteArray(byte** data, size_t& length) const
{
	length = GetByteArraySize();
	bs::pack(m_filepath, data);
}

}