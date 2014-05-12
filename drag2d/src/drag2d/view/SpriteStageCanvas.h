#pragma once

#include "OrthoCanvas.h"

#include "render/SpriteBatch.h"

namespace d2d
{
	class EditPanel;
	class MultiSpritesImpl;

	class SpriteStageCanvas : public OrthoCanvas
	{
	public:
		SpriteStageCanvas(EditPanel* editPanel, MultiSpritesImpl* spritesImpl);

	protected:
		virtual void onDraw();

	protected:
		MultiSpritesImpl* m_spritesImpl;

		SpriteBatch m_batch;

	}; // SpriteStageCanvas
}

