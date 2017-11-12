#ifndef _LIBANIM_PREVIEW_CANVAS_H_
#define _LIBANIM_PREVIEW_CANVAS_H_

#include "Symbol.h"

#include <ee/CameraCanvas.h>
#include <ee/PlayControl.h>

namespace libanim
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
		const std::shared_ptr<Symbol>& sym, wxGLContext* glctx);

	PlaySetting& getPlaySetting();

protected:
	virtual void OnDrawSprites() const override;

protected:
	virtual void OnTimer() override;

private:
	PlaySetting m_setting;

	const std::shared_ptr<Symbol> m_sym;

	ee::PlayControl m_control;

}; // PreviewCanvas

}

#endif // _LIBANIM_PREVIEW_CANVAS_H_