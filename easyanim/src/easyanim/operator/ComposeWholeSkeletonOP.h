#pragma once

#include "dataset/WholeSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class SkeletonEditPanel;

	class ComposeWholeSkeletonOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ComposeWholeSkeletonOP(SkeletonEditPanel* editPanel, 
			WholeSkeleton* skeleton, wxTextCtrl* layerTextCtrl);
		virtual ~ComposeWholeSkeletonOP();

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightUp(int x, int y);

		virtual bool onDraw() const;

		void absorbSprite(WholeSkeleton::Sprite* sprite);

		void setSelectedBoneLayer(int layer);

	protected:
		virtual void translateSprite(const d2d::Vector& delta);
		virtual void rotateSprite(const d2d::Vector& dst);

	private:
		void connect(WholeSkeleton::Sprite* parent, WholeSkeleton::Sprite* child,
			WholeSkeleton::Node* from, WholeSkeleton::Node* to);

		void updateLayerInfo();

		void translateToCenter();

	private:
		class NearestNodeVisitor : public d2d::IVisitor
		{
		public:
			NearestNodeVisitor(WholeSkeleton::Sprite* sprite, WholeSkeleton::Sprite** pResult, 
				WholeSkeleton::Node** pFromNode, WholeSkeleton::Node** pToNode);

			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		private:
			WholeSkeleton::Sprite* m_sprite;

			WholeSkeleton::Sprite** m_pResult;
			WholeSkeleton::Node **m_pFromNode, **m_pToNode;

			float m_nearestDis;

		}; // NearestNodeVisitor

	private:
		WholeSkeleton* m_skeleton;

		WholeSkeleton::Sprite* m_selected;

		wxTextCtrl* m_layerTextCtrl;

	}; // ComposeWholeSkeletonOP
}

