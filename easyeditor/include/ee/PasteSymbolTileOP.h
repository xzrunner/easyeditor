#ifndef _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_
#define _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_

#include "PasteSymbolOP.h"
#include "PasteSymbolOffsetCMPT.h"
#include "Visitor.h"
#include "Sprite.h"

namespace ee
{

class MultiSpritesImpl;
class LibraryPanel;

class PasteSymbolTileOP : public PasteSymbolOP
{
public:
	PasteSymbolTileOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl, 
		LibraryPanel* library, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	class NearestQueryVisitor : public RefVisitor<Sprite>
	{
	public:
		NearestQueryVisitor(const sm::vec2& pos);

		virtual void Visit(const SprPtr& spr, bool& next) override;

		const SprPtr& GetResult() const { return m_result;  }

	private:
		const sm::vec2& m_pos;

		float m_dis;
		
		SprPtr m_result;


	}; // NearestQueryVisitor

private:
	MultiSpritesImpl* m_spritesImpl;

	PasteSymbolOffsetCMPT<PasteSymbolTileOP>* m_cmpt;

	bool m_bCaptured;

	float m_rotate;

}; // PasteSymbolTileOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_