#pragma once

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace enode3d
{

class Sprite;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		const std::shared_ptr<Sprite>& spr);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, const std::shared_ptr<Sprite>& spr);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}