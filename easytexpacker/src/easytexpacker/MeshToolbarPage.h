#ifndef _EASYTEXPACKER_MESH_TOOLBAR_PAGE_H_
#define _EASYTEXPACKER_MESH_TOOLBAR_PAGE_H_

#include <wx/panel.h>

class wxTextCtrl;

namespace etexpacker
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
	void OnSaveImage(wxCommandEvent& event);

	void LoadBodies(const std::string& dir);

private:
	wxTextCtrl* m_src_dir;

	StagePanel* m_stage;

}; // MeshToolbarPage

}

#endif // _EASYTEXPACKER_MESH_TOOLBAR_PAGE_H_