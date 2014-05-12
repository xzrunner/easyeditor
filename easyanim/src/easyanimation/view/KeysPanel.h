#pragma once

#include <wx/wx.h>

namespace eanim
{
	class KeysPanel : public wxScrolledWindow
	{
	public:
		KeysPanel(wxWindow* parent);

		void setCurrPos(int pos);

		void getSelectPos(int& row, int& col) const;
		void setSelectPos(int row, int col);

	private:
		void initLayout();

	private:
		int m_selectRow, m_selectCol;

	}; // KeysPanel
}

