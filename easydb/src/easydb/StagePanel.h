#ifndef EDB_STAGE_PANEL_H
#define EDB_STAGE_PANEL_H



#include "Graphics.h"

namespace edb
{

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	               public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	virtual ~StagePanel();

	void loadFromDir(const std::string& dirpath);
	void loadFromDirFast(const std::string& dirpath);

private:
	void initConnection();

	void initPosition();

private:
	Graphics m_graphics;

	friend class StageCanvas;

}; // StagePanel

}

#endif // EDB_STAGE_PANEL_H