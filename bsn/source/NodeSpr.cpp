#include "bsn/NodeSpr.h"
#include "bsn/ColorParser.h"

#include <bs/ImportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>
#include <bs/FixedPointNum.h>

#include <json/value.h>

namespace bsn
{

static const int LOW_FIXED_TRANS_PRECISION = 1024;
static const int HIGH_FIXED_TRANS_PRECISION = 8192;

NodeSpr::NodeSpr()
	: m_sym_path(nullptr)
	, m_name(nullptr)
	, m_type(0)
	, m_data(nullptr)
{
}

//size_t NodeSpr::GetBinSize() const
//{
//	int sz = TypeSize();
//	sz += bs::pack_size(m_sym_path);
//	sz += bs::pack_size(m_name);
//	sz += DataSize(m_type);
//	return sz;
//}
//
//void NodeSpr::StoreToBin(byte** data, size_t& length) const
//{
//
//}
//
//void NodeSpr::StoreToJson(json::Value& val) const
//{
//
//}

void NodeSpr::LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	m_sym_path = is.String(alloc);
	m_name = is.String(alloc);
	m_type = is.UInt32();

	m_data = static_cast<uint32_t*>(alloc.alloc<char>(DataSize(m_type)));
	int idx = 0;
	if (m_type & SCALE_MASK) {
		m_data[idx++] = is.UInt32();
		m_data[idx++] = is.UInt32();
	}
	if (m_type & SHEAR_MASK) {
		m_data[idx++] = is.UInt32();
		m_data[idx++] = is.UInt32();
	}
	if (m_type & OFFSET_MASK) {
		m_data[idx++] = is.UInt32();
		m_data[idx++] = is.UInt32();
	}
	if (m_type & POSITION_MASK) {
		m_data[idx++] = is.UInt32();
		m_data[idx++] = is.UInt32();
	}
	if (m_type & ANGLE_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & COL_MUL_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & COL_ADD_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & COL_R_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & COL_G_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & COL_B_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & BLEND_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & FAST_BLEND_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & FILTER_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & DOWNSMAPLE_MASK) {
		m_data[idx++] = is.UInt32();
	}
	if (m_type & CAMERA_MASK) {
		m_data[idx++] = is.UInt32();
	}
}

void NodeSpr::LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val)
{
	// load name
	m_name = nullptr;
	if (val.isMember("name")) {
		std::string name = val["name"].asString();
		m_name = String2Char(alloc, name);
	}

	// load filepath
	m_sym_path = nullptr;
	if (val.isMember("filepath")) {
		std::string filepath = val["filepath"].asString();
		m_sym_path = String2Char(alloc, filepath);
	}

	m_type = 0;

	std::vector<uint32_t> data;

	// load scale
	float scale[2] = { 1, 1 };
	if (val.isMember("x scale") && val.isMember("y scale")) {
		scale[0] = static_cast<float>(val["x scale"].asDouble());
		scale[1] = static_cast<float>(val["y scale"].asDouble());
	} else if (val.isMember("scale")) {
		scale[0] = scale[1] = static_cast<float>(val["scale"].asDouble());
	}
	if (scale[0] != 1 || scale[1] != 1) {
		m_type |= SCALE_MASK;
		data.push_back(bs::float2int(scale[0], HIGH_FIXED_TRANS_PRECISION));
		data.push_back(bs::float2int(scale[1], HIGH_FIXED_TRANS_PRECISION));
	}

	// load shear
	float shear[2] = { 0, 0 };
	if (val.isMember("x shear") && val.isMember("y shear")) {
		shear[0] = static_cast<float>(val["x shear"].asDouble());
		shear[1] = static_cast<float>(val["y shear"].asDouble());
	}
	if (shear[0] != 0 || shear[1] != 0) {
		m_type |= SHEAR_MASK;
		data.push_back(bs::float2int(shear[0], HIGH_FIXED_TRANS_PRECISION));
		data.push_back(bs::float2int(shear[1], HIGH_FIXED_TRANS_PRECISION));
	}

	// load offset
	float offset[2] = { 0, 0 };
	if (val.isMember("x offset") && val.isMember("y offset"))
	{
		offset[0] = static_cast<float>(val["x offset"].asDouble());
		offset[1] = static_cast<float>(val["y offset"].asDouble());
	}
	if (offset[0] != 0 || offset[1] != 0) {
		m_type |= OFFSET_MASK;
		data.push_back(bs::float2int(offset[0], LOW_FIXED_TRANS_PRECISION));
		data.push_back(bs::float2int(offset[1], LOW_FIXED_TRANS_PRECISION));
	}

	// load position
	float position[2] = { 0, 0 };
	if (val.isMember("position") && val["position"].isMember("x") && val["position"].isMember("y")) {
		position[0] = static_cast<float>(val["position"]["x"].asDouble());
		position[1] = static_cast<float>(val["position"]["y"].asDouble());
	}
	if (position[0] != 0 || position[1] != 0) {
		m_type |= POSITION_MASK;
		data.push_back(bs::float2int(position[0], LOW_FIXED_TRANS_PRECISION));
		data.push_back(bs::float2int(position[1], LOW_FIXED_TRANS_PRECISION));
	}

	// load rotate
	float angle = 0;
	if (val.isMember("angle")) {
		angle = static_cast<float>(val["angle"].asDouble());
	}
	if (angle != 0) {
		m_type |= SCALE_MASK;
		data.push_back(bs::float2int(angle, HIGH_FIXED_TRANS_PRECISION));
	}

	// load¡¡color mul
	uint32_t col_mul = 0xffffffff;
	if (val.isMember("multi color")) {
		std::string str = val["multi color"].asString();
		if (!str.empty()) {
			col_mul = ColorParser::StringToRGBA(str, BGRA);
		}
	}
	if (col_mul != 0xffffffff) {
		m_type |= COL_MUL_MASK;
		data.push_back(col_mul);
	}

	// load color add
	uint32_t col_add = 0;
	if (val.isMember("add color")) {
		std::string str = val["add color"].asString();
		if (!str.empty()) {
			col_add = ColorParser::StringToRGBA(str, ABGR);
		}
	}
	if (col_add != 0) {
		m_type |= COL_ADD_MASK;
		data.push_back(col_add);
	}

	// load color rmap
	uint32_t col_rmap = 0xff000000;
	if (val.isMember("r trans")) {
		std::string str = val["r trans"].asString();
		if (!str.empty()) {
			col_rmap = ColorParser::StringToRGBA(str, RGBA);
			col_rmap &= 0xffffff00;
		}
	}
	if (col_rmap != 0xff000000) {
		m_type |= COL_R_MASK;
		data.push_back(col_rmap);
	}

	// load color gmap
	uint32_t col_gmap = 0x00ff0000;
	if (val.isMember("g trans")) {
		std::string str = val["g trans"].asString();
		if (!str.empty()) {
			col_gmap = ColorParser::StringToRGBA(str, RGBA);
			col_gmap &= 0xffffff00;
		}
	}
	if (col_gmap != 0x00ff0000) {
		m_type |= COL_G_MASK;
		data.push_back(col_gmap);
	}

	// load color bmap
	uint32_t col_bmap = 0x0000ff00;
	if (val.isMember("b trans")) {
		std::string str = val["b trans"].asString();
		if (!str.empty()) {
			col_bmap = ColorParser::StringToRGBA(str, RGBA);
			col_bmap &= 0xffffff00;
		}
	}
	if (col_bmap != 0x0000ff00) {
		m_type |= COL_B_MASK;
		data.push_back(col_bmap);
	}

	// store
	if (data.empty())
	{
		m_data = nullptr;
	}
	else
	{
		size_t sz = sizeof(float) * data.size();
		m_data = static_cast<uint32_t*>(alloc.alloc<char>(sz));
		memcpy(m_data, &data[0], sz);
	}
}

size_t NodeSpr::DataSize(uint32_t type)
{
	size_t sz = 0;

	if (type & SCALE_MASK) {
		sz += sizeof(float) * 2;
	}
	if (type & SHEAR_MASK) {
		sz += sizeof(float) * 2;
	}
	if (type & OFFSET_MASK) {
		sz += sizeof(float) * 2;
	}
	if (type & POSITION_MASK) {
		sz += sizeof(float) * 2;
	}
	if (type & ANGLE_MASK) {
		sz += sizeof(float);
	}
	if (type & COL_MUL_MASK) {
		sz += sizeof(float);
	}
	if (type & COL_ADD_MASK) {
		sz += sizeof(float);
	}
	if (type & COL_R_MASK) {
		sz += sizeof(float);
	}
	if (type & COL_G_MASK) {
		sz += sizeof(float);
	}
	if (type & COL_B_MASK) {
		sz += sizeof(float);
	}
	if (type & BLEND_MASK) {
		sz += sizeof(float);
	}
	if (type & FAST_BLEND_MASK) {
		sz += sizeof(float);
	}
	if (type & FILTER_MASK) {
		sz += sizeof(float);
	}
	if (type & DOWNSMAPLE_MASK) {
		sz += sizeof(float);
	}
	if (type & CAMERA_MASK) {
		sz += sizeof(float);
	}

	return ALIGN_4BYTE(sz);
}

char* NodeSpr::String2Char(mm::LinearAllocator& alloc, const std::string& str)
{
	if (str.empty()) {
		return nullptr;
	}

	char* ret = static_cast<char*>(alloc.alloc<char>(str.size() + 1));

	strcpy(ret, str.c_str());
	ret[str.size()] = 0;

	return ret;
}

}