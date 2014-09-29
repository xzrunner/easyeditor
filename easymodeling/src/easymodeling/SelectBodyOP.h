
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
			d2d::AbstractEditCMPT* callback = NULL);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseLeftDClick(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

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
