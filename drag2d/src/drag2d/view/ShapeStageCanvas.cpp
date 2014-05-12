#include "ShapeStageCanvas.h"

#include "view/EditPanel.h"
#include "view/MultiShapesImpl.h"
#include "render/DrawShapesVisitor.h"

namespace d2d
{

ShapeStageCanvas::ShapeStageCanvas(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
								   const Colorf& color/* = Colorf(0.0f, 0.0f, 0.0f)*/)
	: OrthoCanvas(editPanel)
	, m_color(color)
	, m_shapesImpl(shapesImpl)
{
}

void ShapeStageCanvas::onDraw()
{
	m_shapesImpl->traverseShapes(DrawShapesVisitor(m_color), e_visible);
	m_editPanel->drawEditTemp();
}

} // d2d