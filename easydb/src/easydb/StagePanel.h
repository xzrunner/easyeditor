#ifndef _EASYDB_STAGE_PANEL_H_
#define _EASYDB_STAGE_PANEL_H_

//#include "Graphics.h"

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/ShapesPanelImpl.h>
#include <ee/CombinedDropTarget.h>

class wxTreeCtrl;

namespace edb
{

class Database;
class Symbol;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	               public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxTreeCtrl* tree);
	virtual ~StagePanel();

	void LoadFromDir(const std::string& dirpath);
	void LoadFromDirFast(const std::string& dirpath);

	Database* GetDB() { return m_db; }
	
	Symbol* GetSymbol() { return m_sym; }

private:
	void InitConnection();

	void InitPosition();

private:
	class DropTarget : public ee::CombinedDropTarget
	{
	public:
		DropTarget(wxWindow* stage_wnd, StagePanel* stage, 
			wxTreeCtrl* tree);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {}

	private:
		StagePanel* m_stage;
		wxTreeCtrl* m_tree;

	}; // DropTarget

private:
//	Graphics m_graphics;

	Database* m_db;

	Symbol* m_sym;

	friend class StageCanvas;

}; // StagePanel

}

#endif // _EASYDB_STAGE_PANEL_H_