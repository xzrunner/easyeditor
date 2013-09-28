#include "StagePanel.h"
#include "FileIO.h"
#include "Context.h"
#include "LibraryPanel.h"

namespace ecomplex
{
	StagePanel::StagePanel(wxWindow* parent,
						   wxTopLevelWindow* frame)
		: libcomplex::StagePanel(parent, frame)
	{
		SetDropTarget(new DragSymbolTarget(this));
	}

	StagePanel::StagePanel(wxWindow* parent, 
						   wxTopLevelWindow* frame,
						   d2d::ComplexSymbol* symbol)
		: libcomplex::StagePanel(parent, frame, symbol)
	{
		SetDropTarget(new DragSymbolTarget(this));
	}

	void StagePanel::clear()
	{
		d2d::EditPanel::clear();

		d2d::ComplexSymbol* symbol = getSymbol();
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
			symbol->m_sprites[i]->release();
		symbol->m_sprites.clear();
	}

	void StagePanel::removeSprite(d2d::ISprite* sprite)
	{
		libcomplex::StagePanel::removeSprite(sprite);
		Context::Instance()->viewlist->remove(sprite);
	}

	void StagePanel::insertSprite(d2d::ISprite* sprite)
	{
		libcomplex::StagePanel::insertSprite(sprite);
		Context::Instance()->viewlist->insert(sprite);
	}

	//////////////////////////////////////////////////////////////////////////
	// class StagePanel::DragSymbolTarget
	//////////////////////////////////////////////////////////////////////////

	StagePanel::DragSymbolTarget::DragSymbolTarget(StagePanel* stage)
		: m_stage(stage)
	{
	}

	bool StagePanel::DragSymbolTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
	{
 		wxString sType = data.substr(0, data.find(","));
 		wxString sIndex = data.substr(data.find(",") + 1);
 
 		long index;
 		sIndex.ToLong(&index);
 
		LibraryPanel* library = static_cast<LibraryPanel*>(Context::Instance()->library);
		d2d::ISymbol* symbol = NULL;
		if (sType == "symbol")
			symbol = static_cast<d2d::ISymbol*>(library->getImagePage()->getSymbol(index));
		else if (sType == "complex")
			symbol = static_cast<d2d::ISymbol*>(library->getComplexPage()->getSymbol(index));
		else if (sType == "anim")
			symbol = static_cast<d2d::ISymbol*>(library->getAnimPage()->getSymbol(index));
		else if (sType == "9patch")
			symbol = static_cast<d2d::ISymbol*>(library->get9PatchPage()->getSymbol(index));
		else if (sType == "fontblank")
			symbol = static_cast<d2d::ISymbol*>(library->getFontPage()->getSymbol(index));

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