//#pragma once
//
//#include <wx/wx.h>
//#include <vector>
//
//#include "OrthoCanvas.h"
//
//struct sprite;
//
//namespace d2d
//{
//
//class EditPanel;
//class Sprite;
//
//class EJPreviewCanvas : public OrthoCanvas
//{
//public:
//	EJPreviewCanvas(EditPanel* stage,
//		const std::vector<const Sprite*>& sprites);
//
//protected:
//	virtual void InitGL();
//	virtual void OnDraw();
//
//	void onTimer(wxTimerEvent& event);
//
//private:
//	void prepareSprites(const std::vector<const Sprite*>& sprites);
//
//private:
//	enum
//	{
//		TIMER_ID = 1000
//	};
//
//private:
//	std::vector<sprite*> m_sprites;
//
//	wxTimer m_timer;
//
//	DECLARE_EVENT_TABLE()
//
//}; // EJPreviewCanvas
//
//}