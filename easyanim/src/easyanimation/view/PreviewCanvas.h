#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <drag2d.h>
#include <wx/wx.h>

namespace eanim
{

class Controller;
struct PlaySettings;

class PreviewCanvas : public d2d::DynamicStageCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
		d2d::PlayControl& control, Controller* ctrl);

protected:
	virtual void initGL();
	virtual void onDraw();

	virtual void OnTimer();

private:
	void drawStageData();

	void getCurrSprites(std::vector<d2d::ISprite*>& sprites) const;

private:
	d2d::PlayControl& m_control;

	const PlaySettings& m_settings;

	Controller* m_ctrl;

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_