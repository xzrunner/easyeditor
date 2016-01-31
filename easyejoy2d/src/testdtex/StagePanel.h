#ifndef _TEST_DTEX_STAGE_PANEL_H_
#define _TEST_DTEX_STAGE_PANEL_H_

#include "TestBase.h"



namespace tdtex
{

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// ee::EditPanel interface
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