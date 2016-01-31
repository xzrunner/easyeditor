#ifndef RAIDEN_PREVIEW_CANVAS_H
#define RAIDEN_PREVIEW_CANVAS_H



namespace raiden
{
	class PreviewCanvas : public ee::OrthoCanvas
	{
	public:
		PreviewCanvas(ee::EditPanel* editPanel);
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
			ee::ISprite* s;

			std::vector<ee::Vector> lines;
			float speed;

			ee::Vector base;
			ee::Vector offset;
			float curr;

			bool finish;

			Sprite(ee::ISprite* sprite);
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