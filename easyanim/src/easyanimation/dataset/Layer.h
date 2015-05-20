#pragma once

#include <drag2d.h>

#include "SpriteObserver.h"

namespace eanim
{
	class Controller;
	class KeyFrame;

	class Layer
	{
	public:
		Layer(Controller* ctrl);
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
		KeyFrame* getPrevKeyFrame(int iFrame);

		int GetMaxFrame() const;

	public:
		std::string name;

		bool editable, visible;

	private:
		void clear();

		std::pair<std::map<int, KeyFrame*>::iterator, bool> 
			insert(int index, KeyFrame* frame);

	private:
		Controller* m_ctrl;

		std::map<int, KeyFrame*> m_frames;

		SpriteObserver m_spriteObserver;

		friend class KeyFrame;

	}; // Layer
}

