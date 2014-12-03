#include "RegularRectPack.h"
#include "math.h"

#include <wx/tokenzr.h>
#include <iostream>

namespace libpacker
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
}

void RegularRectPack::PackPowerOfTwo()
{
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
	for ( ; itr != m_data.end(); )
	{
		CombineArray* ca = *itr;
		// power of 2, w == h
		if (ca->w == ca->h && is_power_of_two(ca->w))
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
		else if (ca->w == ca->h * 2 && is_power_of_two(ca->w))
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
		else if (ca->h == ca->w * 2 && is_power_of_two(ca->w))
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

			if (!is_power_of_two(ca->h)) 
			{
				while (ca->Size() >= 1) {
					int sub_h = next_p2(ca->h) - ca->h;
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

			if (is_power_of_two(ca->h) && ca->w < ca->h)
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

void RegularRectPack::PrintStatics() const
{
	wxString msg;
	msg.Printf("before: %d, after: %d \n", m_ori_count, GetCombineCount());
	std::cout << msg;

	std::set<CombineArray*, CombineArrayCmp>::const_iterator itr;
	for (itr = m_data.begin(); itr != m_data.end(); ++itr) {
		const CombineArray* ca = *itr;
		wxString msg;
		msg.Printf("w: %d, h: %d, count: %d\n", ca->w, ca->h, ca->Size());
		std::cout << msg;
	}
}

void RegularRectPack::LoadData(const wxArrayString& files)
{
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		const wxString& filepath = files[i];

		Rect r;
		r.file = filepath;

		wxStringTokenizer tkz(filepath, wxT("#"));
		int idx = 0;
		while (tkz.HasMoreTokens())
		{
			wxString token = tkz.GetNextToken();
			if (idx == 1) {
				r.x = wxAtoi(token);
			} else if (idx == 2) {
				r.y = wxAtoi(token);
			} else if (idx == 3) {
				r.w = wxAtoi(token);
			} else if (idx == 4) {
				r.h = wxAtoi(token);
			}
			++idx;
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

}