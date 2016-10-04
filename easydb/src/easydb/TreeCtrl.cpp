#include "TreeCtrl.h"
#include "Graphics.h"
#include "Context.h"
#include "StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/SpriteSelection.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>

#include <sprite2/SymType.h>

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

	std::map<ee::Sprite*, Node*>::const_iterator itr
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
		std::string filepath = files[i];
		int type = ee::SymbolFile::Instance()->Type(filepath);
		if (type == s2::SYM_COMPLEX || type == s2::SYM_ANIMATION) {
			std::string name = ee::FileHelper::GetFilename(filepath);
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
		ee::Sprite* child = node.out[i];
		wxTreeItemId id = AppendItem(parent, getItemName(*child));
		m_mapID2Sprite.insert(std::make_pair(id, child));

		Node* query = graph.Query(child);
		if (query)
			addNode(graph, *query, id);
	}
}

std::string TreeCtrl::getItemName(const ee::Sprite& spr) const
{
	std::string name = dynamic_cast<const ee::Symbol*>(spr.GetSymbol())->GetFilepath();
	name = ee::FileHelper::GetFilename(name);
	return name;
}

void TreeCtrl::onSelChanged(wxTreeEvent& event)
{
	ee::Sprite* spr = querySpriteByID(event.GetItem());
	if (!spr) return;
	
	ee::SpriteSelection* selection = Context::Instance()->stage->GetSpriteSelection();
	selection->Clear();
	selection->Add(spr);

	m_parent->Refresh(true);
}

void TreeCtrl::onItemClick(wxTreeEvent& event)
{
	ee::Sprite* spr = querySpriteByID(event.GetItem());
	if (spr) 
	{
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
		{
			StagePanel* stage = Context::Instance()->stage;

			ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
			ecomplex::EditDialog dlg(stage, sym, NULL);
			dlg.ShowModal();
		}
	}
	else
	{
		std::map<wxTreeItemId, std::string>::iterator itr = m_mapID2Path.find(event.GetItem());
		if (itr != m_mapID2Path.end())
		{
			std::string filename = itr->second;
			int type = ee::SymbolFile::Instance()->Type(filename);
			switch (type)
			{
			case s2::SYM_COMPLEX:
				{
					std::string cmd = "easycomplex.exe " + itr->second;
					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
				}
				break;
			case s2::SYM_ANIMATION:
				{
					std::string cmd = "easyanimation.exe " + itr->second;
					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
				}
				break;
			}

// 				ee::Symbol* sym = ee::SymbolMgr::Instance()->getSymbol(itr->second);
// 				if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(sym))
// 				{
//  					StagePanel* stage = Context::Instance()->stage;
//  					ecomplex::EditDialog dlg(stage, complex);
//  					dlg.ShowModal();
//  					stage->resetCanvas();
// 				}
// 				else if (eanim::Symbol* anim = dynamic_cast<eanim::Symbol*>(sym))
// 				{
// 
// 				}
		}
	}
}

ee::Sprite* TreeCtrl::querySpriteByID(wxTreeItemId id) const
{
	std::map<wxTreeItemId, ee::Sprite*>::const_iterator 
		itr = m_mapID2Sprite.find(id);
	if (itr == m_mapID2Sprite.end()) 
		return NULL;
	else
		return itr->second;
}

}