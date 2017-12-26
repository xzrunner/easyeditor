#pragma once

#include "bsn.h"

#include <string>

namespace bsn
{

class NodeComplex : public INode
{
public:
	NodeComplex(const std::string& filepath);

	//
	// ISerializable interface
	//
	virtual size_t GetByteArraySize() const override;
	virtual void LoadFromByteArray(const byte* data) override;
	virtual void StoreToByteArray(byte** data, size_t& length) const override;

private:


}; // NodeComplex

}
