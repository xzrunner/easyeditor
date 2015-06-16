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
//class ISprite;
//
//class EJPreviewCanvas : public OrthoCanvas
//{
//public:
//	EJPreviewCanvas(EditPanel* stage,
//		const std::vector<const ISprite*>& sprites);
//
//protected:
//	virtual void initGL();
//	virtual void OnDraw();
//
//	void onTimer(wxTimerEvent& event);
//
//private:
//	void prepareSprites(const std::vector<const ISprite*>& sprites);
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