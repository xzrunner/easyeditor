#ifndef _EASYEDITOR_SPRITE_IO_H_
#define _EASYEDITOR_SPRITE_IO_H_

#include <gum/SpriteIO.h>

namespace ee
{

class SpriteIO : public gum::SpriteIO
{
public:
	SpriteIO();

protected:
	// geometry
	virtual void LoadGeometry(s2::Sprite* spr);
	virtual void StoreGeometry(const s2::Sprite* spr);
	virtual void LoadGeometry(const Json::Value& val);
	virtual void StoreGeometry(Json::Value& val);

	// info
	virtual void LoadInfo(s2::Sprite* spr);
	virtual void StoreInfo(const s2::Sprite* spr);
	virtual void LoadInfo(const Json::Value& val);
	virtual void StoreInfo(Json::Value& val);

public:
	// geometry
	sm::vec2		m_perspective;

	// info
	std::string		m_tag;
	bool			m_clip;
	bool			m_anchor;

}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_