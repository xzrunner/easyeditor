#pragma once

#include <vector>

#include "interfaces.h"
#include "DrawRectangleOP.h"

#include "common/Rect.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiShapesImpl;
	class PropertySettingPanel;
	class AbstractEditCMPT;
	class ChainShape;

	class SelectNodesOP : public DrawRectangleOP
	{
	public:
		SelectNodesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
			PropertySettingPanel* propertyPanel, AbstractEditCMPT* callback = NULL);
		virtual ~SelectNodesOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void fetchSelectedNode(std::vector<Vector>& nodes) const;

		static int getThreshold();

	protected:
		void clearSelectedNodes();

	protected:
		struct ChainSelectedNodes
		{
		public:
			ChainShape* chain;
			std::vector<Vector> selectedNodes;
		}; // ChainSelectedNodes

	private:
		class PosQueryVisitor : public IVisitor
		{
		public:
			PosQueryVisitor(const Vector& pos, ChainSelectedNodes** result);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			Rect m_rect;
			ChainSelectedNodes** m_result;

		}; // PosQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, std::vector<ChainSelectedNodes*>& result);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			std::vector<ChainSelectedNodes*>& m_result;

		}; // RectQueryVisitor

	protected:
		std::vector<ChainSelectedNodes*> m_nodeSelection;

	private:
		MultiShapesImpl* m_shapeImpl;

		Vector m_firstPos;

	}; // SelectNodesOP
}

