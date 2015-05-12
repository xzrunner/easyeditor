#pragma once

#include <drag2d.h>

#include "SkeletonData.h"

namespace eanim
{
	class Controller;
	class Layer;

	class KeyFrame
	{
	public:
		KeyFrame(Controller* ctrl, int time);
		~KeyFrame();

		void setLayer(Layer* layer) {
			m_layer = layer;
		}

		void copyKeyFrame(const KeyFrame* src);

		bool isEmpty() const {
			return m_sprites.empty();
		}

		void insert(d2d::ISprite* sprite);
		void insertWithClone(d2d::ISprite* sprite);
		bool remove(d2d::ISprite* sprite);
		void reorder(const d2d::ISprite* sprite, bool up);

		size_t size() const { return m_sprites.size(); }
		const d2d::ISprite* getSprite(int index) {
			if (index >= 0 && index < m_sprites.size())
				return m_sprites[index];
			else
				return NULL;
		}

		const std::vector<d2d::ISprite*>& getAllSprites() const {
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

		SkeletonData& getSkeletonData() { return m_skeletonData; }

		void getTweenSprites(const KeyFrame* start, const KeyFrame* end, std::vector<d2d::ISprite*>& tween, float process) const;

	private:
		void getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process) const;

		bool canSpritesTween(const d2d::ISprite& begin, const d2d::ISprite& end) const;

	private:
		Controller* m_ctrl;

		int m_layer_idx, m_frame_idx;

		int m_time;

		std::vector<d2d::ISprite*> m_sprites;

		bool m_bClassicTween;

		Layer* m_layer;

		SkeletonData m_skeletonData;

	public:
		int m_id;

	}; // KeyFrame
}

