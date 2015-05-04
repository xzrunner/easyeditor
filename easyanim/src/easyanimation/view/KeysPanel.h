#pragma once

#include <wx/wx.h>

namespace eanim
{
	class Controller;

	class KeysPanel : public wxScrolledWindow
	{
	public:
		KeysPanel(wxWindow* parent, Controller* ctrl);

		void setCurrPos(int pos);

		void getSelectPos(int& row, int& col) const;
		void setSelectPos(int row, int col);

	private:
		void initLayout();

	private:
		Controller* m_ctrl;

		int m_selectRow, m_selectCol;

	}; // KeysPanel
}

