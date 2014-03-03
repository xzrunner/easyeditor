#pragma once

#include <vector>

#include "DrawRectangleOP.h"

#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiShapesImpl;
	class AbstractEditCMPT;
	class PropertySettingPanel;
	class IPropertySetting;

	class SelectShapesOP : public DrawRectangleOP
	{
	public:
		SelectShapesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
			PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback = NULL);
		virtual ~SelectShapesOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		virtual IPropertySetting* createPropertySetting(IShape* shape) const;

	private:
		void clearClipboard();

	protected:
		Vector m_firstPos;

		ShapeSelection* m_selection;

		PropertySettingPanel* m_propertyPanel;

	private:
		AbstractEditCMPT* m_callback;

		MultiShapesImpl* m_shapeImpl;

		std::vector<IShape*> m_clipboard;

		// To disable mouse able when press ctrl and window query
		bool m_bDraggable;

	}; // SelectShapesOP
}

