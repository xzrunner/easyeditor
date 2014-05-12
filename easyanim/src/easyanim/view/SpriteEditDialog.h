#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace eanim
{
	class Sprite;

	class SpriteEditDialog : public wxDialog
	{
	public:
		SpriteEditDialog(wxWindow* parent, Sprite* sprite);
		~SpriteEditDialog();

	private:
		void initLayout();

		void buildToolBar(wxSizer* topSizer);
		void buildEditPanel(wxSizer* topSizer);

		void onMergeMesh(wxCommandEvent& event);
		void onSeparateMesh(wxCommandEvent& event);
		void onUpdateMeshState(wxUpdateUIEvent& event);

	private:
		Sprite* m_sprite;
		d2d::EditPanel* m_editPanel;

		wxButton *m_btnMerge, *m_btnSeparate;

	}; // SpriteEditDialog
}

