#ifndef _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_
#define _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_

#include "PasteSymbolOP.h"
#include "PasteSymbolOffsetCMPT.h"
#include "Visitor.h"

namespace ee
{

class MultiSpritesImpl;
class LibraryPanel;
class Sprite;

class PasteSymbolTileOP : public PasteSymbolOP
{
public:
	PasteSymbolTileOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl, 
		LibraryPanel* library, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);

	virtual bool OnDraw() const;

private:
	class NearestQueryVisitor : public Visitor
	{
	public:
		NearestQueryVisitor(const Vector& pos, Sprite** ret);

		virtual void Visit(Object* object, bool& next);

	private:
		const Vector& m_pos;

		float m_dis;

		Sprite** m_result;

	}; // NearestQueryVisitor

private:
	MultiSpritesImpl* m_spritesImpl;

	PasteSymbolOffsetCMPT<PasteSymbolTileOP>* m_cmpt;

	bool m_bCaptured;

	float m_rotate;

}; // PasteSymbolTileOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_TILE_OP_H_