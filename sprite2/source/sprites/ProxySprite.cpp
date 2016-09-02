#include "ProxySprite.h"

namespace s2
{

static const std::string PROXY_NAME = "_PROXY_";

ProxySprite::ProxySprite()
{
	m_name = PROXY_NAME;
}

ProxySprite::ProxySprite(Symbol* sym)
	: Sprite(sym) 
{
}

ProxySprite* ProxySprite::Clone() const
{
	return new ProxySprite(*this);
}

bool ProxySprite::IsProxyName(const std::string& name)
{
	return name == PROXY_NAME;
}

}