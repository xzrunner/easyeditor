#ifndef _TEST_DTEX_STAGE_PANEL_H_
#define _TEST_DTEX_STAGE_PANEL_H_

#include "TestBase.h"

#include <drag2d.h>

namespace tdtex
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual bool Update(int version) { return true; }

	void Load();
	void UnLoad();

	void Update();
	void Draw() const;

private:
	void PrintLog() const;

private:
	TestBase* m_test;

}; // StagePanel

}

#endif // _TEST_DTEX_STAGE_PANEL_H_