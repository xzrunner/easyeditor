//#include "SelectPixelsOP.h"
//#include "NullEditOP.h"
//#include "DrawRectangleOP.h"
//#include "DrawPolylineOP.h"
//
//#include "common/tools.h"
//#include "common/Math.h"
//#include "view/EditPanel.h"
//
//#include <queue>
//
//namespace d2d
//{
//
//SelectPixelsOP::SelectPixelsOP(EditPanel* editPanel, RawPixels* pixels, e_Mode mode, 
//							   RawPixels::PixelBuf& result, bool isSelect, int magicMarkerDis/* = 0*/)
//	: ZoomViewOP(editPanel, false), m_result(result)
//{
//	m_pixels = pixels;
//	m_isSelect = isSelect;
//	m_mode = mode;
//	m_magicMarkerDis = magicMarkerDis;
//	switch (m_mode)
//	{
//	case RectSelect:
//		m_drawOP = new DrawRectangleOP(editPanel);
//		break;
//	case PointSelect:
//		m_drawOP = new NullEditOP(editPanel);
//		break;
//	case PolylineSelect:
//		m_drawOP = new DrawPolylineOP(editPanel, true);
//		break;
//	default:
//		m_drawOP = NULL;
//		assert(0);
//	}
//}
//
//SelectPixelsOP::~SelectPixelsOP()
//{
//	delete m_drawOP;
//}
//
//bool SelectPixelsOP::OnMouseLeftDown(int x, int y)
//{
//	if (m_drawOP->OnMouseLeftDown(x, y)) return true;
//
//	switch (m_mode)
//	{
//	case RectSelect:
//		m_firstPos = m_stage->transPosScreenToProject(x, y);
//		break;
//	case PointSelect:
//		m_currPos = m_stage->transPosScreenToProject(x, y);
//		pointSelect();
//		break;
//	case PolylineSelect:
//		m_area.push_back(m_stage->transPosScreenToProject(x, y));
//		break;
//	}
//
//	return false;
//}
//
//bool SelectPixelsOP::OnMouseLeftUp(int x, int y)
//{
//	if (m_drawOP->OnMouseLeftUp(x, y)) return true;
//
//	if (m_mode == RectSelect)
//	{
//		m_currPos = m_stage->transPosScreenToProject(x, y);
//		rectSelect();
//	}
//
//	return false;
//}
//
//bool SelectPixelsOP::OnMouseRightDown(int x, int y)
//{
//	if (m_drawOP->OnMouseRightDown(x, y)) return true;
//
//	if (m_mode == PolylineSelect)
//	{
//		if (!m_area.empty())
//			m_area.pop_back();
//	}
//
//	return false;
//}
//
//bool SelectPixelsOP::OnMouseMove(int x, int y)
//{
//	return m_drawOP->OnMouseMove(x, y);
//}
//
//bool SelectPixelsOP::onMouseLeftDClick(int x, int y)
//{
//	if (m_drawOP->onMouseLeftDClick(x, y)) return true;
//
//	if (m_mode == PolylineSelect)
//	{
//		m_area.push_back(m_area.front());
//		polylineSelect();
//		m_area.clear();
//		m_drawOP->clear();
//		m_stage->Refresh();
//	}
//
//	return false;
//}
//
//bool SelectPixelsOP::OnMouseDrag(int x, int y)
//{
//	return m_drawOP->OnMouseDrag(x, y);
//}
//
//bool SelectPixelsOP::OnDraw() const
//{
//	return m_drawOP->OnDraw();
//}
//
//bool SelectPixelsOP::clear()
//{
//	if (m_drawOP->clear()) return true;
//
//	for_each(m_result.begin(), m_result.end(), DeletePointerFunctor<RawPixels::PixelsPos>());
//	m_result.clear();
//	m_firstPos.SetInvalid();
//	m_currPos.SetInvalid();
//	m_area.clear();
//
//	return false;
//}
//
//void SelectPixelsOP::setEditType(bool isSelect)
//{
//	m_isSelect = isSelect;
//}
//
//void SelectPixelsOP::setMagicMarkerDis(int dis)
//{
//	m_magicMarkerDis = dis;
//}
//
//void SelectPixelsOP::rectSelect()
//{
//	int width, height;
//	m_pixels->getSize(width, height);
//
//	int xmin, xmax, ymin, ymax;
//	if (m_firstPos.x < m_currPos.x)
//	{
//		xmin = static_cast<int>(m_firstPos.x + width * 0.5f + 0.5f);
//		xmax = static_cast<int>(m_currPos.x + width * 0.5f + 0.5f);
//	}
//	else
//	{
//		xmin = static_cast<int>(m_currPos.x + width * 0.5f + 0.5f);
//		xmax = static_cast<int>(m_firstPos.x + width * 0.5f + 0.5f);
//	}
//	if (m_firstPos.y < m_currPos.y)
//	{
//		ymin = static_cast<int>(m_firstPos.y + height * 0.5f + 0.5f);
//		ymax = static_cast<int>(m_currPos.y + height * 0.5f + 0.5f);
//	}
//	else
//	{
//		ymin = static_cast<int>(m_currPos.y + height * 0.5f + 0.5f);
//		ymax = static_cast<int>(m_firstPos.y + height * 0.5f + 0.5f);
//	}
//	if (xmin < 0) xmin = 0;
//	if (xmax >= width) xmax = width - 1;
//	if (ymin < 0) ymin = 0;
//	if (ymax >= height) ymax = height - 1;
//
//	for (int x = xmin; x <= xmax; ++x)
//		for (int y = ymin; y <= ymax; ++y)
//			SelectByPos(x, y);
//
//	m_stage->Refresh();
//}
//
//void SelectPixelsOP::pointSelect()
//{
//	int width, height;
//	m_pixels->getSize(width, height);
//	if (fabs(m_currPos.x) < width * 0.5f && fabs(m_currPos.y) < height * 0.5f)
//	{
//  		int x = static_cast<int>(m_currPos.x + width * 0.5f + 0.5f),
// 			y = static_cast<int>(m_currPos.y + height * 0.5f + 0.5f);
//		if (m_magicMarkerDis <= 0)
//			SelectByPos(x, y);
//		else
//			selectByMagicMarker(x, y);
//	}
//
//	m_stage->Refresh();
//}
//
//void SelectPixelsOP::polylineSelect()
//{
//	if (m_area.size() < 3) return;
//
//	int width, height;
//	m_pixels->getSize(width, height);
//	for (int x = 0; x < width; ++x)
//	{
//		for (int y = 0; y < height; ++y)
//		{
//			Vector pos;
//			pos.x = x - width * 0.5f;
//			pos.y = y - height * 0.5f;
//			if (Math::isPointInArea(pos, m_area))
//				SelectByPos(x, y);
//		}
//	}
//
//	m_stage->Refresh();
//}
//
//void SelectPixelsOP::SelectByPos(int x, int y)
//{
//	RawPixels::PixelsPos* p = new RawPixels::PixelsPos;
//	p->x = x;
//	p->y = y;
//	RawPixels::PixelBuf::iterator itr = m_result.find(p);
//	if (itr == m_result.end()) 
//	{
//		if (m_isSelect)
//			m_result.insert(p);
//	}
//	else
//	{
//		if (!m_isSelect)
//		{
//			delete *itr;
//			m_result.erase(itr);
//		}
//		delete p;
//	}
//}
//
//void SelectPixelsOP::selectByMagicMarker(int x, int y)
//{
//	int width, height;
//	m_pixels->getSize(width, height);
//
//	const unsigned char* pixelData = m_pixels->GetPixelData();
//	const int channels = m_pixels->getChannels();
//	std::vector<bool> flag(width * height, false);
//
//	const int srcPtr = ((height - 1 - y) * width + x) * channels;
//
//	std::queue<Vector> buffer;
//	buffer.push(Vector(x, y));
//	while (!buffer.empty())
//	{
//		Vector curr = buffer.front(); buffer.pop();
//		if (!flag[curr.y * width + curr.x])
//		{
//			flag[curr.y * width + curr.x] = true;
//
//			const int dstPtr = ((height - 1 - curr.y) * width + curr.x) * channels;
//			float dis = 0.0f;
//			for (int i = 0; i < channels; ++i)
//				dis += pow(pixelData[srcPtr + i] - pixelData[dstPtr + i], 2.0f);
//			dis = sqrt(dis);
//			if (dis < m_magicMarkerDis)
//			{
//				SelectByPos(curr.x, curr.y);
//				if (curr.x - 1 >= 0 && curr.x - 1 < width) buffer.push(Vector(curr.x - 1, curr.y));
//				if (curr.x + 1 >= 0 && curr.x + 1 < width) buffer.push(Vector(curr.x + 1, curr.y));
//				if (curr.y - 1 >= 0 && curr.y - 1 < height) buffer.push(Vector(curr.x, curr.y - 1));
//				if (curr.y + 1 >= 0 && curr.y + 1 < height) buffer.push(Vector(curr.x, curr.y + 1));
//			}
//		}
//	}
//}
//
//}