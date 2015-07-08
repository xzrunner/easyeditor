#ifndef _LR_STAGE_PANEL_H_
#define _LR_STAGE_PANEL_H_

#include <drag2d.h>

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
		d2d::LibraryPanel* library,
		d2d::ViewPanelMgr* view_panel_mgr);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(d2d::ISprite* sprite);
	virtual bool RemoveSprite(d2d::ISprite* sprite);
	virtual bool ClearAllSprite();
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;

	//
	// d2d::MultiShapesImpl interface
	//
	virtual bool InsertShape(d2d::IShape* shape);
	virtual bool RemoveShape(d2d::IShape* shape);
	virtual bool ClearAllShapes();
	virtual void TraverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

	void DebugDraw() const;

	void Pathfinding(const d2d::Vector& start, const d2d::Vector& end);
	void PointQuery(const d2d::Vector& pos);

	void SetLayers(const std::vector<Layer*>& layers);
	const std::vector<Layer*>& GetLayers() const { return m_layers; }

	void BuildGrids(int w, int h);
	const Grids* GetGrids() const { return m_grids; }

	Layer* GetCurrLayer() const;

	const CharacterAllDirections* GetCharaDirs() const { return &m_chara_dirs; }

	d2d::AbstractEditOP* GetBaseOP() { return m_arrange_op; }

protected:
	virtual void OnMouseHook(wxMouseEvent& event);
	virtual void OnKeyHook(int key_code);

private:
	void ChangeEditOP();

private:
	d2d::ViewPanelMgr* m_view_panel_mgr;

	d2d::LibraryPanel* m_library;

	d2d::AbstractEditOP* m_arrange_op;

	Grids* m_grids;

	Quadtree* m_sindex;

	preview::IPathfinding* m_pathfinding;

	std::vector<Layer*> m_layers;

	CharacterAllDirections m_chara_dirs;

}; // StagePanel

}

#endif // _LR_STAGE_PANEL_H_
