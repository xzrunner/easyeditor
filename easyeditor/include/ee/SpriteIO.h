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
	virtual void LoadGeometry(const s2::SprPtr& spr) override;
	virtual void StoreGeometry(const s2::SprConstPtr& spr) override;
	virtual void LoadGeometry(const Json::Value& val) override;
	virtual void StoreGeometry(Json::Value& val) override;

	// info
	virtual void LoadInfo(const s2::SprPtr& spr) override;
	virtual void StoreInfo(const s2::SprConstPtr& spr) override;
	virtual void LoadInfo(const Json::Value& val) override;
	virtual void StoreInfo(Json::Value& val) override;

public:
	// geometry
	sm::vec2 m_perspective;

	// info
	std::string   m_tag;
	bool     m_clip;
	bool     m_anchor;

}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_