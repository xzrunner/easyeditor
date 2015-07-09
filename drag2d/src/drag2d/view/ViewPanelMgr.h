#ifndef _DRAG2D_VIEW_PANEL_MGR_H_
#define _DRAG2D_VIEW_PANEL_MGR_H_

#include <vector>

namespace d2d
{

class ISprite;
class ISpriteViewPanel;
class IShapeViewPanel;
class SpriteSelection;
class IShape;
class ShapeSelection;

class ViewPanelMgr
{
public:
	ViewPanelMgr() : m_locked(false) {}

	void AddSpritePanel(ISpriteViewPanel* panel);
	void AddShapePanel(IShapeViewPanel* panel);

	void SelectSprite(ISprite* spr, bool clear, ISpriteViewPanel* expect);
	void SelectMultiSprites(SpriteSelection* selection, ISpriteViewPanel* expect);
	void ReorderSprite(ISprite* spr, bool up, ISpriteViewPanel* expect);
	void InsertSprite(ISprite* spr, ISpriteViewPanel* expect);
	void RemoveSprite(ISprite* spr, ISpriteViewPanel* expect);

	void SelectShape(IShape* shape, IShapeViewPanel* expect);
	void SelectMultiShapes(ShapeSelection* selection, IShapeViewPanel* expect);

private:
	std::vector<ISpriteViewPanel*> m_spr_panels;
	std::vector<IShapeViewPanel*> m_shape_panels;

	bool m_locked;

}; // ViewPanelMgr

}

#endif // _DRAG2D_VIEW_PANEL_MGR_H_