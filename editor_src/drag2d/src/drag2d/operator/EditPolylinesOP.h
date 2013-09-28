#pragma once

#include <map>

#include "interfaces.h"
#include "SelectShapesOP.h"

namespace d2d
{
	class ChainShape;
	class EditPolylinesCMPT;

	class EditPolylinesOP : public SelectShapesOP
	{
	public:
		EditPolylinesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel, EditPolylinesCMPT* cmpt);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void simplify();
		void updateFromSimplified();

	private:
		void clearBuffer();

	private:
		class UpdateChainVisitor : public IVisitor
		{
		public:
			virtual void visit(Object* object, bool& bFetchNext);
		}; // UpdateChainVisitor

		class UpdateBufferVisitor : public IVisitor
		{
		public:
			UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer);

			virtual void visit(Object* object, bool& bFetchNext);

		private:
			std::map<ChainShape*, ChainShape*>& m_simplifyBuffer;

		}; // UpdateBufferVisitor

		class OffsetVisitor : public IVisitor
		{
		public:
			OffsetVisitor(const Vector& offset);

			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_offset;

		}; // OffsetVisitor

	private:
		EditPolylinesCMPT* m_cmpt;

		std::map<ChainShape*, ChainShape*> m_simplifyBuffer;

		Vector m_lastPos;

		bool m_bDirty;

	}; // EditPolylinesOP
}

