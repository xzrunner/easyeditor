#pragma once

#include <wx/wx.h>


namespace eanim
{
	class Symbol;

	class SymbolEditDialog : public wxDialog
	{
	public:
		SymbolEditDialog(wxWindow* parent, Symbol* symbol);
		~SymbolEditDialog();

	private:
		void initLayout();

		void buildToolBarSettings(wxSizer* topSizer);
		void buildEditPanel(wxSizer* topSizer);
		void buildToolBarEditors(wxSizer* topSizer);

		void buildSettingsItems(wxBoxSizer* toolSizer);
		void buildEditModeChoice(wxBoxSizer* toolSizer);

		void buildClipModeToolBar(wxBoxSizer* toolSizer);
		void buildMeshModeToolBar(wxBoxSizer* toolSizer);
		void buildPartSkeletonModeToolBar(wxBoxSizer* toolSizer);
		void buildWholeSkeletonModeToolBar(wxBoxSizer* toolSizer);

		// left
		void onSetName(wxCommandEvent& event);
		void onSetColor(wxCommandEvent& event);
		void onSetEditMode(wxCommandEvent& event);
		void onUpdateSetEditMode(wxUpdateUIEvent& event);

		// clip mode
		void onChooseSelectWay(wxCommandEvent& event);
		void onChooseEditType(wxCommandEvent& event);
		void onSetMagicMarkerDis(wxSpinEvent& event);
		void onSetDisplayPixelBound(wxCommandEvent& event);
		void onSetDisplayOriginalSymbol(wxCommandEvent& event);
		void onClearSelectedPixels(wxCommandEvent& event);
		void onCreateSelectedPixels(wxCommandEvent& event);

		// mesh mode
		void onClearMeshNodes(wxCommandEvent& event);

		// skeleton mode
		void onSetSkeletonEditMode(wxCommandEvent& event);

	private:
		Symbol* m_symbol;
		ee::EditPanel* m_editPanel;

		wxRadioBox* m_editModeChoice;

		wxBoxSizer* m_clipModeSizer;
		wxBoxSizer* m_meshModeSizer;
		wxBoxSizer* m_partSkeletonModeSizer;
		wxBoxSizer* m_wholeSkeletonModeSizer;

		bool m_isEditSelect;
		wxStaticText* m_magicMakerText;
		wxSpinCtrl* m_magicMarkerDisSpin;

	}; // SymbolEditDialog
}

