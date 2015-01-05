#include "BinaryRRP.h"
#include "Exception.h"
#include "typedef.h"
#include "tools.h"
#include "Lzma.h"

#include <json.h>
#include <assert.h>
#include <fstream>
#include <algorithm>

#include <dtex_rrp.h>

namespace epbin
{

static const uint8_t TYPE = 11;

BinaryRRP::BinaryRRP(const std::string& json_file, const std::string& img_id_file)
{
	Load(json_file, img_id_file);
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
	int32_t pic_sz = m_pics.size();

	// data sz
	size_t data_sz = 0;
	data_sz += sizeof(uint8_t);
	for (int i = 0; i < pic_sz; ++i) {
		data_sz += m_pics[i]->Size();
	}

	// fill buffer
	uint8_t* data_buf = new uint8_t[data_sz];
	uint8_t* ptr_data = data_buf;
	memcpy(ptr_data, &pic_sz, sizeof(pic_sz));
	ptr_data += sizeof(pic_sz);
	for (int i = 0; i < pic_sz; ++i) {
		m_pics[i]->Store(&ptr_data);
	}
	assert(ptr_data - data_buf == data_sz);

	// final
	size_t sz = data_sz + sizeof(uint8_t) + sizeof(uint32_t);
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	memcpy(ptr, &TYPE, sizeof(uint8_t));
	ptr += sizeof(uint8_t);
	int cap = dtex_rrp_size(data_buf, data_sz);
	memcpy(ptr, &cap, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	memcpy(ptr, data_buf, data_sz);
	delete[] data_buf;

	// write to file
	std::ofstream fout(outfile.c_str(), std::ios::binary);
	if (compress)
	{
		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	}
	else
	{
		int _sz = -(int)sz;
		fout.write(reinterpret_cast<const char*>(&_sz), sizeof(int32_t));
		fout.write(reinterpret_cast<const char*>(buf), sz);
	}
	delete[] buf;
	fout.close();
}

void BinaryRRP::Load(const std::string& json_file, const std::string& img_id_file)
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
	std::vector<Part*> parts;
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

		p->filepath = path;

		parts.push_back(p);
		val = value["parts"][i++];
	}
	if (parts.empty()) {
		return;
	}

	// part to pictures
	std::sort(parts.begin(), parts.end(), PartCmp());
	Picture* pic = new Picture;
	size_t ptr_s = parts[0]->filepath.find_last_of("\\")+1,
		ptr_e = parts[0]->filepath.find_first_of("#");
	pic->path = parts[0]->filepath.substr(ptr_s, ptr_e - ptr_s);
	pic->parts.push_back(parts[0]);
	for (int i = 1, n = parts.size(); i < n; ++i)
	{
		Part* p = parts[i];
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
	std::map<std::string, int> image_map;
	std::ifstream fin_id_file(img_id_file.c_str());
	std::string line;
	int id = 1;
	while (std::getline(fin_id_file, line)) {
		std::string key = line.substr(0, line.find_last_of('.'));
		str_replace(key, "\\", "_");
		image_map.insert(std::make_pair(key, id++));
	}
	fin_id_file.close();

	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		std::map<std::string, int>::iterator itr = image_map.find(pic->path);
		if (itr == image_map.end()) {
			throw Exception("Cannot find image %s in %s\n", pic->path, img_id_file);
		}
		pic->id = itr->second;
	}
}

//////////////////////////////////////////////////////////////////////////
// struct BinaryRRP::Part
//////////////////////////////////////////////////////////////////////////

size_t BinaryRRP::Part::Size() const
{
	return sizeof(int16_t) * 6;
}

void BinaryRRP::Part::Store(uint8_t** ptr)
{
	memcpy(*ptr, &src.x, sizeof(src.x));
	*ptr += sizeof(src.x);
	memcpy(*ptr, &src.y, sizeof(src.y));
	*ptr += sizeof(src.y);

	memcpy(*ptr, &dst.x, sizeof(dst.x));
	*ptr += sizeof(dst.x);
	memcpy(*ptr, &dst.y, sizeof(dst.y));
	*ptr += sizeof(dst.y);

	int16_t w = src.w, h = src.h;
	if (src.w != dst.w) {
		assert(src.w == dst.h);
		w = -w; h = -h;
	}
	memcpy(*ptr, &w, sizeof(w));
	*ptr += sizeof(w);
	memcpy(*ptr, &h, sizeof(h));
	*ptr += sizeof(h);
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
	memcpy(*ptr, &id, sizeof(id));
	*ptr += sizeof(id);

	memcpy(*ptr, &w, sizeof(w));
	*ptr += sizeof(w);
	memcpy(*ptr, &h, sizeof(h));
	*ptr += sizeof(h);

	int16_t sz = parts.size();
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);
	for (int i = 0; i < sz; ++i) {
		parts[i]->Store(ptr);
	}
}

}