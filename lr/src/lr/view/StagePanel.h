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
		d2d::PropertySettingPanel* property, d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual bool Update(int version);

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void TraverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

	void DebugDraw() const;

	void Pathfinding(const d2d::Vector& start, const d2d::Vector& end);
	void PointQuery(const d2d::Vector& pos);

	void SetLayers(const std::vector<Layer*>& layers);
	const std::vector<Layer*>& GetLayers() const { return m_layers; }

	void BuildGrids(int w, int h);
	const Grids* GetGrids() const { return m_grids; }

	const CharacterAllDirections* GetCharaDirs() const { return &m_chara_dirs; }

	d2d::AbstractEditOP* GetBaseOP() { return m_arrange_op; }

	void SetUpdateState(bool enable) {
		m_enable_update = enable;
	}

	d2d::LibraryPanel* GetLibrary() { return m_library; }

protected:
	virtual void OnMouseHook(wxMouseEvent& event);
	virtual void OnKeyHook(int key_code);

private:
	void ChangeEditOP();

	void ReorderSprite(d2d::ISprite* spr, bool up);
	void ReorderSpriteMost(d2d::ISprite* spr, bool up);
	void InsertSprite(d2d::ISprite* spr);
	void RemoveSprite(d2d::ISprite* spr);
	void ClearSprite();

	void RemoveShape(d2d::IShape* shape);
	void InsertShape(d2d::IShape* shape);
	void ClearShape();

private:
	d2d::LibraryPanel* m_library;

	d2d::AbstractEditOP* m_arrange_op;

	Grids* m_grids;

	Quadtree* m_sindex;

	preview::IPathfinding* m_pathfinding;

	std::vector<Layer*> m_layers;

	CharacterAllDirections m_chara_dirs;

	bool m_enable_update;

}; // StagePanel

}

#endif // _LR_STAGE_PANEL_H_
