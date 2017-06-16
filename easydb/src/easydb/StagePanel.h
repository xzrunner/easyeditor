#ifndef _EASYDB_STAGE_PANEL_H_
#define _EASYDB_STAGE_PANEL_H_

//#include "Graphics.h"
#include "Database.h"

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/ShapesPanelImpl.h>

namespace edb
{

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	               public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	virtual ~StagePanel();

	void LoadFromDir(const std::string& dirpath);
	void LoadFromDirFast(const std::string& dirpath);

	Database& GetDB() { return m_db; }

private:
	void InitConnection();

	void InitPosition();

private:
//	Graphics m_graphics;

	Database m_db;

	friend class StageCanvas;

}; // StagePanel

}

#endif // _EASYDB_STAGE_PANEL_H_