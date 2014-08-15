#pragma once

#include <vector>
#include <wx/menu.h>

#include "interfaces.h"
#include "common/typedef.h"
#include "dataset/SelectionSet.h"
#include "render/ShapeStyle.h"

namespace d2d
{

class ArrangeSpriteImpl;
class EditPanel;
class MultiSpritesImpl;
class PropertySettingPanel;
class AbstractEditCMPT;

template <typename TBase>
class ArrangeSpriteOP : public TBase
{
public:
	ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL,
		bool isDeformOpen = true, bool isAutoAlignOpen = true);
	virtual ~ArrangeSpriteOP();

	virtual bool onKeyDown(int keyCode);
	virtual bool onKeyUp(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onPopMenuSelected(int type);

	virtual bool onDraw() const;
	virtual bool clear();

protected:
	virtual void onDirectionKeyDown(DirectionType type);
	virtual void onSpaceKeyDown();

	//virtual void translateSprite(const Vector& delta);
	//virtual void rotateSprite(const Vector& dst);
	//virtual void scaleSprite(const Vector& currPos);
	//virtual void shearSprite(const Vector& currPos);

	virtual void setRightPopupMenu(wxMenu& menu);
	virtual bool isOffsetEnable() const { return true; };

private:
	void setScalingFromSelected();

// private:
// 	class TranslateVisitor : public IVisitor
// 	{
// 	public:
// 		TranslateVisitor(const Vector& delta);
// 		virtual void visit(Object* object, bool& bFetchNext);
// 
// 	private:
// 		const Vector& m_delta;
// 
// 	}; // TranslateVisitor
// 
// 	class RotateVisitor : public IVisitor
// 	{
// 	public:
// 		RotateVisitor(const Vector& start, const Vector& end);
// 		virtual void visit(Object* object, bool& bFetchNext);
// 
// 	private:
// 		const Vector &m_start, &m_end;
// 
// 	}; // RotateVisitor

private:
	static const float SCALE_NODE_RADIUS;

private:
	PropertySettingPanel* m_propertyPanel;

	ArrangeSpriteImpl* m_impl;

protected:
	MultiSpritesImpl* m_spritesImpl;

	SpriteSelection* m_selection;
	Vector m_lastPos;

	ISprite* m_selected;

	Vector m_firstPos;

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
	bool m_isDeformOpen;

	bool m_bRightPress;

	CtrlNode m_ctrlNodeSelected;
	bool m_selOffset;

	bool m_bDirty;

	ShapeStyle m_shearNodeStyle;

	friend class SpritePropertySetting;

}; // ArrangeSpriteOP

}

#include "ArrangeSpriteOP.cpp"

