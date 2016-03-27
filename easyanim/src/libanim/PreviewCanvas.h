#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include "Symbol.h"

#include <ee/CameraCanvas.h>
#include <ee/PlayControl.h>

namespace eanim
{

class PreviewCanvas : public ee::CameraCanvas
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

#endif // _EASYANIM_PREVIEW_CANVAS_H_