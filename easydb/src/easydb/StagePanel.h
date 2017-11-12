#ifndef _EASYDB_STAGE_PANEL_H_
#define _EASYDB_STAGE_PANEL_H_

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
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~StagePanel();

	Database* GetDB() { return m_db; }
	
	auto& GetSymbol() { return m_sym; }

private:
	class DropTarget : public ee::CombinedDropTarget
	{
	public:
		DropTarget(wxWindow* stage_wnd, StagePanel* stage);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override {}

	private:
		StagePanel* m_stage;

	}; // DropTarget

private:
	Database* m_db;

	std::shared_ptr<Symbol> m_sym;

	friend class StageCanvas;

}; // StagePanel

}

#endif // _EASYDB_STAGE_PANEL_H_