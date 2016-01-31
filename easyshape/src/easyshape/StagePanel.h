#ifndef _ESHAPE_STAGE_PANEL_H_
#define _ESHAPE_STAGE_PANEL_H_



namespace eshape
{
	class LibraryItem;
	class ToolbarPanel;

	class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// ee::EditPanel interface
		//
		virtual void clear();

		//
		// ee::MultiShapesImpl interface
		//
		virtual void traverseShapes(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting) const;
		virtual void removeShape(ee::IShape* shape);
		virtual void insertShape(ee::IShape* shape);
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