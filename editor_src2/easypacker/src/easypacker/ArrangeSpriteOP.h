#ifndef EPACKER_ARRANGE_SPRITE_OP_H
#define EPACKER_ARRANGE_SPRITE_OP_H

#include <drag2d.h>

namespace epacker
{
	class StagePanel;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* editPanel, 
			d2d::PropertySettingPanel* propertyPanel,
			d2d::AbstractEditCMPT* callback = NULL);

		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);

		virtual bool onDraw() const;

	private:
		class FixCoordsVisitor : public d2d::IVisitor
		{
		public:
			virtual void visit(d2d::Object* object, bool& bFetchNext);
		}; // FixCoordsVisitor

	private:
		StagePanel* m_editPanel;

	}; // ArrangeSpriteOP
}

#endif // EPACKER_ARRANGE_SPRITE_OP_H