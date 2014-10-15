#ifndef _LIBSKETCH_STAGE_CANVAS_H_
#define _LIBSKETCH_STAGE_CANVAS_H_

#include <drag2d.h>

namespace libsketch
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage, d2d::LibraryPanel* library);

protected:
	virtual void initGL();
	virtual void onDraw();

private:
	void InitDefaultSymbol();

private:
	StagePanel* m_stage;
	d2d::LibraryPanel* m_library;

}; // StageCanvas

}

#endif // _LIBSKETCH_STAGE_CANVAS_H_
