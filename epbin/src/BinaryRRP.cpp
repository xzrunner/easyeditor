#include "BinaryRRP.h"
#include "Exception.h"
#include "tools.h"
#include "Lzma.h"
#include "ImageIDer.h"

#include <json/json.h>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <sstream>

//#include <dtex_rrp.h>

namespace epbin
{

static const uint8_t TYPE = 11;

BinaryRRP::BinaryRRP(const std::string& json_file, const std::string& img_id_file)
{
	LoadMulti(json_file, img_id_file);
}

BinaryRRP::~BinaryRRP()
{
	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		for (int j = 0, m = pic->parts.size(); j < m; ++j) {
			delete pic->parts[j];
		}
		delete pic;
	}
}

void BinaryRRP::Pack(const std::string& outfile, bool compress) const
{
// 	int32_t pic_sz = m_pics.size();
// 
// 	// data sz
// 	size_t data_sz = 0;
// 	data_sz += sizeof(int32_t);
// 	for (int i = 0; i < pic_sz; ++i) {
// 		data_sz += m_pics[i]->Size();
// 	}
// 
// 	// fill buffer
// 	uint8_t* data_buf = new uint8_t[data_sz];
// 	uint8_t* ptr_data = data_buf;
// 	pack2mem(pic_sz, &ptr_data);
// 	for (int i = 0; i < pic_sz; ++i) {
// 		m_pics[i]->Store(&ptr_data);
// 	}
// 	assert(ptr_data - data_buf == data_sz);
// 
// 	// final
// 	size_t sz = data_sz + sizeof(uint8_t) + sizeof(uint32_t);
// 	uint8_t* buf = new uint8_t[sz];
// 	uint8_t* ptr = buf;
// 	pack2mem(TYPE, &ptr);
// 
// 	int cap = dtex_rrp_size(data_buf, data_sz);
// 	pack2mem(cap, &ptr);
// 
// 	memcpy(ptr, data_buf, data_sz);
// 	delete[] data_buf;
// 
// 	// write to file
// 	std::ofstream fout(outfile.c_str(), std::ios::binary);
// 	if (compress)
// 	{
// 		uint8_t* dst = NULL;
// 		size_t dst_sz;
// 		Lzma::Compress(&dst, &dst_sz, buf, sz);
// 
// 		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
// 		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
// 	}
// 	else
// 	{
// 		int _sz = -(int)sz;
// 		fout.write(reinterpret_cast<const char*>(&_sz), sizeof(int32_t));
// 		fout.write(reinterpret_cast<const char*>(buf), sz);
// 	}
// 	delete[] buf;
// 	fout.close();
}

void BinaryRRP::LoadMulti(const std::string& json_file, const std::string& img_id_file)
{
	// load parts
	int i = 1;
	while (true) {
		std::stringstream ss;
		ss << json_file << i << ".json";

		std::ifstream fin(ss.str().c_str(), std::ios::binary);
		if (fin.fail()) {
			break;
		}
		fin.close();

		LoadSingleParts(ss.str(), i - 1);

		++i;
	}

	// parts to pictures
	std::sort(m_parts.begin(), m_parts.end(), PartCmp());
	Picture* pic = new Picture;
	size_t ptr_s = m_parts[0]->filepath.find_last_of("\\")+1,
		ptr_e = m_parts[0]->filepath.find_first_of("#");
	pic->path = m_parts[0]->filepath.substr(ptr_s, ptr_e - ptr_s);
	pic->parts.push_back(m_parts[0]);
	for (int i = 1, n = m_parts.size(); i < n; ++i)
	{
		Part* p = m_parts[i];
		if (p->filepath.find(pic->path) != std::string::npos) {
			pic->parts.push_back(p);
		} else {
			m_pics.push_back(pic);
			pic = new Picture;
			size_t ptr_s = p->filepath.find_last_of("\\")+1,
				ptr_e = p->filepath.find_first_of("#");
			pic->path = p->filepath.substr(ptr_s, ptr_e - ptr_s);
			std::transform(pic->path.begin(), pic->path.end(), pic->path.begin(), ::tolower);
			pic->parts.push_back(p);
		}
	}
	m_pics.push_back(pic);

	// set picture region
	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		int xmin = INT_MAX, ymin = INT_MAX,
			xmax = -INT_MAX, ymax = -INT_MAX;
		for (int j = 0, m = pic->parts.size(); j < m; ++j) {
			const Rect& r = pic->parts[j]->src;
			if (r.x < xmin) xmin = r.x;
			if (r.x+r.w > xmax) xmax = r.x+r.w;
			if (r.y < ymin) ymin = r.y;
			if (r.y+r.h > ymax) ymax = r.y+r.h;
		}
		pic->w = xmax - xmin;
		pic->h = ymax - ymin;
	}

	// set picture id
	ImageIDer ider(img_id_file);
	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		std::string path = m_pics[i]->path;
		str_replace(path, "%", "\\");
		m_pics[i]->id = ider.Query(path);
	}
}

void BinaryRRP::LoadSingleParts(const std::string& json_file, int pic_idx)
{
	// load file
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(json_file.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	// parser json
	int height = value["height"].asUInt();
	int i = 0;
	Json::Value val = value["parts"][i++];
	while (!val.isNull()) {
		std::string path = val["filepath"].asString();
		Part* p = new Part;

		p->src.x = val["src"]["x"].asInt();
		p->src.y = val["src"]["y"].asInt();
		p->src.w = val["src"]["w"].asInt();
		p->src.h = val["src"]["h"].asInt();

		p->dst.x = val["dst"]["x"].asInt();
		p->dst.y = val["dst"]["y"].asInt();
		p->dst.w = val["dst"]["w"].asInt();
		p->dst.h = val["dst"]["h"].asInt();

		p->dst.y = height - (p->dst.y + p->dst.h);

		p->filepath = path;
		p->idx = pic_idx;

		m_parts.push_back(p);
		val = value["parts"][i++];
	}
}

//////////////////////////////////////////////////////////////////////////
// struct BinaryRRP::Part
//////////////////////////////////////////////////////////////////////////

// todo: w & h can use only 3 bytes
size_t BinaryRRP::Part::Size() const
{
	return sizeof(int16_t) * 6 + sizeof(int8_t);
}

void BinaryRRP::Part::Store(uint8_t** ptr)
{
	pack2mem(src.x, ptr);
	pack2mem(src.y, ptr);

	pack2mem(dst.x, ptr);
	pack2mem(dst.y, ptr);

	int16_t w = src.w, h = src.h;
	if (src.w != dst.w) {
		assert(src.w == dst.h);
		w = -w; h = -h;
	}
	pack2mem(w, ptr);
	pack2mem(h, ptr);

	pack2mem(idx, ptr);
}

//////////////////////////////////////////////////////////////////////////
// struct BinaryRRP::Picture
//////////////////////////////////////////////////////////////////////////

size_t BinaryRRP::Picture::Size() const 
{
	size_t sz = 0;
	sz += sizeof(int16_t) * 4;
	for (int i = 0, n = parts.size(); i < n; ++i) {
		sz += parts[i]->Size();
	}
	return sz;
}

void BinaryRRP::Picture::Store(uint8_t** ptr)
{
	pack2mem(id, ptr);

	pack2mem(w, ptr);
	pack2mem(h, ptr);

	int16_t sz = parts.size();
	pack2mem(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		parts[i]->Store(ptr);
	}
}

}