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
	virtual void StoreToBin(uint8_t** ptr) const override;
	virtual void StoreToJson(Json::Value& val) const override;

private:
	std::string m_filepath;

}; // NodeImgae

}