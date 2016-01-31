#ifndef _LIBANIM_PREVIEW_CANVAS_H_
#define _LIBANIM_PREVIEW_CANVAS_H_


#include <wx/wx.h>

#include "Symbol.h"

namespace libanim
{

class PreviewCanvas : public ee::OrthoCanvas
{
public:
	struct PlaySetting
	{
		bool isCirculate;
		bool isStop;

		PlaySetting()
		{
			isCirculate = true;
			isStop = false;
		}
	};

public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		const Symbol* symbol, wxGLContext* glctx);

	PlaySetting& getPlaySetting();

protected:
	virtual void OnDrawSprites() const;

protected:
	virtual void OnTimer();

private:
	PlaySetting m_setting;

	const Symbol* m_symbol;

	ee::PlayControl m_control;

}; // PreviewCanvas

}

#endif // _LIBANIM_PREVIEW_CANVAS_H_