#ifndef _TEST_DTEX_STAGE_PANEL_H_
#define _TEST_DTEX_STAGE_PANEL_H_

#include "ITest.h"

#include <drag2d.h>

namespace tdtex
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	void Load();
	void UnLoad();

	void Draw() const;

private:
	ITest* m_test;

}; // StagePanel

}

#endif // _TEST_DTEX_STAGE_PANEL_H_