#include "Layer.h"
#include "KeyFrame.h"

#include "dataset/Sprite.h"

namespace eanim
{

Layer::Layer()
{
	static int count = 0;
	m_name = "Layer" + StringTools::intToString(count++);
}

Layer::~Layer()
{
	clear();
}

void Layer::loadFromTextFile(std::ifstream& fin)
{
	clear();

	m_name = StringTools::getLine(fin);

	std::string flag;
	size_t size;
	fin >> flag >> size;
	for (size_t i = 0; i < size; ++i)
	{
		size_t time;
		fin >> time;
		KeyFrame* frame = new KeyFrame(time);
		frame->loadFromTextFile(fin);
		m_frames.insert(std::make_pair(time, frame));
	}
}

void Layer::storeToTextFile(std::ofstream& fout) const
{
	fout << m_name << '\n';
	fout << "frame " << m_frames.size() << '\n';
	std::map<int, KeyFrame*>::const_iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); ++itr)
	{
		fout << itr->first << " ";
		itr->second->storeToTextFile(fout);
		fout << '\n';
	}
}

void Layer::insertKeyFrame(int time, KeyFrame* frame)
{
	if (!m_frames.empty())
	{
		std::map<int, KeyFrame*>::iterator itr = m_frames.end();
		--itr;
		if (itr->first < time)
		{
			const std::vector<Sprite*>& src = itr->second->getAllSprites();
			for (size_t i = 0, n = src.size(); i < n; ++i)
			{
			//	frame->insertSprite(static_cast<Sprite*>(src[i]->clone()));
				frame->insertSprite(src[i]->clone());
			}
		}
	}
	else
	{
		if (time != 1)
			m_frames.insert(std::make_pair(1, new KeyFrame(1)));
	}
	m_frames.insert(std::make_pair(time, frame));
}

bool Layer::isKeyFrame(int time) const
{
	return m_frames.find(time) != m_frames.end();
}

void Layer::deleteKeyFrame(int time)
{
	if (time == 1) return;

	std::map<int, KeyFrame*>::iterator itr = m_frames.find(time);
	if (itr != m_frames.end())
	{
		delete itr->second;
		m_frames.erase(itr);
	}
}

const std::string& Layer::getName() const
{
	return m_name;
}

void Layer::setName(const std::string& name)
{
	m_name = name;
}

const std::map<int, KeyFrame*>& Layer::getAllFrames() const
{
	return m_frames;
}

bool Layer::insertSprite(Sprite* sprite, int currFrame)
{
	KeyFrame* frame = getCurrKeyFrame(currFrame);
	if (frame) 
	{
		frame->insertSprite(sprite);
		return true;
	}
	else
	{
		return false;
	}
}

bool Layer::removeSprite(ee::ISprite* sprite)
{
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); ++itr)
	{
		if (itr->second->removeSprite(sprite))
			return true;
	}
	return false;
}

KeyFrame* Layer::getCurrKeyFrame(int currFrame)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.lower_bound(currFrame);
	if (itr == m_frames.end()) return NULL;

	if (itr->first == currFrame) 
		return itr->second;
	else
	{
		if (itr == m_frames.begin()) return NULL;

		--itr;
		assert(itr->first <= currFrame);
		if (itr->first > currFrame) return NULL;

		return itr->second;
	}
}

KeyFrame* Layer::getNextKeyFrame(int currFrame)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.upper_bound(currFrame);
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

} // eanim