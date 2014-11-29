#ifndef _EASYIMAGE_REGULAR_RECT_CUT_H_
#define _EASYIMAGE_REGULAR_RECT_CUT_H_

#include <drag2d.h>

namespace eimage
{

class RegularRectCut
{
public:
	struct Rect
	{
		Rect(int x, int y, int e) : x(x), y(y), edge(e) {}

		int x, y;
		int edge;
	};

public:
	RegularRectCut(const d2d::Image& image);
	~RegularRectCut();

	void AutoCut();

	const std::vector<Rect>& GetResult() { return m_result; }

	// statics
	int GetLeftArea() const { return m_left_area; }
	int GetUseArea() const;

private:
	void LoadPixels(const d2d::Image& image);
	void BuildEdgeTable();

	void AutoCut(float limit);
	int AutoCut(int edge, int& ret_x, int& ret_y);

private:
	class EdgeTable
	{
	public:
		EdgeTable(const bool* pixels, int width, int height);

		int GetRectArea(int x, int y, int edge, int limit) const;
		void CutByRect(int x, int y, int edge, int& left_area);

	private:
		void Load(const bool* pixels, int width, int height);

	private:
		struct Line
		{
			Line() : area(0) {}

			int area;
			std::map<int, int> worlds;
		};

	private:
		std::map<int, Line> m_lines;		

	}; // EdgeTable

private:
	bool* m_pixels;
	int m_width, m_height;
	int m_left_area;

	float m_density;

	EdgeTable* m_hor_table;

	std::vector<Rect> m_result;

}; // RegularRectCut

}

#endif // _EASYIMAGE_REGULAR_RECT_CUT_H_