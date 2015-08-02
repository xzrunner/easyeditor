#ifndef _EASYIMAGE_REGULAR_RECT_CONDENSE_H_
#define _EASYIMAGE_REGULAR_RECT_CONDENSE_H_

#include <drag2d.h>

#include "Rect.h"

namespace eimage
{

class RegularRectCondense
{
public:
	RegularRectCondense(const std::vector<Rect>& rects, int width, int height, bool* ori_pixels);
	~RegularRectCondense();

	void Condense(const Rect& r);

	void Remove(const Rect& r);
	void Insert(const Rect& r);

	void GetSortedRects(std::vector<Rect>& rects) const;

private:
	void LoadPixels(const std::vector<Rect>& rects, bool* ori_pixels);

private:
	class Pixel;

	struct Grid
	{
		Grid(const Rect& r) : r(r) {}
		Rect r;		
	}; // Grid

	class AreaCmp
	{
	public:
		bool operator () (const Grid* g0, const Grid* g1) const;
	}; // AreaCmp

	class Pixel
	{
	public:
		Pixel(bool has_data) : m_has_data(has_data) {}

		void Insert(Grid* g);
		void Remove(Grid* g);

		bool IsSingleCoverd() const {
			return m_grids.size() == 1;
		}
		bool HasData() const {
			return m_has_data;
		}

		Grid* QueryGrid(const Rect& r) const;

	private:
		std::vector<Grid*> m_grids;

		const bool m_has_data;

	}; // Pixel

private:
	enum Direction
	{
		e_left,
		e_right,
		e_down,
		e_up
	};

	void CondenseEmpty(Grid* g);
	bool IsPixelImmoveable(int x, int y) const;

	void CondenseCovered(const Rect& r);
	bool CondenseCovered(Grid* s, Grid* l);

	void RemoveSize(Grid* g, Direction dir, int len);

	void RemovePixelGrid(const Rect& r, Grid* g);
	void InsertPixelGrid(const Rect& r, Grid* g);

	bool IsRectIntersect(const Rect& r0, const Rect& r1) const;

private:
	int m_width, m_height;
	Pixel** m_pixels;

	std::vector<Grid*> m_grids;

}; // RegularRectCondense

}

#endif // _EASYIMAGE_REGULAR_RECT_CONDENSE_H_