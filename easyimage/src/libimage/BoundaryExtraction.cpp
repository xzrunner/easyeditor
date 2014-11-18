#include "BoundaryExtraction.h"

#include <queue>

namespace eimage
{

BoundaryExtraction::BoundaryExtraction(const d2d::Image& image)
	: m_pixels(image.getPixelData())
	, m_width(image.originWidth())
	, m_height(image.originHeight())
{
}

void BoundaryExtraction::GetBoundary(std::vector<d2d::Vector>& border) const
{
	bool* flag = new bool[m_width * m_height];
	d2d::Vector first;
	first.setInvalid();
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			flag[m_width*y+x] = is_border;
			if (is_border) {
				first.set(x, y);
			}
		}
	}

	std::queue<d2d::Vector> buf;
	if (first.isValid()) {
		buf.push(first);
	}
	while (!buf.empty()) 
	{
		d2d::Vector pos = buf.front(); buf.pop();
		border.push_back(pos);
		flag[int(pos.y*m_width+pos.x)] = false;
		if (!IsPixelTransparente(flag, pos.x-1, pos.y-1)) {
			buf.push(d2d::Vector(pos.x-1, pos.y-1));
		} else if (!IsPixelTransparente(flag, pos.x, pos.y-1)) {
			buf.push(d2d::Vector(pos.x, pos.y-1));
		} else if (!IsPixelTransparente(flag, pos.x+1, pos.y-1)) {
			buf.push(d2d::Vector(pos.x+1, pos.y-1));
		} else if (!IsPixelTransparente(flag, pos.x-1, pos.y)) {
			buf.push(d2d::Vector(pos.x-1, pos.y));
		} else if (!IsPixelTransparente(flag, pos.x+1, pos.y)) {
			buf.push(d2d::Vector(pos.x+1, pos.y));
		} else if (!IsPixelTransparente(flag, pos.x-1, pos.y+1)) {
			buf.push(d2d::Vector(pos.x-1, pos.y+1));
		} else if (!IsPixelTransparente(flag, pos.x, pos.y+1)) {
			buf.push(d2d::Vector(pos.x, pos.y+1));
		} else if (!IsPixelTransparente(flag, pos.x+1, pos.y+1)) {
			buf.push(d2d::Vector(pos.x+1, pos.y+1));
		}
	}

	delete[] flag;
}

bool BoundaryExtraction::IsPixelBorder(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return false;
	}

	if (IsPixelTransparente(x, y)) {
		return false;
	}
	if (IsPixelTransparente(x-1, y-1)) return true;
	if (IsPixelTransparente(x  , y-1)) return true;
	if (IsPixelTransparente(x+1, y-1)) return true;
	if (IsPixelTransparente(x-1, y)) return true;
	if (IsPixelTransparente(x+1, y)) return true;
	if (IsPixelTransparente(x-1, y+1)) return true;
	if (IsPixelTransparente(x  , y+1)) return true;
	if (IsPixelTransparente(x+1, y+1)) return true;

	return false;
}

bool BoundaryExtraction::IsPixelTransparente(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}
	
	byte alpha = m_pixels[(y*m_width+x)*4+3];
	return alpha == 0;
}

bool BoundaryExtraction::IsPixelTransparente(bool* flag, int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}

	return !flag[y*m_width+x];
}

}