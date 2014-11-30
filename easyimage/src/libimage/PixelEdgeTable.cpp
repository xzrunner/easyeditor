#include "PixelEdgeTable.h"

namespace eimage
{

PixelEdgeTable::PixelEdgeTable(const bool* pixels, int width, int height)
{
	Load(pixels, width, height);
}

int PixelEdgeTable::GetRectArea(int x, int y, int w, int h, int limit) const
{
	int area = 0;
	std::map<int, Line>::const_iterator 
		itr_y_start = m_lines.lower_bound(y),
		itr_y_end = m_lines.lower_bound(y+h),
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
				} else if (end >= x && end < x + w) {
					area += end-x+1;
				} else {
					area += w;
					break;
				}
			} else if (start >= x && start < x + w) {
				if (end < x + w) {
					area += end-start+1;
				} else {
					area += x+w-start;
					break;
				}
			} else {
				break;
			}
		}
	}	
	return area;
}

void PixelEdgeTable::CutByRect(int x, int y, int w, int h, int& left_area)
{
 	std::map<int, Line>::iterator 
 		itr_y_start = m_lines.lower_bound(y),
 		itr_y_end = m_lines.upper_bound(y+h-1),
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
 				} else if (end >= x && end < x+w) {
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
					left_area -= w;
					line.area -= w;
					itr_world->second -= end-x+1;
					line.worlds.insert(std::make_pair(x+w, end-(x+w)+1));
					break;
 				}
 			} else if (start >= x && start < x + w) {
 				if (end < x+w) {
					int area = end-start+1;
					line.area -= area;
					left_area -= area;
 					itr_world = line.worlds.erase(itr_world);
 				} else {
					int area = x+w-start;
					line.area -= area;
					left_area -= area;

					line.worlds.erase(itr_world);
					line.worlds.insert(std::make_pair(x+w, end-(x+w)+1));

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

void PixelEdgeTable::Load(const bool* pixels, int width, int height)
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