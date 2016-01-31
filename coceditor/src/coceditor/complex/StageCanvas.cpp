#include "StageCanvas.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

namespace coceditor
{
namespace complex
{
	BEGIN_EVENT_TABLE(StageCanvas, ee::OrthoCanvas)
		EVT_MOUSE_EVENTS(StageCanvas::onMouse)
		EVT_KEY_DOWN(StageCanvas::onKeyDown)
	END_EVENT_TABLE()

	StageCanvas::StageCanvas(StagePanel* editPanel)
		: ee::OrthoCanvas(editPanel)
		, m_editPanel(editPanel)
	{
	}

	void StageCanvas::initGL()
	{
		ee::OrthoCanvas::initGL();
		m_editPanel->getLibraryPanel()->reloadTexture();
	}

	void StageCanvas::onDraw()
	{
// 		m_editPanel->traverseSprites(ee::DrawSpritesVisitor(m_batch), ee::e_visible);
// 		m_editPanel->drawEditTemp();

		std::vector<ee::ISprite*> sprites;
		m_editPanel->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			ee::SpriteDraw::drawSprite(sprites[i]);
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