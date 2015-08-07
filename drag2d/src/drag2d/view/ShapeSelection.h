#ifndef _DRAG2D_SHAPE_SELECTION_H_
#define _DRAG2D_SHAPE_SELECTION_H_

#include "dataset/IShape.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class EditPanelImpl;
class IStageCanvas;

class ShapeSelection : public ObjSelectionSet<IShape>
{
public:
	ShapeSelection(EditPanelImpl* stage);
	virtual ~ShapeSelection();

	virtual void Clear();

	virtual void Add(IShape* item);
	virtual void Remove(IShape* item);

private:
	EditPanelImpl* m_stage;

}; // ShapeSelection

}

#endif // _DRAG2D_SHAPE_SELECTION_H_