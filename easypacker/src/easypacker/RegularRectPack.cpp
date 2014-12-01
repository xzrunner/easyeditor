#include "RegularRectPack.h"

#include <wx/tokenzr.h>

namespace epacker
{

RegularRectPack::RegularRectPack(const wxArrayString& files)
{
	LoadData(files);

	int count_ori = GetCombineCount();
	Pack();
	wxString msg;
	msg.Printf("before: %d, after: %d", count_ori, GetCombineCount());

	int zz = 0;
}

RegularRectPack::~RegularRectPack()
{
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
	for ( ; itr != m_data.end(); ++itr) {
		delete (*itr);
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

bool IsPowerOfTwo(int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

void RegularRectPack::Pack()
{
	std::set<CombineArray*, CombineArrayCmp>::iterator itr = m_data.begin();
	for ( ; itr != m_data.end(); )
	{
		CombineArray* ca = *itr;
		// power of 2, w == h
		if (ca->w == ca->h && IsPowerOfTwo(ca->w))
		{
			int next_sz = ca->w << 1;
			// compose 4 to a larger one
			while (ca->Size() >= 4) {
				Combine cb(next_sz, next_sz);
				for (int i = 0; i < 4; ++i) {
					cb.Add(ca->Pop());
				}
				InsertToCombineArray(cb);
			}		
		}
		// power of 2, w == h * 2
		else if (ca->w == ca->h * 2 && IsPowerOfTwo(ca->w))
		{
			int next_sz = ca->w;
			// compose 2 to a larger one
			while (ca->Size() >= 2) {
				Combine cb(next_sz, next_sz);
				for (int i = 0; i < 2; ++i) {
					cb.Add(ca->Pop());
				}
				InsertToCombineArray(cb);
			}
		}
		// power of 2, h == w * 2
		else if (ca->h == ca->w * 2 && IsPowerOfTwo(ca->w))
		{
			int next_sz = ca->h;
			// compose 2 to a larger one
			while (ca->Size() >= 2) {
				Combine cb(next_sz, next_sz);
				for (int i = 0; i < 2; ++i) {
					cb.Add(ca->Pop());
				}
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

}