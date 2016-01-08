#include "DrawSelectedShapeVisitor.h"

#include "dataset/IShape.h"
#include "common/Matrix.h"

namespace d2d
{

void DrawSelectedShapeVisitor::Visit(Object* object, bool& bFetchNext) 
{
	IShape* shape = static_cast<IShape*>(object);
	ColorTrans col;
	col.multi.set(1, 0, 0);
	shape->Draw(Matrix(), col);
	bFetchNext = true;
}

}