#ifndef _LIBSKELETON_EDIT_DIALOG_H_
#define _LIBSKELETON_EDIT_DIALOG_H_

#include <sprite2/SkeletonPose.h>

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace libskeleton
{

class Sprite;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx, 
		const std::shared_ptr<Sprite>& spr);

private:
	void InitLayout(wxGLContext* glctx);

	void OnCloseEvent(wxCloseEvent& event);

private:
	std::shared_ptr<Sprite> m_spr;

	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _LIBSKELETON_EDIT_DIALOG_H_