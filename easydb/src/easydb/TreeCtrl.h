#pragma once

#include <wx/treectrl.h>
#include <drag2d.h>

namespace edb
{
	class Graphics;
	struct Node;

	class TreeCtrl : public wxTreeCtrl
	{
	public:
		TreeCtrl(wxWindow* parent);

		void test();

		void init(const Graphics& graph);
		void init(const wxArrayString& files);

		void clear();

	private:
		enum
		{
			ID_CTRL
		};

	private:
		void addNode(const Graphics& graph, const Node& node, wxTreeItemId parent);

		wxString getItemName(const d2d::Sprite& sprite) const;

		void onSelChanged(wxTreeEvent& event);
		void onItemClick(wxTreeEvent& event);

		d2d::Sprite* querySpriteByID(wxTreeItemId id) const;

	private:
		wxTreeItemId m_root;

		std::map<wxTreeItemId, d2d::Sprite*> m_mapID2Sprite;

		std::map<wxTreeItemId, wxString> m_mapID2Path;

		DECLARE_EVENT_TABLE()

	}; // TreeCtrl
}