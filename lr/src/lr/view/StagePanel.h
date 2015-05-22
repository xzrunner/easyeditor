#ifndef _LR_STAGE_PANEL_H_
#define _LR_STAGE_PANEL_H_

#include <drag2d.h>

#include "RightPopupMenu.h"

#include "dataset/CharacterAllDirections.h"
#include "preview/IPathfinding.h"

namespace lr
{

class Grids;
class Quadtree;
class Layer;

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl, public d2d::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::PropertySettingPanel* property,
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void traverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual void removeSprite(d2d::ISprite* sprite);
	virtual void insertSprite(d2d::ISprite* sprite);
	virtual void clearSprites();
	virtual bool resetSpriteOrder(d2d::ISprite* sprite, bool up);

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void traverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;
	virtual void removeShape(d2d::IShape* shape);
	virtual void insertShape(d2d::IShape* shape);
	virtual void clearShapes();

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}

	void DebugDraw() const;

	void Pathfinding(const d2d::Vector& start, const d2d::Vector& end);
	void PointQuery(const d2d::Vector& pos);

	void SetLayers(const std::vector<Layer*>& layers);
	const std::vector<Layer*>& GetLayers() const { return m_layers; }

	void BuildGrids(int w, int h);
	const Grids* GetGrids() const { return m_grids; }

	Layer* GetCurrLayer() const;

	const CharacterAllDirections* GetCharaDirs() const { return &m_chara_dirs; }

protected:
	virtual void OnRightPopupMenu(int menu_id);

	virtual void OnMouseHook(wxMouseEvent& event);
	virtual void OnKeyHook(int key_code);

private:
	void ChangeEditOP();

private:
	d2d::LibraryPanel* m_library;
	d2d::ViewlistPanel* m_viewlist;

	d2d::AbstractEditOP* m_arrange_op;

	Grids* m_grids;

	Quadtree* m_sindex;

	preview::IPathfinding* m_pathfinding;

	std::vector<Layer*> m_layers;

	RightPopupMenu m_popup;

	CharacterAllDirections m_chara_dirs;

}; // StagePanel

}

#endif // _LR_STAGE_PANEL_H_
