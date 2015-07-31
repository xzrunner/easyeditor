#include "RegularRectCut.h"
#include "PixelAreaLUT.h"
#include "RectPostProcessor.h"

#include "RegularRectMerge.h"

namespace eimage
{

static const int EDGE_LIST[] = {512, 256, 128, 64, 32, 16, 8, 4, 2};
static const int EDGE_COUNT = 7;

//static const float AREA_LIMIT_TIMES = 1.7f;
// static const float AREA_LIMIT_TIMES = 1.8f;
static const float AREA_LIMIT_TIMES = 2.1f;

static const float TRY_MAX_COUNT = 3;
static const float TRY_FACTOR = 0.95f;
//static const int TRY_MIN_EDGE = 4;
static const int TRY_MIN_EDGE = 1;
static const int TRY_MIN_LIMIT = 2;

RegularRectCut::RegularRectCut(const d2d::Image& image)
{
	LoadPixels(image.GetPixelData(), image.GetOriginWidth(), image.GetOriginHeight());

	m_area_array = new PixelAreaLUT(m_pixels, m_width, m_height, true);
}

RegularRectCut::RegularRectCut(const uint8_t* pixels, int width, int height)
{
	LoadPixels(pixels, width, height);

	m_area_array = new PixelAreaLUT(m_pixels, m_width, m_height, true);
}

RegularRectCut::~RegularRectCut()
{
	delete[] m_pixels;
	delete m_area_array;
}

void RegularRectCut::AutoCut()
{
	float limit = m_density * AREA_LIMIT_TIMES;
	int count = 0;
	while (m_left_area > 0 /*&& count < TRY_MAX_COUNT*/) {
		AutoCutWithLimit(limit);
		limit *= TRY_FACTOR;
		++count;
	}

// 	// cut others by the smaller one
// 	while (m_left_area > 0) {
// 		int x, y;
// 		int area = CalBestRectPos(TRY_MIN_EDGE, TRY_MIN_EDGE, x, y);
// 		if (area <= TRY_MIN_LIMIT) {
// 			break;
// 		}
// 		m_area_array->CutByRect(x, y, TRY_MIN_EDGE, TRY_MIN_EDGE, m_left_area);
// 
// 		m_result.push_back(Rect(x, y, TRY_MIN_EDGE, TRY_MIN_EDGE));
// 	}

	PoseProcessResult();
}

int RegularRectCut::GetUseArea() const
{
	int area = 0;
	for (int i = 0, n = m_result.size(); i < n; ++i) {
		area += m_result[i].w * m_result[i].h;
	}
	return area;
}

void RegularRectCut::LoadPixels(const uint8_t* pixels, int width, int height)
{
	m_width = width;
	m_height = height;
	m_left_area = 0;

	int sz = m_width * m_height;
	m_pixels = new bool[sz];
	memset(m_pixels, 0, sz * sizeof(bool));
	int ptr = 3;
	for (int i = 0; i < sz; ++i) {
		if (pixels[ptr]) { 
			m_pixels[i] = true;
			++m_left_area;
		}
		ptr += 4;
	}

	m_density = (float)m_left_area / (m_width*m_height);
}

void RegularRectCut::AutoCutWithLimit(float limit)
{
	const float min_limit = limit * 0.5f;
	for (int i = 0; i < EDGE_COUNT && m_left_area > 0; ++i)
	{
		int edge = EDGE_LIST[i];
		if (edge > m_width && edge > m_height ||
			(edge >> 1) > m_width ||
			(edge >> 1) > m_height) {
				continue;
		}
		for (int j = 0; j < 3; ++j)
		{
			int hw, hh;
			hw = hh = (edge >> 1);

			int w, h;
			w = h = edge;
			if (j == 1) {
				h = hh;
			} else if (j == 2) {
				w = hw;
			}

			if (w < TRY_MIN_EDGE || h < TRY_MIN_EDGE) {
				continue;
			}

			int area_limit = (int)(w*h*limit);
			int area_limit_min = (int)(hw*hh*min_limit);

			bool success = false;
			do {
				int x, y;
				int area = CalBestRectPos(w, h, x, y);
				success = area > area_limit;
				if (success) {
					if (j == 0) {
						if (m_area_array->GetRectArea(x, y, hw, hh) < area_limit_min ||
							m_area_array->GetRectArea(x, y+hh, hw, hh) < area_limit_min ||
							m_area_array->GetRectArea(x+hw, y+hh, hw, hh) < area_limit_min ||
							m_area_array->GetRectArea(x+hw, y, hw, hh) < area_limit_min) {
							success = false;
						}
					} else if (j == 1) {
						if (m_area_array->GetRectArea(x, y, hw, hh) < area_limit_min ||
							m_area_array->GetRectArea(x+hw, y, hw, hh) < area_limit_min) {
							success = false;
						}
					} else {
						if (m_area_array->GetRectArea(x, y, hw, hh) < area_limit_min ||
							m_area_array->GetRectArea(x, y+hh, hw, hh) < area_limit_min) {
							success = false;
						}						
					}
				}

				if (success) {
					m_area_array->CutByRect(x, y, w, h, m_left_area);
					m_result.push_back(Rect(x, y, w, h));
				}
			} while(success && m_left_area > 0);	
		}
	}
}

int RegularRectCut::CalBestRectPos(int w, int h, int& ret_x, int& ret_y)
{
	int max_area = -1;
	int max_x, max_y;
	for (int y = 0, up = m_height - h; y <= up; y+=1) {
		for (int x = 0, right = m_width - w; x <= right; x+=1) {
			int area = m_area_array->GetRectArea(x, y, w, h);
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

void RegularRectCut::PoseProcessResult()
{
 	RectPostProcessor processor(m_result, m_width, m_height, m_pixels);

	processor.Condense();

//  	processor.MoveToNoCover();
//  	processor.RemoveUnnecessary();
// 	while (true) {
// 		bool dirty = processor.Merge();
// 		if (!dirty) {
// 			break;
// 		} else {
// 			processor.Align();
// 		}
// 	}

 	std::vector<Rect> result;

 	processor.LoadResult(result);
	RegularRectMerge merge(m_width, m_height, result, processor);
	merge.Merge();

	processor.LoadResult(result);
	m_result = result;
}

}