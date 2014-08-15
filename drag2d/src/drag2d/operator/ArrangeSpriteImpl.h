#ifndef _DRAG2D_ARRANGE_SPRITE_IMPL_H_
#define _DRAG2D_ARRANGE_SPRITE_IMPL_H_

#include "AutoAlign.h"

#include "dataset/SelectionSet.h"

namespace d2d
{

class MultiSpritesImpl;
class PropertySettingPanel;

// class EditPanel;
// class Vector;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(MultiSpritesImpl* spritesImpl, SpriteSelection* selection,
		PropertySettingPanel* propertyPanel, EditPanel* editPanel, bool& bDirty);
	~ArrangeSpriteImpl();

	virtual void translateSprite(const Vector& delta);
	virtual void rotateSprite(const Vector& dst);
	virtual void scaleSprite(const Vector& currPos);
	virtual void shearSprite(const Vector& currPos);
	
	bool IsAutoAlignOpen() const { return m_align.IsOpen(); }
	void SetAutoAlignOpen(bool open) { m_align.SetOpen(open); }

	void SetAutoAlignInvisible();

	void Align(const std::vector<ISprite*>& sprites);
	void Align(const ISprite* src, ISprite* dst);

	void OnDraw() const;

public:
	// 0 - 3 are scale, 4 - 7 are shear
	// 0 4 1
	// 5   6
	// 2 7 3
	static void GetSpriteCtrlNodes(const ISprite* sprite, Vector nodes[8]);

private:
	class TranslateVisitor : public IVisitor
	{
	public:
		TranslateVisitor(const Vector& delta);
		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const Vector& m_delta;

	}; // TranslateVisitor

	class RotateVisitor : public IVisitor
	{
	public:
		RotateVisitor(const Vector& start, const Vector& end);
		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const Vector &m_start, &m_end;

	}; // RotateVisitor

protected:
	SpriteSelection* m_selection;

private:
	AutoAlign m_align;

	PropertySettingPanel* m_propertyPanel;
	EditPanel* m_editPanel;

	bool& m_bDirty;

	Vector& m_lastPos;
	ISprite* m_selected;
	Vector m_firstPos;

}; // ArrangeSpriteImpl

}

#endif // _DRAG2D_ARRANGE_SPRITE_IMPL_H_