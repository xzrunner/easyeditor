#pragma once
#include <drag2d.h>

namespace eanim
{
	class Sprite;

	class SpriteEditGLCanvas : public d2d::OrthoCanvas
	{
	public:
		SpriteEditGLCanvas(d2d::EditPanel* parent, Sprite* sprite);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);

	private:
		Sprite* m_sprite;

	}; // SpriteEditGLCanvas
}

