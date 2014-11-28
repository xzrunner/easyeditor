#ifndef _EASYIMAGE_REGULAR_RECT_CUT_H_
#define _EASYIMAGE_REGULAR_RECT_CUT_H_

#include <drag2d.h>

namespace eimage
{

class RegularRectCut
{
public:
	RegularRectCut(const d2d::Image& image);
	~RegularRectCut();

	void AutoCut();

private:
	void LoadPixels(const d2d::Image& image);
	void BuildEdgeTable();

	bool AutoCut(int edge, int limit, int& ret_x, int& ret_y);

private:
	class EdgeTable
	{
	public:
		EdgeTable(const bool* pixels, int width, int height);

		int GetRectArea(int x, int y, int edge) const;
		void CutByRect(int x, int y, int edge, int& left_area);

	private:
		void Load(const bool* pixels, int width, int height);

	private:
		std::map<int, std::map<int, int> > m_lines;		

	}; // EdgeTable

	struct Rect
	{
		Rect(int x, int y, int e) : x(x), y(y), edge(e) {}

		int x, y;
		int edge;
	};

private:
	bool* m_pixels;
	int m_width, m_height;
	int m_left_area;

	EdgeTable* m_hor_table;

	std::vector<Rect> m_result;

}; // RegularRectCut

}

#endif // _EASYIMAGE_REGULAR_RECT_CUT_H_