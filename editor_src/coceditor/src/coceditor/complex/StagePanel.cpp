#include "StagePanel.h"
#include "StageCanvas.h"
#include "FileIO.h"

#include "Context.h"
#include "LibraryPanel.h"
//#include "ComplexSprite.h"

namespace coceditor
{
namespace complex
{
	StagePanel::StagePanel(wxWindow* parent, 
						   wxTopLevelWindow* frame,
						   LibraryPanel* library,
						   d2d::ComplexSymbol* complexSymbol)
		: EditPanel(parent, frame)
		, d2d::MultiSpritesImpl(this)
		, m_library(library)
		, m_complexSymbol(complexSymbol)
	{
		m_editOP = NULL;
		m_canvas = new StageCanvas(this);

		SetDropTarget(new DragSymbolTarget(this, m_library));

//		load();
	}

	StagePanel::~StagePanel()
	{
		
	}

	void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::TraverseType type/* = d2d::e_allExisting*/, 
		bool order/* = true*/) const
	{
		if (order)
		{
			std::vector<d2d::ISprite*>::const_iterator itr = m_complexSymbol->m_sprites.begin();
			for ( ; itr != m_complexSymbol->m_sprites.end(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			std::vector<d2d::ISprite*>::const_reverse_iterator itr = m_complexSymbol->m_sprites.rbegin();
			for ( ; itr != m_complexSymbol->m_sprites.rend(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
	}

	void StagePanel::insertSprite(d2d::ISprite* sprite)
	{
		m_complexSymbol->m_sprites.push_back(sprite);
		Refresh();
	}

	void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
	{
//		d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	}

// 	void StagePanel::onDraw()
// 	{
// 		if (m_complexSymbol) m_complexSymbol->draw();
// 	}

	void StagePanel::loadSymbol()
	{
		
	}
	
	void StagePanel::storeSymbol()
	{
		if (!m_complexSymbol) return;

		wxString filepath = m_complexSymbol->getFilepath();
		if (filepath.empty())
		{
			std::string ext = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
			ext = "_" + ext + ".json";
			filepath = d2d::Context::Instance()->getDir() + "\\" + m_complexSymbol->getName() + ext;
		}

		FileIO::store(filepath, m_complexSymbol);
	}


	//////////////////////////////////////////////////////////////////////////
	// class StagePanel::DragSymbolTarget
	//////////////////////////////////////////////////////////////////////////

	StagePanel::DragSymbolTarget::DragSymbolTarget(StagePanel* stage,
		LibraryPanel* library)
		: m_stage(stage)
		, m_library(library)
	{
	}

	bool StagePanel::DragSymbolTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
	{
 		wxString sType = data.substr(0, data.find(","));
 		wxString sIndex = data.substr(data.find(",") + 1);
 
 		long index;
 		sIndex.ToLong(&index);
 
		d2d::ISymbol* symbol = NULL;
		if (sType == "symbol")
			symbol = static_cast<d2d::ISymbol*>(m_library->getImagePage()->getSymbol(index));
		else if (sType == "complex")
			symbol = static_cast<d2d::ISymbol*>(m_library->getComplexPage()->getSymbol(index));

		if (symbol)
		{
			d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->translate(pos);
			m_stage->insertSprite(sprite);
		}
 
 		return true;
	}
}
}