#pragma once

namespace d2d
{
	class DrawSelectedShapeVisitor : public IVisitor
	{
	public:
		virtual void visit(Object* object, bool& bFetchNext) 
		{
			IShape* shape = static_cast<IShape*>(object);
			shape->draw(d2d::Matrix(), Colorf(1, 0, 0));
			bFetchNext = true;
		}

	}; // DrawSelectedShapeVisitor
}

