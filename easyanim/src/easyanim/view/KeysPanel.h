#pragma once
#include <wx/wx.h>

namespace eanim
{
	class LayersMgr;
	class KeysScaleWidget;
	class KeysContentWidget;

	class KeysPanel : public wxPanel
	{
	public:
		KeysPanel(wxWindow* parent, LayersMgr* layersMgr);

		int getCurrPos() const;
		void setCurrPos(int pos);

		void getSelectPos(int& row, int& col) const;
		void setSelectPos(int row, int col);

	private:
		void initLayout();

	private:
		LayersMgr* m_layersMgr;

		KeysScaleWidget* m_scaleWidget;
		KeysContentWidget* m_contentWidget;

		int m_currPos;	// start from 1

		int m_selectRow, m_selectCol;

	}; // KeysPanel
}

