#include "RegularRectPack.h"
#include "MaxRectsBinaryPack2.h"

#include <ee/StringHelper.h>
#include <ee/std_functor.h>
#include <ee/math_common.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <json/json.h>

//#include <wx/tokenzr.h>
#include <iostream>

namespace etexpacker
{

RegularRectPack::RegularRectPack(const wxArrayString& files)
{
	LoadData(files);

	m_ori_count = GetCombineCount();
}

RegularRectPack::~RegularRectPack()
{
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
	for ( ; itr != m_data.end(); ++itr) {
		delete (*itr);
	}
}

void RegularRectPack::Pack()
{
	PackNotPowerOfTwo();
	PackPowerOfTwo();

	PrintStatics();

	PackWithMaxRectAlg();
}

void RegularRectPack::OutputToText(const std::string& filepath) const
{
	if (m_sizes.empty()) {
		return;
	}

	struct Part {
		RegularRectPack::Rect src;
		etexpacker::Rect dst;
	};

	struct Picture {
		std::vector<Part*> parts;
		Json::Value value;
	};

	std::vector<Picture*> pictures;
	for (int i = 0, n = m_sizes.size(); i < n; ++i) {
		Picture* pic = new Picture;
		pic->value["width"] = m_sizes[i].width;
		pic->value["height"] = m_sizes[i].height;
		pictures.push_back(pic);
	}

	std::vector<std::pair<RegularRectPack::Rect, etexpacker::Rect> >::const_iterator itr;
	for (itr = m_result.begin(); itr != m_result.end(); ++itr) {
		Part* p = new Part;
		p->src = itr->first;
		p->dst = itr->second;
		pictures[p->dst.tex_id]->parts.push_back(p);
	}

	for (int i = 0, n = pictures.size(); i < n; ++i)
	{
		Picture* pic = pictures[i];
		for (int j = 0, m = pic->parts.size(); j < m; ++j)
		{
			Part* part = pic->parts[j];

			Json::Value rect_val;

			rect_val["filepath"] = part->src.file;
			if (part->src.rot) {
				rect_val["src"]["w"] = part->src.h;
				rect_val["src"]["h"] = part->src.w;
			} else {
				rect_val["src"]["w"] = part->src.w;
				rect_val["src"]["h"] = part->src.h;
			}
			rect_val["src"]["x"] = part->src.x;
			rect_val["src"]["y"] = part->src.y;
			rect_val["dst"]["w"] = part->dst.width;
			rect_val["dst"]["h"] = part->dst.height;
			rect_val["dst"]["x"] = part->dst.x;
			rect_val["dst"]["y"] = part->dst.y;

			pic->value["parts"][j] = rect_val;
		}
	}

	for (int i = 0, n = pictures.size(); i < n; ++i)
	{
		std::string out_file = filepath + ee::StringHelper::ToString(i+1) + ".json";
 	 	Json::StyledStreamWriter writer;
 	 	std::locale::global(std::locale(""));
 	 	std::ofstream fout(out_file.c_str());
 	 	std::locale::global(std::locale("C"));	
 	 	writer.write(fout, pictures[i]->value);
	}

	for (int i = 0, n = pictures.size(); i < n; ++i) {
		Picture* pic = pictures[i];
		for_each(pic->parts.begin(), pic->parts.end(), ee::DeletePointerFunctor<Part>());
		delete pic;
	}
}

void RegularRectPack::PackPowerOfTwo()
{
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
	for ( ; itr != m_data.end(); )
	{
		CombineArray* ca = *itr;
		// power of 2, w == h
		if (ca->w == ca->h && ee::is_power_of_two(ca->w))
		{
			int next_sz = ca->w << 1;
			// compose 4 to a larger one
			while (ca->Size() >= 4) {
				Combine cb(next_sz, next_sz);
				cb.Add(ca->Pop().SetPos(0, 0));
				cb.Add(ca->Pop().SetPos(0, ca->h));
				cb.Add(ca->Pop().SetPos(ca->w, ca->h));
				cb.Add(ca->Pop().SetPos(ca->w, 0));
				InsertToCombineArray(cb);
			}		
		}
		// power of 2, w == h * 2
		else if (ca->w == ca->h * 2 && ee::is_power_of_two(ca->w))
		{
			int next_sz = ca->w;
			// compose 2 to a larger one
			while (ca->Size() >= 2) {
				Combine cb(next_sz, next_sz);
				cb.Add(ca->Pop().SetPos(0, 0));
				cb.Add(ca->Pop().SetPos(0, ca->h));
				InsertToCombineArray(cb);
			}
		}
		// power of 2, h == w * 2
		else if (ca->h == ca->w * 2 && ee::is_power_of_two(ca->w))
		{
			int next_sz = ca->h;
			// compose 2 to a larger one
			while (ca->Size() >= 2) {
				Combine cb(next_sz, next_sz);
				cb.Add(ca->Pop().SetPos(0, 0));
				cb.Add(ca->Pop().SetPos(ca->w, 0));
				InsertToCombineArray(cb);
			}
		}

		if (ca->Size() == 0) {
			delete ca;
			itr = m_data.erase(itr);
		} else {
			itr = ++itr;
		}
	}
}

void RegularRectPack::PackNotPowerOfTwo()
{
	// add h to po2 or add an unit(4)
	bool success;
	do {
		success = false;
		std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
		for ( ; itr != m_data.end(); )
		{
			CombineArray* ca = *itr;
			// before
			if (ca->Size() == 0) {
				delete ca;
				itr = m_data.erase(itr);
				continue;
			}

			if (!ee::is_power_of_two(ca->h)) 
			{
				while (ca->Size() >= 1) {
					int sub_h = ee::next_p2(ca->h) - ca->h;
					if (ComposeTwo(ca, ca->w, sub_h, false) ||
						ComposeTwo(ca, ca->w, 4, false)) {
						success = true;
					} else {
						break;
					}
				}
			}

			// after
			if (ca->Size() == 0) {
				delete ca;
				itr = m_data.erase(itr);
			} else {
				itr = ++itr;
			}
		}		
	} while (success);

	// add w for h is po2 and w < h
	do {
		success = false;
		std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
		for ( ; itr != m_data.end(); )
		{
			CombineArray* ca = *itr;

			// before
			if (ca->Size() == 0) {
				delete ca;
				itr = m_data.erase(itr);
				continue;
			}

			if (ee::is_power_of_two(ca->h) && ca->w < ca->h)
			{
				while (ca->Size() >= 1) {
					int sub_w = ca->h - ca->w;
					bool curr_success = false;
					for (int w = sub_w; w >= 4; w -= 4) {
						if (ComposeTwo(ca, w, ca->h, true)) {
							success = true;
							curr_success = true;
							break;
						}
					}
					if (!curr_success) {
						break;
					}
				}
			}

			// after
			if (ca->Size() == 0) {
				delete ca;
				itr = m_data.erase(itr);
			} else {
				itr = ++itr;
			}
		}		
	} while (success);
}

void RegularRectPack::PackWithMaxRectAlg()
{
	// prepare data
	std::vector<RectSize> rects;
	std::set<CombineArray*, CombineArrayCmp>::iterator itr;
	for (itr = m_data.begin(); itr != m_data.end(); ++itr)
	{
		CombineArray* ca = *itr;
		for (int i = 0; i < ca->Size(); ++i) {
			RectSize size(ca->w, ca->h);
			rects.push_back(size);
		}
	}

	std::vector<etexpacker::Rect> output;

	// pack
	MaxRectsBinaryPack2 packer;
	packer.Pack(PACK_SQUARE_MULTI_AUTO, 0, 2048, 0, rects, output);
	packer.GetSize(m_sizes);

	// parser result
	int idx = 0;
	for (itr = m_data.begin(); itr != m_data.end(); ++itr)
	{
		CombineArray* ca = *itr;
		while (!ca->combines.empty()) {
			Combine cb = ca->combines.front(); ca->combines.pop();
			const etexpacker::Rect& r = output[idx++];
			ParserPackResult(cb, r);
		}
	}
}

void RegularRectPack::PrintStatics() const
{
	std::cout << ee::StringHelper::Format("before: %d, after: %d \n", m_ori_count, GetCombineCount());
	std::set<CombineArray*, CombineArrayCmp>::const_iterator itr;
	for (itr = m_data.begin(); itr != m_data.end(); ++itr) {
		const CombineArray* ca = *itr;
		std::cout << ee::StringHelper::Format("w: %d, h: %d, count: %d\n", ca->w, ca->h, ca->Size());
	}
}

void RegularRectPack::LoadData(const wxArrayString& files)
{
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = files[i];
		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
			continue;
		}

		Rect r;
		r.file = filepath;

		std::vector<std::string> tokens;
		ee::StringHelper::Split(filepath, "#", tokens);
		ee::StringHelper::FromString(tokens[1], r.x);
		ee::StringHelper::FromString(tokens[2], r.y);
		ee::StringHelper::FromString(tokens[3], r.w);
		ee::StringHelper::FromString(tokens[4], r.h);
		if (r.w > r.h) {
			std::swap(r.w, r.h);
			r.rot = true;
		}

		InsertToCombineArray(Combine(r));
	}
}

void RegularRectPack::InsertToCombineArray(const Combine& c)
{
	assert(c.w > 0 && c.h > 0);

	CombineArray array(c.w, c.h);
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.find(&array);
	if (itr == m_data.end()) {
		CombineArray* new_list = new CombineArray(c.w, c.h);
		new_list->Push(c);
		m_data.insert(new_list);
	} else {
		(*itr)->Push(c);
	}
}

int RegularRectPack::GetCombineCount() const
{
	int count = 0;
	std::set<CombineArray*, CombineArrayCmp>::const_iterator itr;
	for (itr = m_data.begin(); itr != m_data.end(); ++itr) {
		count += (*itr)->Size();
	}
	return count;
}

bool RegularRectPack::ComposeTwo(CombineArray* ca, int width, int height, bool is_right_side)
{
	CombineArray array(width, height);
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.find(&array);
	if (itr == m_data.end()) {
		return false;
	}
	if (*itr == ca && ca->Size() < 2 || (*itr)->Size() < 1) {
		return false;
	}

	if (is_right_side) {
		assert(ca->h == height);
		Combine cb(ca->w+width, ca->h);
		cb.Add(ca->Pop().SetPos(0, 0));
		cb.Add((*itr)->Pop().SetPos(ca->w, 0));
		InsertToCombineArray(cb);
	} else {
		assert(ca->w == width);
		Combine cb(ca->w, ca->h+height);
		cb.Add(ca->Pop().SetPos(0, 0));
		cb.Add((*itr)->Pop().SetPos(0, ca->h));
		InsertToCombineArray(cb);
	}
	return true;
}

void RegularRectPack::ParserPackResult(const Combine& cb, const etexpacker::Rect& r)
{
	assert(cb.w == r.width && cb.h == r.height
		|| cb.h == r.width && cb.w == r.height);

	if (cb.IsLeaf()) {
		m_result.push_back(std::make_pair(cb.r, r));
	} else {
		for (int i = 0, n = cb.children.size(); i < n; ++i) {
			const Combine& child = cb.children[i];

			etexpacker::Rect cr;
			cr.tex_id = r.tex_id;
			if (cb.w <= r.width && cb.h <= r.height) {
				cr.x = r.x + child.x;
				cr.y = r.y + child.y;
				cr.width = child.w;
				cr.height = child.h;	
			} else {
				// clockwise rotate
				cr.x = r.x + child.y;
				cr.y = r.y + child.x;
				cr.width = child.h;
				cr.height = child.w;
			}

			ParserPackResult(child, cr);
		}
	}
}

}