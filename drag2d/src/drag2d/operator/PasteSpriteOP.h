#ifndef _DRAG2D_PASTE_SPRITE_OP_H_
#define _DRAG2D_PASTE_SPRITE_OP_H_

#include <vector>

#include "SelectSpritesOP.h"

namespace d2d
{

class ISprite;
class MultiSpritesImpl;
class PasteSpriteCMPT;

class PasteSpriteOP : public SelectSpritesOP
{
public:
	PasteSpriteOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, ViewPanelMgr* view_panel_mgr, 
		PasteSpriteCMPT* cmpt = NULL);
	virtual ~PasteSpriteOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void setMousePos(int x, int y);
	void fixPosOrthogonal();

private:
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void loadFromSelection(const SpriteSelection& selection);

		void insertToSpritesImpl(MultiSpritesImpl* spritesImpl, const Vector& pos,
			bool isHorMirror, bool isVerMirror);
		void draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const;

		const Vector& getCenter() const { return m_center; }

		void clear();

		bool empty() const { return m_selected.empty(); }

	private:
		void computeCenter();

	private:
		std::vector<ISprite*> m_selected;
		Vector m_center;

	}; // SpriteBatch

private:
	MultiSpritesImpl* m_spritesImpl;

	PasteSpriteCMPT* m_cmpt;

	SpriteSelection* m_selection;

	Vector m_pos;

	SpriteBatch m_batch;

}; // PasteSpriteOP

}

#endif // _DRAG2D_PASTE_SPRITE_OP_H_