#pragma once

#include "bsn.h"

#include <string>

namespace bsn
{
	
class ImageSpr : public INode
{
public:
	ImageSpr(const std::string& filepath);

	////
	//// ISerializable interface
	////
	//virtual size_t GetByteArraySize() const override;
	//virtual void LoadFromByteArray(const byte* data) override;
	//virtual void StoreToByteArray(byte** data, size_t& length) const override;

private:
	std::string m_filepath;

}; // NodeImgae

}