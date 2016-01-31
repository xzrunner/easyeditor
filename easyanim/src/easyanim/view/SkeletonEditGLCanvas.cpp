#include "SkeletonEditGLCanvas.h"
#include "SkeletonSymbolList.h"
#include "tools/Render.h"

namespace eanim
{

BEGIN_EVENT_TABLE(SkeletonEditGLCanvas, ee::OrthoCanvas)
	EVT_MOUSE_EVENTS(SkeletonEditGLCanvas::onMouse)
END_EVENT_TABLE()

SkeletonEditGLCanvas::SkeletonEditGLCanvas(ee::EditPanel* parent, WholeSkeleton* skeleton, 
										   SkeletonSymbolList* symbolList)
	: ee::OrthoCanvas(parent)
{
	m_skeleton = skeleton;
	m_symbolList = symbolList;
}

void SkeletonEditGLCanvas::initGL()
{
	ee::OrthoCanvas::initGL();
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