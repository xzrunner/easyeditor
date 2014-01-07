#pragma once

#include <drag2d.h>

#include "SpriteObserver.h"

namespace eanim
{
	class KeyFrame;

	class Layer
	{
	public:
		Layer();
		~Layer();

		bool isKeyFrame(int time) const;

		void insertFrame(int time);
		void removeFrame(int time);

		void insertKeyFrame(KeyFrame* frame);
		void insertKeyFrame(int time);
		void removeKeyFrame(int time);

 		const std::map<int, KeyFrame*>& getAllFrames() const {
 			return m_frames;
 		}

// 		void insertSprite(d2d::ISprite* sprite, int iFrame);
// 		void removeSprite(d2d::ISprite* sprite, int iFrame);

		KeyFrame* getCurrKeyFrame(int iFrame);
		KeyFrame* getNextKeyFrame(int iFrame);

		int getFrameCount() const;

	public:
		std::string name;

	private:
		void clear();

		std::pair<std::map<int, KeyFrame*>::iterator, bool> 
			insert(int index, KeyFrame* frame);

	private:
		std::map<int, KeyFrame*> m_frames;

		SpriteObserver m_spriteObserver;

		friend class KeyFrame;

	}; // Layer
}

