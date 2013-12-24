
#ifndef LIBSHAPE_STAGE_PANEL_H
#define LIBSHAPE_STAGE_PANEL_H

#include <drag2d.h>

namespace libshape
{
	class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::ISymbol* symbol);
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::ISprite* sprite);
		virtual ~StagePanel();

		//
		// ICameraObserver interface
		//
		virtual void clear();

		//
		// d2d::MultiShapesImpl interface
		//
		virtual void traverseShapes(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting) const;
		virtual void removeShape(d2d::IShape* shape);
		virtual void insertShape(d2d::IShape* shape);
		virtual void clearShapes();

		virtual void loadShapes();
		virtual void storeShapes() const;

		const d2d::ISymbol& getSymbol() const { return m_symbol; }

	protected:
		const d2d::ISymbol& m_symbol;

		d2d::ISprite* m_sprite;

		mutable std::vector<d2d::IShape*> m_shapes;

	}; // StagePanel
}

#endif // LIBSHAPE_STAGE_PANEL_H