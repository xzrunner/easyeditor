#pragma once

#include "dataset/WholeSkeleton.h"



namespace eanim
{
	class SkeletonEditPanel;

	class ComposeWholeSkeletonOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>
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
		virtual void translateSprite(const ee::Vector& delta);
		virtual void rotateSprite(const ee::Vector& dst);

	private:
		void connect(WholeSkeleton::Sprite* parent, WholeSkeleton::Sprite* child,
			WholeSkeleton::Node* from, WholeSkeleton::Node* to);

		void updateLayerInfo();

		void translateToCenter();

	private:
		class NearestNodeVisitor : public ee::IVisitor
		{
		public:
			NearestNodeVisitor(WholeSkeleton::Sprite* sprite, WholeSkeleton::Sprite** pResult, 
				WholeSkeleton::Node** pFromNode, WholeSkeleton::Node** pToNode);

			virtual void visit(ee::ICloneable* object, bool& bFetchNext);

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

