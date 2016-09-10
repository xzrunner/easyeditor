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

	// edit
	virtual void LoadEdit(s2::Sprite* spr);
	virtual void StoreEdit(const s2::Sprite* spr);
	virtual void LoadEdit(const Json::Value& val);
	virtual void StoreEdit(Json::Value& val);
	
public:
	// geometry
	sm::vec2		m_perspective;

	// info
	std::string		m_tag;
	bool			m_clip;
	bool			m_anchor;

	// edit
	bool			m_editable;

}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_