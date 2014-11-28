#include "RegularRectCut.h"

namespace eimage
{

static const int EDGE_LIST[] = {128, 64, 32, 16, 8};
static const int EDGE_COUNT = 5;

static const float AREA_LIMIT = 0.5f;

RegularRectCut::RegularRectCut(const d2d::Image& image)
{
	LoadPixels(image);
	BuildEdgeTable();
}

RegularRectCut::~RegularRectCut()
{
	delete[] m_pixels;
	delete m_hor_table;
}

void RegularRectCut::AutoCut()
{
	for (int i = 0; i < EDGE_COUNT && m_left_area > 0; ++i)
	{
		bool success = false;
		do {
			int edge = EDGE_LIST[i];
			int limit = (int)(edge*edge*AREA_LIMIT);
			int x, y;
			success = AutoCut(edge, limit, x, y);
			if (success) {
				m_hor_table->CutByRect(x, y, edge, m_left_area);
				m_result.push_back(Rect(x, y, edge));
			}
		} while(success && m_left_area > 0);
	}
}

void RegularRectCut::LoadPixels(const d2d::Image& image)
{
	m_width = image.originWidth();
	m_height = image.originHeight();
	const unsigned char* data = image.getPixelData();
	m_left_area = 0;

	int sz = m_width * m_height;
	m_pixels = new bool[sz];
	memset(m_pixels, 0, sz * sizeof(bool));
	int ptr = 3;
	for (int i = 0; i < sz; ++i) {
		if (data[ptr]) { 
			m_pixels[i] = true;
			++m_left_area;
		}
		ptr += 4;
	}
}

void RegularRectCut::BuildEdgeTable()
{
	m_hor_table = new EdgeTable(m_pixels, m_width, m_height);
}

bool RegularRectCut::AutoCut(int edge, int limit, int& ret_x, int& ret_y)
{
	for (int y = 0, up = m_height - edge; y < up; ++y) {
		for (int x = 0, right = m_width - edge; x < right; ++x) {
			int area = m_hor_table->GetRectArea(x, y, edge);
			if (area > limit) {
				ret_x = x;
				ret_y = y;
				return true;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// class RegularRectCut::EdgeTable
//////////////////////////////////////////////////////////////////////////

RegularRectCut::EdgeTable::
EdgeTable(const bool* pixels, int width, int height)
{
	Load(pixels, width, height);
}

int RegularRectCut::EdgeTable::
GetRectArea(int x, int y, int edge) const
{
	int area = 0;
	std::map<int, std::map<int, int> >::const_iterator 
		itr_y_start = m_lines.lower_bound(y),
		itr_y_end = m_lines.upper_bound(y+edge),
		itr;
	for (itr = itr_y_start; itr != itr_y_end; ++itr)
	{
		const std::map<int, int>& line = itr->second;
		std::map<int, int>::const_iterator itr_world = line.begin();
		for ( ; itr_world != line.end(); ++itr_world) {
			int start = itr_world->first,
				len = itr_world->second;
			int end = start + len - 1;
			if (start < x) {
				if (end < x) {
					continue;
				} else if (end >= x && end < x+edge) {
					area += end-x+1;
				} else {
					area += edge;
					break;
				}
			} else if (start >= x && start < x + edge) {
				if (end < x+edge) {
					area += end-start+1;
				} else {
					area += x+edge-start+1;
					break;
				}
			} else {
				break;
			}
		}
	}	
	return area;
}

void RegularRectCut::EdgeTable::
CutByRect(int x, int y, int edge, int& left_area)
{
 	std::map<int, std::map<int, int> >::iterator 
 		itr_y_start = m_lines.lower_bound(y),
 		itr_y_end = m_lines.upper_bound(y+edge),
 		itr;
 	for (itr = itr_y_start; itr != itr_y_end; ++itr)
 	{
 		std::map<int, int>& line = itr->second;
 		std::map<int, int>::iterator itr_world = line.begin();
 		for ( ; itr_world != line.end();) {
			bool erase = false;
 			int start = itr_world->first,
 				len = itr_world->second;
 			int end = start + len - 1;
 			if (start < x) {
 				if (end < x) {
					++itr_world;
 					continue;
 				} else if (end >= x && end < x+edge) {
 					itr_world->second -= end-x+1;
					if (itr_world->second == 0) {
						itr_world = line.erase(itr_world);
					} else {
						++itr_world;
					}
					left_area -= end-x+1;
 				} else {
 					itr_world->second -= end-x+1;
					if (itr_world->second == 0) {
						itr_world = line.erase(itr_world);
					} else {
						++itr_world;
					}
 					line.insert(std::make_pair(end+1, len-itr_world->second-edge));
					left_area -= edge;
 					break;
 				}
 			} else if (start >= x && start < x + edge) {
 				if (end < x+edge) {
 					itr_world = line.erase(itr_world);
					left_area -= end-start+1;
 				} else {
					itr_world = line.erase(itr_world);
					line.insert(std::make_pair(end+1, x+edge-start+1));
					left_area -= x+edge-start+1;
 				}
 			} else {
 				break;
 			}
 		}
 	}
}

void RegularRectCut::EdgeTable::
Load(const bool* pixels, int width, int height)
{
	for (int y = 0; y < height; ++y) {
		std::map<int, int> line;

		int id, len = 0;
		for (int x = 0; x < width; ++x) {
			if (pixels[width*y+x]) {
				if (len == 0) {
					id = x;
				}
				++len;
			} else {
				if (len != 0) {
					line.insert(std::make_pair(id, len));
					len = 0;
				}
			}
		}

		if (!line.empty()) {
			m_lines.insert(std::make_pair(y, line));
		}
	}
}

}