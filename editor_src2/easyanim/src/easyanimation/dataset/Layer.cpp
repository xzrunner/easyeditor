#include "Layer.h"
#include "KeyFrame.h"

#include "frame/Context.h"
#include "view/KeysPanel.h"

namespace eanim
{

Layer::Layer()
	: m_spriteObserver(*this)
{
	static int count = 0;
	name = "Layer" + StringTools::intToString(count++);

	insertKeyFrame(1);
}

Layer::~Layer()
{
	clear();
}

bool Layer::isKeyFrame(int time) const
{
	return m_frames.find(time) != m_frames.end();
}

void Layer::insertFrame(int time)
{
	std::vector<KeyFrame*> frames;
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first >= time)
		{
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		}
		else
			++itr;
	}

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		KeyFrame* frame = frames[i];
		frame->setTime(frame->getTime() + 1);
		insert(frame->getTime(), frame);
	}
}

void Layer::removeFrame(int time)
{
	if (m_frames.find(time) != m_frames.end())
		return;

	std::vector<KeyFrame*> frames;
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first >= time)
		{
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		}
		else
			++itr;
	}

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		KeyFrame* frame = frames[i];
		frame->setTime(frame->getTime() - 1);
		insert(frame->getTime(), frame);
	}
}

void Layer::insertKeyFrame(KeyFrame* frame)
{
	std::pair<std::map<int, KeyFrame*>::iterator, bool> status 
		= insert(frame->getTime(), frame);
	if (!status.second)
	{
		if (frame != status.first->second)
		{
			if (status.first->second->getTime() == Context::Instance()->maxFrame)
				static_cast<KeysPanel*>(Context::Instance()->keysPanel)->setCurrPos(INT_MAX);

			delete status.first->second;
			status.first->second = frame;
		}
	}

	if (frame->getTime() > Context::Instance()->maxFrame)
		Context::Instance()->maxFrame = frame->getTime();

	Context::Instance()->currFrame = frame->getTime();
}

void Layer::insertKeyFrame(int time)
{
	if (!m_frames.empty())
	{
		std::map<int, KeyFrame*>::iterator itr = m_frames.end();
		--itr;
		if (itr->first < time)
		{
			KeyFrame* frame = new KeyFrame(time);
			insert(time, frame);
			frame->copyKeyFrame(itr->second);

			if (time > Context::Instance()->maxFrame)
				Context::Instance()->maxFrame = time;

			Context::Instance()->currFrame = time;
		}
		else
			insert(time, new KeyFrame(time));
	}
	else
		insert(1, new KeyFrame(1));
}

void Layer::removeKeyFrame(int time)
{
	if (time == 1) return;

	std::map<int, KeyFrame*>::iterator itr = m_frames.find(time);
	if (itr != m_frames.end())
	{
		delete itr->second;
		m_frames.erase(itr);

		static_cast<KeysPanel*>(Context::Instance()->keysPanel)->setCurrPos(INT_MAX);

		Context::Instance()->maxFrame = Context::Instance()->currFrame;
	}
}

// void Layer::insertSprite(d2d::ISprite* sprite, int iFrame)
// {
// 	KeyFrame* frame = getCurrKeyFrame(iFrame);
// 	if (frame)
// 		frame->insertSprite(sprite);
// }
// 
// void Layer::removeSprite(d2d::ISprite* sprite, int iFrame)
// {
// 	KeyFrame* frame = getCurrKeyFrame(iFrame);
// 	if (frame)
// 		frame->removeSprite(sprite);
// }

KeyFrame* Layer::getCurrKeyFrame(int iFrame)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.lower_bound(iFrame);
	if (itr == m_frames.end()) return NULL;

	if (itr->first == iFrame) 
		return itr->second;
	else
	{
		if (itr == m_frames.begin()) return NULL;

		--itr;
		assert(itr->first <= iFrame);
		if (itr->first > iFrame) return NULL;

		return itr->second;
	}
}

KeyFrame* Layer::getNextKeyFrame(int iFrame)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.upper_bound(iFrame);
	if (itr == m_frames.end()) return NULL;
	else return itr->second;
}

int Layer::getFrameCount() const
{
	if (m_frames.empty()) return 0; 

	std::map<int, KeyFrame*>::const_iterator itr = m_frames.end();
	--itr;
	return itr->first;
}

void Layer::clear()
{
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); ++itr) delete itr->second;
	m_frames.clear();
}

std::pair<std::map<int, KeyFrame*>::iterator, bool> 
Layer::insert(int index, KeyFrame* frame)
{
	frame->setLayer(this);
	return m_frames.insert(std::make_pair(index, frame));
}

} // eanim