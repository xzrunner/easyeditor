#pragma once

#include "bsn/NodeSpr.h"

#include <string>

namespace bsn
{
	
class ImageSpr : public NodeSpr
{
public:
	ImageSpr(const std::string& filepath);

	//
	// serialization
	//
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(uint8_t** data, size_t& length) const override;
	virtual void StoreToJson(Json::Value& val) const override;

private:
	std::string m_filepath;

}; // NodeImgae

}