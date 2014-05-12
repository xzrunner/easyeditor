#ifndef _ESHAPE_STAGE_PANEL_H_
#define _ESHAPE_STAGE_PANEL_H_

#include <drag2d.h>

namespace eshape
{
	class LibraryItem;
	class ToolbarPanel;

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

		void setToolbar(ToolbarPanel* toolbar) {
			m_toolbar = toolbar;
		}

	private:
		LibraryItem* m_item;

		ToolbarPanel* m_toolbar;

		friend class StageCanvas;

	}; // StagePanel
}

#endif // _ESHAPE_STAGE_PANEL_H_