#ifndef _LIBANIM_PREVIEW_CANVAS_H_
#define _LIBANIM_PREVIEW_CANVAS_H_

#include <drag2d.h>
#include <wx/wx.h>

#include "Symbol.h"

namespace libanim
{

class PreviewCanvas : public d2d::OrthoCanvas
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
	PreviewCanvas(d2d::EditPanel* stage, const Symbol* symbol);

	PlaySetting& getPlaySetting();

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

protected:
	virtual void OnTimer();

private:
	PlaySetting m_setting;

	const Symbol* m_symbol;

	d2d::PlayControl m_control;

}; // PreviewCanvas

}

#endif // _LIBANIM_PREVIEW_CANVAS_H_