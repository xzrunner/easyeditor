#ifndef _EPACKER_MESH_TOOLBAR_PAGE_H_
#define _EPACKER_MESH_TOOLBAR_PAGE_H_

#include <drag2d.h>

namespace epacker
{

class StagePanel;

class MeshToolbarPage : public wxPanel
{
public:
	MeshToolbarPage(wxWindow* parent, StagePanel* stage);

private:
	void InitLayout();

	void OnSetSrcDir(wxCommandEvent& event);
	void OnSaveToFile(wxCommandEvent& event);

	void LoadBodies(const wxString& dir);

private:
	wxTextCtrl* m_src_dir;

	StagePanel* m_stage;

}; // MeshToolbarPage

}

#endif // _EPACKER_MESH_TOOLBAR_PAGE_H_