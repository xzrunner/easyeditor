#ifndef _LIBTEXPACKER_REGULAR_RECT_PACK_H_
#define _LIBTEXPACKER_REGULAR_RECT_PACK_H_

#include <queue>
#include <set>

#include <wx/arrstr.h>

#include "Rect.h"

namespace libtp
{

class RegularRectPack
{
public:
	RegularRectPack(const wxArrayString& files);
	~RegularRectPack();

	void Pack();

	void OutputToText(const wxString& filepath) const;

private:
	void PrintStatics() const;

	void LoadData(const wxArrayString& files);

	// special pack
	void PackPowerOfTwo();
	void PackNotPowerOfTwo();

	// normal pack
	void PackWithMaxRectAlg();

private:
	struct Rect
	{
		Rect() : w(-1), h(-1), rot(false) {}

		bool HasFilepath() const { return !file.IsEmpty(); }

		int x, y;
		int w, h;
		wxString file;

		bool rot;

	}; // Rect

	struct Combine
	{
		Combine(const Rect& r) 
			: w(r.w), h(r.h), r(r), x(0), y(0) {}
		Combine(int w, int h) : w(w), h(h), x(0), y(0) {}

		void Add(const Combine& c) {
			children.push_back(c);
		}

		Combine& SetPos(int x, int y) {
			this->x = x;
			this->y = y;
			return *this;
		}

		bool IsLeaf() const {
			return r.HasFilepath();
		}

		int w, h;
		std::vector<Combine> children;

		// pos in its father
		int x, y;

		// leaf data
		Rect r;

	}; // Combine

	struct CombineArray
	{
		CombineArray(int w, int h) : w(w), h(h) {}

		int Size() const { return combines.size(); }

		void Push(const Combine& c) {
			combines.push(c);
		}
		Combine Pop() {
			Combine c = combines.front();
			combines.pop();
			return c;
		}

		int w, h;
		std::queue<Combine> combines;

	}; // CombineArray

	struct CombineArrayCmp
	{
	public:
		bool operator () (const CombineArray* lhs, const CombineArray* rhs) const {
			return lhs->w < rhs->w 
				|| lhs->w == rhs->w && lhs->h < rhs->h;
		}

	}; // CombineArrayCmp

private:
	void InsertToCombineArray(const Combine& c);

	int GetCombineCount() const;

	bool ComposeTwo(CombineArray* ca, int width, int height, bool is_right_side);

	void ParserPackResult(const Combine& cb, const libtp::Rect& r);

private:
	std::set<CombineArray*, CombineArrayCmp> m_data;

	int m_ori_count;

	std::vector<std::pair<Rect, libtp::Rect> > m_result;

	std::vector<libtp::RectSize> m_sizes;

}; // RegularRectPack

}

#endif // _LIBTEXPACKER_REGULAR_RECT_PACK_H_