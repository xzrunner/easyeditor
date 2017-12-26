#pragma once

#include "bsn.h"

#include <memmgr/LinearAllocator.h>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }
namespace json { class Value; }

namespace bsn
{

class NodeSpr : public INode
{
public:
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(byte** data, size_t& length) const override;
	virtual void StoreToJson(json::Value& val) const override;

	static NodeSpr* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static NodeSpr* Create(mm::LinearAllocator& alloc, const json::Value& val);

private:
	NodeSpr();

private:
	static size_t Size(uint32_t type);

public:
	// geometry
	static const int SCALE_MASK			= 1 << 1;
	static const int SHEAR_MASK			= 1 << 2;
	static const int OFFSET_MASK		= 1 << 3;
	static const int POSITION_MASK		= 1 << 4;
	static const int ANGLE_MASK			= 1 << 5;

	// color
	static const int COL_MUL_MASK		= 1 << 11;
	static const int COL_ADD_MASK		= 1 << 12;
	static const int COL_R_MASK			= 1 << 13;
	static const int COL_G_MASK			= 1 << 14;
	static const int COL_B_MASK			= 1 << 15;

	// shader
	static const int BLEND_MASK			= 1 << 21;
	static const int FAST_BLEND_MASK	= 1 << 22;
	static const int FILTER_MASK		= 1 << 23;	
	static const int CAMERA_MASK		= 1 << 24;
	static const int DOWNSMAPLE_MASK    = 1 << 25;

	// other
	static const int ACTOR_MASK         = 1 << 28;
	static const int INTEGRATE_MASK     = 1 << 29;

public:
	const char* m_sym_path;

	const char* m_name;

	uint32_t m_type;
	uint32_t m_data[1];
	
}; // NodeSpr

}
