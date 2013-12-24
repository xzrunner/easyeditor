#pragma once

#include <drag2d.h>

namespace eanim
{
	class KeyFrame
	{
	public:
		KeyFrame(int time);
		~KeyFrame();

		bool isEmpty() const {
			return m_sprites.empty();
		}

		std::vector<d2d::ISprite*>& getAllSprites() {
			return m_sprites;
		}

		bool hasClassicTween() const {
			return m_bClassicTween;
		}
		void setClassicTween(bool tween = true) {
			m_bClassicTween = tween;
		}

		int getTime() const {
			return m_time;
		}
		void setTime(int time) { m_time = time; }

		void clear();

	private:
		int m_time;

		std::vector<d2d::ISprite*> m_sprites;

		bool m_bClassicTween;

	public:
		int m_id;

	}; // KeyFrame
}

