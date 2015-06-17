
#ifndef EMODELING_SELECT_BODY_OP_H
#define EMODELING_SELECT_BODY_OP_H

#include <drag2d.h>

namespace libmodeling { class Body; }

namespace emodeling
{
	class SelectBodyOP : public d2d::SelectSpritesOP
	{
	public:
		SelectBodyOP(d2d::EditPanel* editPanel, 
			d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, 
			d2d::ViewPanelMgr* view_panel_mgr = NULL,
			d2d::AbstractEditCMPT* callback = NULL);

		virtual bool OnKeyDown(int keyCode);
		virtual bool OnMouseMove(int x, int y);
		virtual bool OnMouseLeftDClick(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	private:
		class DrawSelectedVisitor : public d2d::IVisitor
		{
		public:
			virtual void visit(d2d::Object* object, bool& bFetchNext);
		}; // DrawSelectedVisitor

	private:
		libmodeling::Body* m_mouseOn;

	}; // SelectBodyOP
}

#endif // EMODELING_SELECT_BODY_OP_H
