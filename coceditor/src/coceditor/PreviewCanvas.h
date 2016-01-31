#pragma once



namespace coceditor
{
	class PreviewCanvas : public ee::OrthoCanvas
	{
	public:
		PreviewCanvas(ee::EditPanel* stage, ee::LibraryPanel* library,
			const std::vector<ee::ISprite*>& sprites);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		void drawStageData();

		void getAnimChildren(ee::AnimSprite* anim, 
			std::vector<ee::ISprite*>& sprites) const;

		static ee::AnimSymbol::Frame* getCurrFrame(ee::AnimSymbol::Layer* layer, int index);

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;
		int m_currFrame;

		ee::LibraryPanel* m_library;

		const std::vector<ee::ISprite*>& m_sprites;
		size_t m_maxFrame;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

