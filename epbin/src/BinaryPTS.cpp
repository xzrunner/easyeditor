#include "BinaryPTS.h"
#include "ImageIDer.h"
#include "tools.h"
#include "Lzma.h"

#include <math.h>
#include <json/json.h>
#include <fstream>
#include <assert.h>

#include <dtex_pts.h>

namespace epbin
{

static const uint8_t TYPE = 12;

BinaryPTS::BinaryPTS(const std::string& config_dir,
					 const std::vector<std::string>& json_file, 
					 const std::string& img_id_file)
{
	Load(config_dir, json_file, img_id_file);
}

BinaryPTS::~BinaryPTS()
{
	for (int i = 0, n = m_pts.size(); i < n; ++i) {
		delete m_pts[i];
	}
	m_pts.clear();
}

void BinaryPTS::Pack(const std::string& outfile, bool compress) const
{
	int16_t pts_sz = m_pts.size();

	// data sz
	size_t data_sz = 0;
	data_sz += sizeof(int16_t);
	for (int i = 0; i < pts_sz; ++i) {
		data_sz += m_pts[i]->Size();
	}

	// fill buffer
	uint8_t* data_buf = new uint8_t[data_sz];
	uint8_t* ptr_data = data_buf;
	memcpy(ptr_data, &pts_sz, sizeof(pts_sz));
	ptr_data += sizeof(pts_sz);
	for (int i = 0; i < pts_sz; ++i) {
		m_pts[i]->Store(&ptr_data);
	}
	assert(ptr_data - data_buf == data_sz);

	// final
	size_t sz = data_sz + sizeof(uint8_t) + sizeof(uint32_t);
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	memcpy(ptr, &TYPE, sizeof(uint8_t));
	ptr += sizeof(uint8_t);
 	int cap = dtex_pts_size(data_buf, data_sz);
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

static double _round(double number)
{
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

void BinaryPTS::Load(const std::string& source_folder,
					 const std::vector<std::string>& json_file, 
					 const std::string& img_id_file)
{
	for (int i = 0, n = json_file.size(); i < n; ++i) {
		const std::string& filepath = json_file[i];

		// load
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		// parser
		PTS* pts = new PTS();
		int j = 0;
		Json::Value strip_val = value["strips"][j++];
		while (!strip_val.isNull()) {
			std::vector<Point> strip;
			int k = 0;
			Json::Value xval = strip_val["x"][k],
						yval = strip_val["y"][k];
			++k;
			while (!xval.isNull() && !yval.isNull()) {
				Point p;
				p.x = _round(xval.asDouble());
				p.y = _round(yval.asDouble());
				strip.push_back(p);

				xval = strip_val["x"][k];
				yval = strip_val["y"][k];
				++k;
			}
			pts->strips.push_back(strip);

			strip_val = value["strips"][j++];
		}

		// set id
		std::string path_fixed = filepath;
		str_replace(path_fixed, source_folder + "\\", "");
		path_fixed = path_fixed.substr(0, path_fixed.find("_strip.json"));
		ImageIDer ider(img_id_file);
		pts->id = ider.Query(path_fixed);

		m_pts.push_back(pts);
	}
}

//////////////////////////////////////////////////////////////////////////
// struct BinaryRRP::PTS
//////////////////////////////////////////////////////////////////////////

size_t BinaryPTS::PTS::Size() const
{
	size_t sz = 0;
	sz += sizeof(int16_t) + sizeof(int16_t);
	for (int i = 0, n = strips.size(); i < n; ++i) {
		const std::vector<Point>& strip = strips[i];
		sz += sizeof(int16_t);
		sz += strip.size() * sizeof(int16_t) * 2;
	}
	return sz;
}

void BinaryPTS::PTS::Store(uint8_t** ptr)
{
	memcpy(*ptr, &id, sizeof(id));
	*ptr += sizeof(id);

	int16_t strips_sz = strips.size();
	memcpy(*ptr, &strips_sz, sizeof(strips_sz));
	*ptr += sizeof(strips_sz);

	for (int i = 0; i < strips_sz; ++i) {
		const std::vector<Point>& strip = strips[i];

		int16_t strip_sz = strip.size();
		memcpy(*ptr, &strip_sz, sizeof(strip_sz));
		*ptr += sizeof(strip_sz);

		for (int j = 0; j < strip_sz; ++j) {
			const Point& p = strip[j];
			memcpy(*ptr, &p.x, sizeof(p.x));
			*ptr += sizeof(p.x);
			memcpy(*ptr, &p.y, sizeof(p.y));
			*ptr += sizeof(p.y);
		}
	}
}

}