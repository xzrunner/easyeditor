#include "RegularRectCut.h"

namespace eimage
{

static const int EDGE_LIST[] = {128, 64, 32, 16, 8};
static const int EDGE_COUNT = 5;

static const float AREA_LIMIT_TIMES = 1.5f;

static const float TRY_MAX_COUNT = 3;
static const float TRY_FACTOR = 0.8f;
static const int TRY_MIN_EDGE = 4;
static const int TRY_MIN_LIMIT = 2;

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
	float limit = m_density * AREA_LIMIT_TIMES;
	int count = 0;
	while (m_left_area > 0 && count < TRY_MAX_COUNT) {
		AutoCut(limit);
		limit *= TRY_FACTOR;
		++count;
	}

	// cut others by the smaller one
	while (m_left_area > 0) {
		int x, y;
		int area = AutoCut(TRY_MIN_EDGE, x, y);
		if (area <= TRY_MIN_LIMIT) {
			break;
		}
		m_hor_table->CutByRect(x, y, TRY_MIN_EDGE, m_left_area);
		m_result.push_back(Rect(x, y, TRY_MIN_EDGE));
	}
}

int RegularRectCut::GetUseArea() const
{
	int area = 0;
	for (int i = 0, n = m_result.size(); i < n; ++i) {
		area += m_result[i].edge * m_result[i].edge;
	}
	return area;
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

	m_density = (float)m_left_area / (m_width*m_height);
}

void RegularRectCut::BuildEdgeTable()
{
	m_hor_table = new EdgeTable(m_pixels, m_width, m_height);
}

void RegularRectCut::AutoCut(float limit)
{
	for (int i = 0; i < EDGE_COUNT && m_left_area > 0; ++i)
	{
		bool success = false;
		do {
			int edge = EDGE_LIST[i];
			int x, y;
			int area = AutoCut(edge, x, y);
			success = area > (int)(edge*edge*limit);
			if (success) {
				m_hor_table->CutByRect(x, y, edge, m_left_area);
				m_result.push_back(Rect(x, y, edge));
			}
		} while(success && m_left_area > 0);
	}
}

int RegularRectCut::AutoCut(int edge, int& ret_x, int& ret_y)
{
	int max_area = -1;
	int max_x, max_y;
	for (int y = 0, up = m_height - edge; y <= up; y+=1) {
		for (int x = 0, right = m_width - edge; x <= right; x+=1) {
			int area = m_hor_table->GetRectArea(x, y, edge, max_area);
			if (area > max_area) {
				max_area = area;
				max_x = x;
				max_y = y;
			}
		}
	}

	if (max_area < 0) {
		return 0;	
	} else {
		ret_x = max_x;
		ret_y = max_y;
		return max_area;
	}
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
GetRectArea(int x, int y, int edge, int limit) const
{
	int area = 0;
	std::map<int, Line>::const_iterator 
		itr_y_start = m_lines.lower_bound(y),
		itr_y_end = m_lines.lower_bound(y+edge),
		itr;

	int raw_area = 0;
	for (itr = itr_y_start; itr != itr_y_end; ++itr) {
		raw_area += itr->second.area;
	}
	if (raw_area <= limit) {
		return 0;
	}

	for (itr = itr_y_start; itr != itr_y_end; ++itr)
	{
		const Line& line = itr->second;
		std::map<int, int>::const_iterator itr_world = line.worlds.begin();
		for ( ; itr_world != line.worlds.end(); ++itr_world) {
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
					area += x+edge-start;
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
 	std::map<int, Line>::iterator 
 		itr_y_start = m_lines.lower_bound(y),
 		itr_y_end = m_lines.upper_bound(y+edge-1),
 		itr;
 	for (itr = itr_y_start; itr != itr_y_end; )
 	{
 		Line& line = itr->second;
 		std::map<int, int>::iterator itr_world = line.worlds.begin();
 		for ( ; itr_world != line.worlds.end();) {
			bool erase = false;
 			int start = itr_world->first,
 				len = itr_world->second;
 			int end = start + len - 1;
 			if (start < x) {
 				if (end < x) {
					++itr_world;
 					continue;
 				} else if (end >= x && end < x+edge) {
					int area = end-x+1;
 					itr_world->second -= area;
					line.area -= area;
					left_area -= area;
					if (itr_world->second == 0) {
						itr_world = line.worlds.erase(itr_world);
					} else {
						++itr_world;
					}
 				} else {
					left_area -= edge;
					line.area -= edge;
					itr_world->second -= end-x+1;
					line.worlds.insert(std::make_pair(x+edge, end-(x+edge)+1));
					break;
 				}
 			} else if (start >= x && start < x + edge) {
 				if (end < x+edge) {
					int area = end-start+1;
					line.area -= area;
					left_area -= area;
 					itr_world = line.worlds.erase(itr_world);
 				} else {
					int area = x+edge-start;
					line.area -= area;
					left_area -= area;

					line.worlds.erase(itr_world);
					line.worlds.insert(std::make_pair(x+edge, end-(x+edge)+1));

					break;
 				}
 			} else {
 				break;
 			}
 		}

		// debug
		if (line.area != 0) {
			int a = 0;
			std::map<int, int>::iterator itr_world = line.worlds.begin();
			for ( ; itr_world != line.worlds.end(); ++itr_world) {
				a += itr_world->second;
			}
			assert(a == line.area);
		}

		if (line.area == 0) {
			itr = m_lines.erase(itr);
		} else {
			++itr;
		}
 	}

	// debug
	int a = 0;
	std::map<int, Line>::const_iterator itr_d = m_lines.begin();
	for ( ; itr_d != m_lines.end(); ++itr_d) {
		a += itr_d->second.area;
	}
	assert(a == left_area);
}

void RegularRectCut::EdgeTable::
Load(const bool* pixels, int width, int height)
{
	for (int y = 0; y < height; ++y) {
		Line line;

		int id, len = 0;
		for (int x = 0; x < width; ++x) {
			if (pixels[width*y+x]) {
				if (len == 0) {
					id = x;
				}
				++line.area;
				++len;
			}

			if (!pixels[width*y+x] || x == width-1) {
				if (len != 0) {
					line.worlds.insert(std::make_pair(id, len));
					len = 0;
				}
			}
		}

		if (!line.worlds.empty()) {
			m_lines.insert(std::make_pair(y, line));
		}
	}
}

}