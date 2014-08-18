#ifndef _DRAG2D_ARRANGE_SPRITE_IMPL_H_
#define _DRAG2D_ARRANGE_SPRITE_IMPL_H_

#include <wx/menu.h>

#include "AutoAlign.h"

#include "common/typedef.h"
#include "dataset/SelectionSet.h"
#include "render/ShapeStyle.h"

namespace d2d
{

class MultiSpritesImpl;
class PropertySettingPanel;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel, bool isDeformOpen = true,
		bool isAutoAlignOpen = true);
	~ArrangeSpriteImpl();

	virtual void onKeyDown(int keyCode);
	virtual void onKeyUp(int keyCode);
	virtual void onMouseLeftDown(int x, int y);
	virtual void onMouseLeftUp(int x, int y);
	virtual void onMouseRightDown(int x, int y);
	virtual void onMouseRightUp(int x, int y);
	virtual void onMouseDrag(int x, int y);

	virtual void onPopMenuSelected(int type);

	virtual void onDraw() const;
	virtual void clear();

public:
	// 0 - 3 are scale, 4 - 7 are shear
	// 0 4 1
	// 5   6
	// 2 7 3
	static void GetSpriteCtrlNodes(const ISprite* sprite, Vector nodes[8]);

protected:
	virtual void onDirectionKeyDown(DirectionType type);
	virtual void onSpaceKeyDown();

	virtual void translateSprite(const Vector& delta);
	virtual void rotateSprite(const Vector& dst);
	virtual void scaleSprite(const Vector& currPos);
	virtual void shearSprite(const Vector& currPos);

	virtual void setRightPopupMenu(wxMenu& menu);
	virtual bool isOffsetEnable() const { return true; };

private:
	void setScalingFromSelected();

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
	static const float SCALE_NODE_RADIUS;

private:
	// 0 4 1
	// 5   6
	// 2 7 3
	enum CtrlNodeType
	{
		LEFT_UP = 0,
		RIGHT_UP,
		LEFT_DOWN,
		RIGHT_DOWN,
		UP,
		LEFT,
		RIGHT,
		DOWN
	};

	struct CtrlNode
	{
		Vector pos;
		CtrlNodeType type;

		void setInvalid() {
			pos.setInvalid();
		}
		bool isValid() {
			return pos.isValid();
		}
	};

private:
	AutoAlign m_align;

	EditPanel* m_editPanel;
	MultiSpritesImpl* m_spritesImpl;
	PropertySettingPanel* m_propertyPanel;

	bool m_bDirty;

	Vector m_lastPos;
	ISprite* m_selected;
	Vector m_firstPos;

	bool m_isDeformOpen;

	bool m_bRightPress;

	CtrlNode m_ctrlNodeSelected;
	bool m_selOffset;

	ShapeStyle m_shearNodeStyle;

}; // ArrangeSpriteImpl

}

#endif // _DRAG2D_ARRANGE_SPRITE_IMPL_H_