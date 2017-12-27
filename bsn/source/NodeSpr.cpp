#include "bsn/NodeSpr.h"

#include <bs/ImportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>

#include <json/value.h>

namespace bsn
{

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

	m_data = static_cast<float*>(alloc.alloc<char>(DataSize(m_type)));
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

	std::vector<float> data;

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
		data.push_back(scale[0]);
		data.push_back(scale[1]);
	}

	// load shear
	float shear[2] = { 0, 0 };
	if (val.isMember("x shear") && val.isMember("y shear")) {
		shear[0] = static_cast<float>(val["x shear"].asDouble());
		shear[1] = static_cast<float>(val["y shear"].asDouble());
	}
	if (shear[0] != 0 || shear[1] != 0) {
		m_type |= SHEAR_MASK;
		data.push_back(shear[0]);
		data.push_back(shear[1]);
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
		data.push_back(offset[0]);
		data.push_back(offset[1]);
	}

	// load position
	float position[2] = { 0, 0 };
	if (val.isMember("position") && val["position"].isMember("x") && val["position"].isMember("y")) {
		position[0] = static_cast<float>(val["position"]["x"].asDouble());
		position[1] = static_cast<float>(val["position"]["y"].asDouble());
	}
	if (position[0] != 0 || position[1] != 0) {
		m_type |= POSITION_MASK;
		data.push_back(position[0]);
		data.push_back(position[1]);
	}

	// load rotate
	float angle = 0;
	if (val.isMember("angle")) {
		angle = static_cast<float>(val["angle"].asDouble());
	}
	if (angle != 0) {
		m_type |= SCALE_MASK;
		data.push_back(angle);
	}

	// load¡¡color mul
	uint32_t col_mul = 0xffffffff;
	if (val.isMember("multi color")) {
		std::string str = val["multi color"].asString().c_str();
		if (!str.empty()) {
			col_mul = str2color(str, BGRA);
		}
	}
	if (col_mul != 0xffffffff) {
		m_type |= COL_MUL_MASK;
		
	}

	m_col.SetAdd(s2::Color(0, 0, 0, 0));
	if (val.isMember("add color")) {
		std::string str = val["add color"].asString().c_str();
		if (!str.empty()) {
			m_col.SetAdd(str2color(str, s2::ABGR));
		}
	}

	m_col.SetRMap(s2::Color(255, 0, 0, 0));
	if (val.isMember("r trans")) {
		std::string str = val["r trans"].asString().c_str();
		if (!str.empty()) {
			s2::Color col(str2color(str, s2::RGBA));
			col.a = 0;
			m_col.SetRMap(col);
		}
	}

	m_col.SetGMap(s2::Color(0, 255, 0, 0));
	if (val.isMember("g trans")) {
		std::string str = val["g trans"].asString().c_str();
		if (!str.empty()) {
			s2::Color col(str2color(str, s2::RGBA));
			col.a = 0;
			m_col.SetGMap(col);
		}
	}

	m_col.SetBMap(s2::Color(0, 0, 255, 0));
	if (val.isMember("b trans")) {
		std::string str = val["b trans"].asString().c_str();
		if (!str.empty()) {
			s2::Color col(str2color(str, s2::RGBA));
			col.a = 0;
			m_col.SetBMap(col);
		}
	}

	// store
	if (data.empty())
	{
		m_data = nullptr;
	}
	else
	{
		size_t sz = sizeof(float) * data.size();
		m_data = static_cast<float*>(alloc.alloc<char>(sz));
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

enum PIXEL_TYPE
{
	RGBA = 0,
	ARGB,
	ABGR,
	BGRA
};

static inline
int char2hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else {
		GD_REPORT_ASSERT("char2hex");
		return 0;
	}
}
static inline
int char2channel(char high, char low)
{
	int col = char2hex(high) * 16 + char2hex(low);
	GD_ASSERT(col >= 0 && col <= 255, "char2channel");
	return col;
}

static inline 
uint32_t str2color(const std::string& str, PIXEL_TYPE type)
{
	std::string snum = str;

	if (snum.empty()) {
		return 0;
	}
	if (snum == "0xffffffff") {
		return 0xffffffff;
	}

	if (snum[0] != '0' || (snum[1] != 'x' && snum[1] != 'X'))
	{
		int n = atoi(snum.c_str());
		char buffer[33];
		itoa(n, buffer, 16);
		snum = "0x" + std::string(buffer);
	}

	int len = snum.length();

	uint8_t ret[4] = { 0, 0, 0, 0 }; // rgba
	if (len == 4)
	{
		if (type == RGBA || BGRA)
			ret[3] = char2channel(snum[2], snum[3]);
		else if (type == ARGB)
			ret[2] = char2channel(snum[2], snum[3]);
		else if (type == ABGR)
			ret[0] = char2channel(snum[2], snum[3]);
	}
	else if (len == 10)
	{
		if (type == RGBA) {
			ret[0] = char2channel(snum[2], snum[3]);
			ret[1] = char2channel(snum[4], snum[5]);
			ret[2] = char2channel(snum[6], snum[7]);
			ret[3] = char2channel(snum[8], snum[9]);
		} else if (type == ARGB) {
			ret[3] = char2channel(snum[2], snum[3]);
			ret[0] = char2channel(snum[4], snum[5]);
			ret[1] = char2channel(snum[6], snum[7]);
			ret[2] = char2channel(snum[8], snum[9]);
		} else if (type == ABGR) {
			ret[3] = char2channel(snum[2], snum[3]);
			ret[2] = char2channel(snum[4], snum[5]);
			ret[1] = char2channel(snum[6], snum[7]);
			ret[0] = char2channel(snum[8], snum[9]);
		}  else if (type == BGRA) {
			ret[2] = char2channel(snum[2], snum[3]);
			ret[1] = char2channel(snum[4], snum[5]);
			ret[0] = char2channel(snum[6], snum[7]);
			ret[3] = char2channel(snum[8], snum[9]);
		}
	}

	return (ret[0] << 24) | (ret[1] << 16) | (ret[2] << 8) | ret[3];
}

}