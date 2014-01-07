#pragma once

#include <vector>

#include "interfaces.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class EditPanel;
	class MultiSpritesImpl;
	class PropertySettingPanel;
	class AbstractEditCMPT;

	template <typename TBase>
	class ArrangeSpriteOP : public TBase
	{
	public:
		ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL,
			bool scaleOpen = true);
		virtual ~ArrangeSpriteOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onKeyUp(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onPopMenuSelected(int type);

		virtual bool onDraw() const;
		virtual bool clear();

	protected:
		virtual void translateSprite(const Vector& delta);
		virtual void rotateSprite(const Vector& dst);
		virtual void scaleSprite(const Vector& currPos);

	private:
		void setScalingFromSelected();

		void autoAlign(const std::vector<ISprite*>& sprites);
		void autoAlign(const ISprite* src, ISprite* dst);

	private:
		class TranslateVisitor : public IVisitor
		{
		public:
			TranslateVisitor(const Vector& delta);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_delta;

		}; // TranslateVisitor

		class RotateVisitor : public IVisitor
		{
		public:
			RotateVisitor(const Vector& start, const Vector& end);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector &m_start, &m_end;

		}; // RotateVisitor

	private:
		static const float SCALE_NODE_RADIUS;

	private:
		PropertySettingPanel* m_propertyPanel;

	protected:
		MultiSpritesImpl* m_spritesImpl;

		SpriteSelection* m_selection;
		Vector m_lastPos;

		ISprite* m_scaling;

	private:
		bool m_scaleOpen;

		bool m_bRightPress;

		Vector m_firstPos;

		Vector m_scaleSelected;

		bool m_bDirty;

		bool m_autoAlignOpen;
		Vector m_autoAlignHor[2], m_autoAlignVer[2];

		friend class SpritePropertySetting;

	}; // ArrangeSpriteOP
}

#include "ArrangeSpriteOP.cpp"

