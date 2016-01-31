#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

namespace eicon
{

class QuadIcon : public Icon
{
public:
	QuadIcon();
	QuadIcon(ee::Image* img, const ee::Vector* src, 
		const ee::Vector* screen);

	virtual const char* GetIconDesc() const { return "quad"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	virtual void GetBound(float process, ee::Vector bound[4]) const;

	virtual void Draw(const ee::Matrix& mt, float process) const;

	virtual ee::Rect GetRegion(float process) const;

	const ee::Vector* GetScreen() const { return m_screen; }
	void SetScreen(const ee::Vector* screen);

protected:
	virtual void AfterSetImage();

private:
	ee::Vector m_src[4], m_screen[4];

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_