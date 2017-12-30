#pragma once

#include <cu/uncopyable.h>

#include <string>

#include <stdint.h>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; class ExportStream; }
namespace Json { class Value; }

namespace bsn
{

class NodeSprBase : private cu::Uncopyable
{
public:
	NodeSprBase();

	//
	// serialization
	//
	size_t GetBinSize() const;
	void StoreToBin(bs::ExportStream& es) const;
	void StoreToJson(Json::Value& val) const;

	//
	// deserialization
	//
	void LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is);
	void LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val);

	const char* GetFilepath() const { return m_sym_path; }

	uint32_t GetType() const { return m_type; }
	const uint32_t* GetData() const { return m_data; }

protected:
	static size_t DataSize(uint32_t type);

	static char* String2Char(mm::LinearAllocator& alloc, const std::string& str);

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
	static const int LOW_FIXED_TRANS_PRECISION = 1024;
	static const int HIGH_FIXED_TRANS_PRECISION = 8192;

private:
	const char* m_sym_path;

	const char* m_name;

	uint32_t  m_type;
	uint32_t* m_data;

}; // NodeSprBase

}