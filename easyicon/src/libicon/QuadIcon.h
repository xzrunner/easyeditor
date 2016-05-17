#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

namespace eicon
{

class QuadIcon : public Icon
{
public:
	QuadIcon();
	QuadIcon(ee::Image* img, const sm::vec2* src, 
		const sm::vec2* screen);

	virtual const char* GetIconDesc() const { return "quad"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, sm::vec2 bound[4]) const;

	virtual void Draw(const sm::mat4& mt, float process) const;

	virtual sm::rect GetRegion(float process) const;

	const sm::vec2* GetScreen() const { return m_screen; }
	void SetScreen(const sm::vec2* screen);

protected:
	virtual void AfterSetImage();

private:
	sm::vec2 m_src[4], m_screen[4];

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_