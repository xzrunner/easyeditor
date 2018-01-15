#pragma once

#include <sprite2/Sprite.h>
#include <node3/INode.h>

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace enode3d
{

class Sprite;
class StagePanel;
class StageCanvas;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		const std::shared_ptr<Sprite>& spr);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel*  m_stage;
	StageCanvas* m_canvas;

	s2::SprPtr  m_spr = nullptr;
	n3::NodePtr m_node = nullptr;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}