#pragma once


namespace eanim
{
	class Sprite;

	class SpriteEditGLCanvas : public ee::OrthoCanvas
	{
	public:
		SpriteEditGLCanvas(ee::EditPanel* parent, Sprite* sprite);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);

	private:
		Sprite* m_sprite;

	}; // SpriteEditGLCanvas
}

