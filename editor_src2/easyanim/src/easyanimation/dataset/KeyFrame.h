#pragma once

#include "Sprite.h"

namespace eanim
{
	class KeyFrame
	{
	public:
		KeyFrame(int time);
		~KeyFrame();

		void copySprites(const KeyFrame* src);

		bool isEmpty() const {
			return m_sprites.empty();
		}

		void insert(d2d::ISprite* sprite);
		bool remove(const d2d::ISprite* sprite);
		void reorder(const d2d::ISprite* sprite, bool up);

		size_t size() const { return m_sprites.size(); }
		const d2d::ISprite* getSprite(int index) {
			if (index >= 0 && index < m_sprites.size())
				return m_sprites[index]->curr;
			else
				return NULL;
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

		static void getTweenSprites(const KeyFrame* start, const KeyFrame* end, std::vector<d2d::ISprite*>& tween, float process);

	private:
		static void getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process);

	private:
		int m_time;

		std::vector<Sprite*> m_sprites;

		bool m_bClassicTween;

	public:
		int m_id;

	}; // KeyFrame
}

