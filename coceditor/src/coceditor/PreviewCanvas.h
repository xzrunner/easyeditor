#pragma once

#include <drag2d.h>

namespace coceditor
{
	class PreviewCanvas : public d2d::OrthoCanvas
	{
	public:
		PreviewCanvas(d2d::EditPanel* stage, d2d::LibraryPanel* library,
			const std::vector<d2d::ISprite*>& sprites);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		void drawStageData();

		void getAnimChildren(d2d::AnimSprite* anim, 
			std::vector<d2d::ISprite*>& sprites) const;

		static d2d::AnimSymbol::Frame* getCurrFrame(d2d::AnimSymbol::Layer* layer, int index);

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;
		int m_currFrame;

		d2d::LibraryPanel* m_library;

		const std::vector<d2d::ISprite*>& m_sprites;
		size_t m_maxFrame;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

