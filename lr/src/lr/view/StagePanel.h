#ifndef _LR_STAGE_PANEL_H_
#define _LR_STAGE_PANEL_H_

#include <drag2d.h>

#include "dataset/SymbolsCfg.h"
#include "preview/IPathfinding.h"

namespace lr
{

class SymbolsCfg;
class Quadtree;
class Layer;

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
//	, public d2d::MultiShapesImpl
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
		d2d::TraverseType type = d2d::e_allExisting, bool order = true) const;
	virtual void removeSprite(d2d::ISprite* sprite);
	virtual void insertSprite(d2d::ISprite* sprite);
	virtual void clearSprites();
	virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

	SymbolsCfg& GetSymbolsCfg() { return m_symbols_cfg; }

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}

	void DebugDraw() const;

	void Pathfinding(const d2d::Vector& start, const d2d::Vector& end);
	void PointQuery(const d2d::Vector& pos);

protected:
	virtual void OnMouseHook(wxMouseEvent& event);

private:
	void ChangeEditOP();

	Layer* GetCurrLayer() const;

private:
	SymbolsCfg m_symbols_cfg;

	d2d::LibraryPanel* m_library;
	d2d::ViewlistPanel* m_viewlist;

	d2d::AbstractEditOP* m_paste_op;
	d2d::AbstractEditOP* m_arrange_op;

	Quadtree* m_sindex;

	preview::IPathfinding* m_pathfinding;

}; // StagePanel

}

#endif // _LR_STAGE_PANEL_H_
