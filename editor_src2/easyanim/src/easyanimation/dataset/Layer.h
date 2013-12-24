#pragma once

#include <drag2d.h>

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

	private:
		std::map<int, KeyFrame*> m_frames;

	}; // Layer
}

