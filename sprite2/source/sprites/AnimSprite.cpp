#include "AnimSprite.h"
#include "AnimSymbol.h"

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
{
}

AnimSprite::AnimSprite(Symbol* sym) 
	: Sprite(sym)
	, m_curr(VI_DOWNCASTING<AnimSymbol*>(sym))
	, m_loop(true)
{
	Start();
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

bool AnimSprite::Update(const RenderParams& params, float dt)
{
	return m_curr.Update(params, dt);
}

void AnimSprite::Start()
{
	m_curr.Start();
}

}