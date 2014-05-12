#include "StageCanvas.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

namespace coceditor
{
namespace complex
{
	BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
		EVT_MOUSE_EVENTS(StageCanvas::onMouse)
		EVT_KEY_DOWN(StageCanvas::onKeyDown)
	END_EVENT_TABLE()

	StageCanvas::StageCanvas(StagePanel* editPanel)
		: d2d::OrthoCanvas(editPanel)
		, m_editPanel(editPanel)
	{
	}

	void StageCanvas::initGL()
	{
		d2d::OrthoCanvas::initGL();
		m_editPanel->getLibraryPanel()->reloadTexture();
	}

	void StageCanvas::onDraw()
	{
// 		m_editPanel->traverseSprites(d2d::DrawSpritesVisitor(m_batch), d2d::e_visible);
// 		m_editPanel->drawEditTemp();

		std::vector<d2d::ISprite*> sprites;
		m_editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			d2d::SpriteDraw::drawSprite(sprites[i]);
	}

	void StageCanvas::onMouse(wxMouseEvent& event)
	{
		m_editPanel->onMouse(event);
	}

	void StageCanvas::onKeyDown(wxKeyEvent& event)
	{
		m_editPanel->onKeyDown(event);
	}

}
}