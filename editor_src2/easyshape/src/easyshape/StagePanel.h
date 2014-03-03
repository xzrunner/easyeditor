
#ifndef ESHAPE_STAGE_PANEL_H
#define ESHAPE_STAGE_PANEL_H

#include <drag2d.h>

namespace eshape
{
	class LibraryItem;

	class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// d2d::EditPanel interface
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

		void changeCurrItem(LibraryItem* item);

	private:
		LibraryItem* m_item;

		friend class StageCanvas;

	}; // StagePanel
}

#endif // ESHAPE_STAGE_PANEL_H