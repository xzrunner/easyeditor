//#pragma once
//
//#include <vector>
//
//#include "ZoomViewOP.h"
//#include "dataset/RawPixels.h"
//
//namespace d2d
//{
//	class SelectPixelsOP : public ZoomViewOP
//	{
//	public:
//		enum e_Mode
//		{
//			RectSelect = 0,
//			PointSelect,
//			PolylineSelect
//		};
//
//	public:
//		SelectPixelsOP(EditPanel* editPanel, RawPixels* pixels, e_Mode selectType, 
//			RawPixels::PixelBuf& result, bool isSelect, int magicMarkerDis = 0);
//		virtual ~SelectPixelsOP();
//
//		virtual bool OnMouseLeftDown(int x, int y);
//		virtual bool OnMouseLeftUp(int x, int y);
//		virtual bool OnMouseRightDown(int x, int y);
//		virtual bool OnMouseMove(int x, int y);
//		virtual bool onMouseLeftDClick(int x, int y);
//		virtual bool OnMouseDrag(int x, int y);
//
//		virtual bool OnDraw() const;
//		virtual bool clear();
//
//		void setEditType(bool isSelect);
//		void setMagicMarkerDis(int dis);
//
//	private:
//		void rectSelect();
//		void pointSelect();
//		void polylineSelect();
//
//		void SelectByPos(int x, int y);
//
//		void selectByMagicMarker(int x, int y);
//
//	private:
//		RawPixels* m_pixels;
//
//		bool m_isSelect;
//
//		e_Mode m_mode;
//		AbstractEditOP* m_drawOP;
//		int m_magicMarkerDis;
//
//		RawPixels::PixelBuf& m_result;
//		
//		Vector m_firstPos, m_currPos;
//		std::vector<Vector> m_area;
//
//	}; // SelectPixelsOP
//}
//
