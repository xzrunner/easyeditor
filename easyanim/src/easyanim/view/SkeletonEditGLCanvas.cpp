#include "SkeletonEditGLCanvas.h"
#include "SkeletonSymbolList.h"
#include "tools/Render.h"

namespace eanim
{

BEGIN_EVENT_TABLE(SkeletonEditGLCanvas, d2d::OrthoCanvas)
	EVT_MOUSE_EVENTS(SkeletonEditGLCanvas::onMouse)
END_EVENT_TABLE()

SkeletonEditGLCanvas::SkeletonEditGLCanvas(d2d::EditPanel* parent, WholeSkeleton* skeleton, 
										   SkeletonSymbolList* symbolList)
	: d2d::OrthoCanvas(parent)
{
	m_skeleton = skeleton;
	m_symbolList = symbolList;
}

void SkeletonEditGLCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
 	m_symbolList->reloadAllTexture();
}

void SkeletonEditGLCanvas::onDraw()
{
	m_skeleton->onDraw();
	m_editPanel->drawEditTemp();
}

void SkeletonEditGLCanvas::onMouse(wxMouseEvent& event)
{
	m_editPanel->onMouse(event);
}

} // eanim