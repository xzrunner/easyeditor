#include "GenRegularRectBinary.h"

#include <drag2d.h>

#include <json/json.h>
#include <fstream>
#include <algorithm>

namespace libtexpacker
{

GenRegularRectBinary::GenRegularRectBinary(const wxString& json_file, 
										   const wxString& img_id_file)
	: m_filepath(json_file)
{
	LoadRegularRectPackFile(json_file, img_id_file);
}

GenRegularRectBinary::~GenRegularRectBinary()
{
	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		for (int j = 0, m = pic->parts.size(); j < m; ++j) {
			delete pic->parts[j];
		}
		delete pic;
	}
}

void GenRegularRectBinary::PackToBinary() const
{
	wxString dir = d2d::FileHelper::GetFileDir(m_filepath);
	wxString filepath = dir + "\\pack.rrp";

	std::ofstream fout(filepath.mb_str(), std::ios::binary);
	int pic_sz = m_pics.size();
	fout.write(reinterpret_cast<const char*>(&pic_sz), sizeof(int32_t));
	for (int i = 0, n = m_pics.size(); i < n; ++i)
	{
		Picture* pic = m_pics[i];
		fout.write(reinterpret_cast<const char*>(&pic->id), sizeof(int16_t));
		int part_sz = pic->parts.size();
		fout.write(reinterpret_cast<const char*>(&part_sz), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&pic->w), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&pic->h), sizeof(int16_t));
		for (int j = 0; j < part_sz; ++j)
		{
			Part* part = pic->parts[j];
	
			int16_t si = part->src.x;
			fout.write(reinterpret_cast<const char*>(&si), sizeof(int16_t));
			si = part->src.y;
			fout.write(reinterpret_cast<const char*>(&si), sizeof(int16_t));
			si = part->dst.x;
			fout.write(reinterpret_cast<const char*>(&si), sizeof(int16_t));
			si = part->dst.y;
			fout.write(reinterpret_cast<const char*>(&si), sizeof(int16_t));
			int16_t w = part->src.w, h = part->src.h;
			if (part->src.w != part->dst.w) {
				assert(part->src.w == part->dst.h);
				w = -w; h = -h;
			}
			fout.write(reinterpret_cast<const char*>(&w), sizeof(int16_t));
			fout.write(reinterpret_cast<const char*>(&h), sizeof(int16_t));
		}
	}
	fout.close();
}

void GenRegularRectBinary::LoadRegularRectPackFile(const wxString& json_file, 
												   const wxString& img_id_file)
{
	// load file
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(json_file.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	// parser json
	std::vector<Part*> parts;
	int i = 0;
	Json::Value val = value["parts"][i++];
	while (!val.isNull()) {
		wxString path = val["filepath"].asString();
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
		if (p->filepath.Contains(pic->path)) {
			pic->parts.push_back(p);
		} else {
			m_pics.push_back(pic);
			pic = new Picture;
			size_t ptr_s = p->filepath.find_last_of("\\")+1,
				ptr_e = p->filepath.find_first_of("#");
			pic->path = p->filepath.substr(ptr_s, ptr_e - ptr_s);
			pic->parts.push_back(p);
		}
	}
	m_pics.push_back(pic);

	// set picture region
	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		d2d::Rect r;
		for (int j = 0, m = pic->parts.size(); j < m; ++j) {
			const Rect& r_src = pic->parts[j]->src;
			r.Combine(d2d::Vector(r_src.x, r_src.y));
			r.Combine(d2d::Vector(r_src.x+r_src.w, r_src.y+r_src.h));
			pic->w = r.Width();
			pic->h = r.Height();
		}
	}

	// set picture id
	std::map<std::string, int> image_map;
	std::ifstream fin_id_file(img_id_file.mb_str());
	std::string line;
	int id = 1;
	while (std::getline(fin_id_file, line)) {
		wxString key = line.substr(0, line.find_last_of('.'));
		key.Replace("\\", "_");
		image_map.insert(std::make_pair(key, id++));
	}
	fin_id_file.close();

	for (int i = 0, n = m_pics.size(); i < n; ++i) {
		Picture* pic = m_pics[i];
		std::map<std::string, int>::iterator itr 
			= image_map.find(pic->path.Lower().ToStdString());
		if (itr == image_map.end()) {
			throw d2d::Exception("Cannot find image %s in %s\n", 
				pic->path, img_id_file);
		}
		pic->id = itr->second;
	}
}

}