#ifndef RAIDEN_PREVIEW_CANVAS_H
#define RAIDEN_PREVIEW_CANVAS_H

#include <drag2d.h>

namespace raiden
{
	class PreviewCanvas : public d2d::OrthoCanvas
	{
	public:
		PreviewCanvas(d2d::EditPanel* editPanel);
		virtual ~PreviewCanvas();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

		static const int FRAME_RATE = 24;

	private:
		struct Sprite
		{
			d2d::ISprite* s;

			std::vector<d2d::Vector> lines;
			float speed;

			d2d::Vector base;
			d2d::Vector offset;
			float curr;

			bool finish;

			Sprite(d2d::ISprite* sprite);
		};

	private:
		void loadSprites();
		void drawSprites();
		void updateSprites(float dt);

	private:
		wxTimer m_timer;
		float m_yOffset;

		std::vector<Sprite*> m_sprites;

		std::set<Sprite*> m_buffer;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

#endif // RAIDEN_PREVIEW_CANVAS_H