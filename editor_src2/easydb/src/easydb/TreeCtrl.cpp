#include "TreeCtrl.h"

#include <map>
#include <easycomplex.h>

#include "Graphics.h"
#include "Context.h"
#include "StagePanel.h"

namespace edb
{
	BEGIN_EVENT_TABLE(TreeCtrl, wxTreeCtrl)
		EVT_TREE_SEL_CHANGED(ID_CTRL, TreeCtrl::onSelChanged)
		EVT_TREE_ITEM_ACTIVATED(ID_CTRL, TreeCtrl::onItemClick)

	END_EVENT_TABLE()

	TreeCtrl::TreeCtrl(wxWindow* parent)
		: wxTreeCtrl(parent, ID_CTRL)
	{
	}

	void TreeCtrl::init(const Graphics& graph)
	{
		bool empty = IsEmpty();

		clear();
		
		if (empty)
			m_root = AddRoot("Root");

		std::map<d2d::ISprite*, Node*>::const_iterator itr
			= graph.connection.begin();
		for ( ; itr != graph.connection.end(); ++itr)
		{
			wxTreeItemId id = AppendItem(m_root, getItemName(*itr->first));
			m_mapID2Sprite.insert(std::make_pair(id, itr->first));
			addNode(graph, *itr->second, id);
		}
	}

	void TreeCtrl::init(const wxArrayString& files)
	{
		bool empty = IsEmpty();

		clear();

		if (empty)
			m_root = AddRoot("Root");

		for (size_t i = 0, n = files.size(); i < n; ++i)
		{
			wxString filepath = files[i];
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
			{
				wxString name = d2d::FilenameTools::getFilename(filepath);
				wxTreeItemId id = AppendItem(m_root, name);
				m_mapID2Path.insert(std::make_pair(id, filepath));
			}
		}
	}

	void TreeCtrl::clear()
	{
		if (!IsEmpty())
			CollapseAndReset(m_root);
	}

	void TreeCtrl::addNode(const Graphics& graph, const Node& node, wxTreeItemId parent)
	{
		for (size_t i = 0, n = node.out.size(); i < n; ++i)
		{
			d2d::ISprite* child = node.out[i];
			wxTreeItemId id = AppendItem(parent, getItemName(*child));
			m_mapID2Sprite.insert(std::make_pair(id, child));

			Node* query = graph.query(child);
			if (query)
				addNode(graph, *query, id);
		}
	}

	wxString TreeCtrl::getItemName(const d2d::ISprite& sprite) const
	{
		wxString name = sprite.getSymbol().getFilepath();
		name = d2d::FilenameTools::getFilename(name);
		return name;
	}

	void TreeCtrl::onSelChanged(wxTreeEvent& event)
	{
		d2d::ISprite* sprite = querySpriteByID(event.GetItem());
		if (!sprite) return;
		
		d2d::SpriteSelection* selection = Context::Instance()->stage->getSpriteSelection();
		selection->clear();
		selection->insert(sprite);

		m_parent->Refresh();
	}

	void TreeCtrl::onItemClick(wxTreeEvent& event)
	{
		d2d::ISprite* sprite = querySpriteByID(event.GetItem());
		if (sprite) 
		{
			if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(sprite))
			{
				StagePanel* stage = Context::Instance()->stage;

				complex::Symbol& symbol = const_cast<complex::Symbol&>(complex->getSymbol());
				complex::EditDialog dlg(stage, &symbol);
				dlg.ShowModal();

				stage->resetCanvas();
			}
		}
		else
		{
			std::map<wxTreeItemId, wxString>::iterator itr = m_mapID2Path.find(event.GetItem());
			if (itr != m_mapID2Path.end())
			{
				std::string filename = itr->second;
				if (d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_complex))
				{
					std::string cmd = "easycomplex.exe " + itr->second;
					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
				}
				else if (d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_anim))
				{
					std::string cmd = "easyanimation.exe " + itr->second;
					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
				}

// 				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(itr->second);
// 				if (complex::Symbol* complex = dynamic_cast<complex::Symbol*>(symbol))
// 				{
//  					StagePanel* stage = Context::Instance()->stage;
//  					complex::EditDialog dlg(stage, complex);
//  					dlg.ShowModal();
//  					stage->resetCanvas();
// 				}
// 				else if (anim::Symbol* anim = dynamic_cast<anim::Symbol*>(symbol))
// 				{
// 
// 				}
			}
		}
	}

	d2d::ISprite* TreeCtrl::querySpriteByID(wxTreeItemId id) const
	{
		std::map<wxTreeItemId, d2d::ISprite*>::const_iterator 
			itr = m_mapID2Sprite.find(id);
		if (itr == m_mapID2Sprite.end()) 
			return NULL;
		else
			return itr->second;
	}
}