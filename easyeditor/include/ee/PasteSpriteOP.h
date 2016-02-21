#ifndef _EASYEDITOR_PASTE_SPRITE_OP_H_
#define _EASYEDITOR_PASTE_SPRITE_OP_H_

#include "SelectSpritesOP.h"

#include <vector>

namespace ee
{

class Sprite;
class MultiSpritesImpl;
class PasteSpriteCMPT;

class PasteSpriteOP : public SelectSpritesOP
{
public:
	PasteSpriteOP(wxWindow* wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, PasteSpriteCMPT* cmpt = NULL);
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

		void insertToSpritesImpl(const Vector& pos, bool isHorMirror, bool isVerMirror);
		void draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const;

		const Vector& getCenter() const { return m_center; }

		void clear();

		bool empty() const { return m_selected.empty(); }

	private:
		void computeCenter();

	private:
		std::vector<Sprite*> m_selected;
		Vector m_center;

	}; // SpriteBatch

private:
	PasteSpriteCMPT* m_cmpt;

	SpriteSelection* m_selection;

	Vector m_pos;

	SpriteBatch m_batch;

}; // PasteSpriteOP

}

#endif // _EASYEDITOR_PASTE_SPRITE_OP_H_