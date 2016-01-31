
#ifndef EBUILDER_PREVIEW_ANIMATION_CONTROL_H
#define EBUILDER_PREVIEW_ANIMATION_CONTROL_H



namespace ebuilder
{
	namespace preview
	{
		class Canvas;

		class ITimeObserver
		{
		public:
			virtual ~ITimeObserver() {}

			virtual void onTick() = 0;
		};

		class AnimationControl : public wxWindow
		{
		public:
			static AnimationControl* Instance();

			void insertObserver(ITimeObserver* observer);
			void removeObserver(ITimeObserver* observer);

			void registerCanvas(Canvas* canvas) {
				m_canvas = canvas;
			}

			void start() { m_timer.Start(1000 / FRAME_RATE); }
			void stop() { m_timer.Stop(); }

			void clear();

		private:
			AnimationControl();

			void onTimer(wxTimerEvent& event);

			void notifyObservers();

		private:
			enum
			{
				TIMER_ID = 1000
			};

			static const int FRAME_RATE = 60;

		private:
			static AnimationControl* m_instance;

			Canvas* m_canvas;

			wxTimer m_timer;

			std::set<ITimeObserver*> m_observers;

			DECLARE_EVENT_TABLE()

		}; // AnimationControl
	}
}

#endif // EBUILDER_PREVIEW_ANIMATION_CONTROL_H
