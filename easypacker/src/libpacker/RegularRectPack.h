#ifndef _LIBPACKER_REGULAR_RECT_PACK_H_
#define _LIBPACKER_REGULAR_RECT_PACK_H_

#include <queue>
#include <set>

#include <wx/arrstr.h>

namespace libpacker
{

class RegularRectPack
{
public:
	RegularRectPack(const wxArrayString& files);
	~RegularRectPack();

	void Pack();

	void PrintStatics() const;

private:
	void LoadData(const wxArrayString& files);

	void PackPowerOfTwo();
	void PackNotPowerOfTwo();

private:
	struct Rect
	{
		Rect() : w(-1), h(-1) {}

		int x, y;
		int w, h;
		wxString file;

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

		int w, h;
		std::vector<Combine> children;

		// pos in its father
		int x, y;

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

private:
	std::set<CombineArray*, CombineArrayCmp> m_data;

	int m_ori_count;

}; // RegularRectPack

}

#endif // _LIBPACKER_REGULAR_RECT_PACK_H_