#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace eanim
{
	class LayersMgr;
	class LibraryPanel;

	class PreviewDialog : public wxDialog
	{
	public:
		PreviewDialog(wxWindow* parent, LayersMgr* layers, int fps = 24);
		~PreviewDialog();

	private:
		void initLayout(LayersMgr* layers, int fps);

		void buildToolBar(wxSizer* topSizer);
		void buildEditPanel(wxSizer* topSizer, LayersMgr* layers, int fps);

		void onSetCirculate(wxCommandEvent& event);
		void onSetStop(wxCommandEvent& event);

	private:
		d2d::EditPanel* m_editPanel;
		
	}; // PreviewDialog
}

