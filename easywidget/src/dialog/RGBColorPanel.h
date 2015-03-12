#ifndef _EASYWIDGET_RGB_COLOR_PANEL_H_
#define _EASYWIDGET_RGB_COLOR_PANEL_H_

#include <easy2d.h>
#include <easygui.h>

#include "ITweenColorPanel.h"
#include "canvas/SimpleGLCanvas.h"

namespace ewidget
{

class RGBColorPanel : public egui::Panel, public ITweenColorPanel
{
public:
	RGBColorPanel(egui::Window* parent, int w, int h);
	virtual ~RGBColorPanel();

	//
	// interface ITweenColorPanel
	//
	virtual void SetColor(const Colorf& begin, const Colorf& end);

protected:
	//
	// interface Panel
	//
	virtual void OnSize(int w, int h);

private:
	class Canvas : public SimpleGLCanvas
	{
	public:
		Canvas(egui::Window* parent);

		void SetColor(const Colorf& begin, const Colorf& end);

	protected:
		virtual void OnDraw() const;

	private:
		Colorf m_col_begin, m_col_end;

	}; // Canvas

private:
	Canvas* m_canvas;

}; // RGBColorPanel

}

#endif // _EASYWIDGET_RGB_COLOR_PANEL_H_
