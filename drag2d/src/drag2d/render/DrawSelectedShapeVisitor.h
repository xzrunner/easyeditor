#pragma once

namespace d2d
{
	class DrawSelectedShapeVisitor : public IVisitor
	{
	public:
		DrawSelectedShapeVisitor(const Screen& scr) : m_scr(scr) {}

		virtual void visit(Object* object, bool& bFetchNext) 
		{
			IShape* shape = static_cast<IShape*>(object);
			shape->draw(m_scr, Colorf(1, 0, 0));
			bFetchNext = true;
		}

	private:
		const Screen& m_scr;

	}; // DrawSelectedShapeVisitor
}

