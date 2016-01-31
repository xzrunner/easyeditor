#include "KeyFrame.h"
#include "Sprite.h"
#include "SpriteFactory.h"

namespace eanim
{

KeyFrame::KeyFrame(int time)
{
	m_time = time;
	m_bClassicTween = false;
}

KeyFrame::~KeyFrame()
{
	clear();
}

void KeyFrame::loadFromTextFile(std::ifstream& fin)
{
	clear();

	fin >> m_bClassicTween;

	std::string flag;
	size_t size;
	fin >> flag >> size;
	m_sprites.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		int type;
		fin >> type;
		Sprite* sprite = SpriteFactory::Instance()->createSprite(SpriteFactory::SPRITE_TYPE(type));
		sprite->loadFromTextFile(fin);
		m_sprites.push_back(sprite);
	}
}

void KeyFrame::storeToTextFile(std::ofstream& fout) const
{
	fout << m_bClassicTween << " ";
	fout << "sprite " << m_sprites.size() << '\n';
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->storeToTextFile(fout);
}

bool KeyFrame::isEmpty() const
{
	return m_sprites.empty();
}

void KeyFrame::insertSprite(Sprite* sprite)
{
	m_sprites.push_back(sprite);
}

bool KeyFrame::removeSprite(ee::ISprite* sprite)
{
	std::vector<Sprite*>::iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr)
	{
		if (*itr == sprite)
		{
			m_sprites.erase(itr);
			return true;
		}
	}
	return false;
}

const std::vector<Sprite*>& KeyFrame::getAllSprites() const
{
	return m_sprites;
}

bool KeyFrame::hasClassicTween() const
{
	return m_bClassicTween;
}

void KeyFrame::setClassicTween(bool has/* = true*/)
{
	m_bClassicTween = has;
}

int KeyFrame::getTime() const
{
	return m_time;
}

void KeyFrame::clear()
{
	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<Sprite>());
	m_sprites.clear();
}

} // eanim