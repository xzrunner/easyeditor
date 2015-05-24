#ifndef _EASYICON_RECT_ICON_H_
#define _EASYICON_RECT_ICON_H_

#include "Icon.h"

namespace eicon
{

class RectIcon : public Icon
{
public:
	RectIcon();

	virtual const char* GetIconDesc() const { return "rect"; }

	virtual void Draw(const d2d::Matrix& mt) const;

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

	void SetHoriRegion(float xmin, float xmax);
	void SetVertRegion(float ymin, float ymax);

protected:
	virtual void OnImageChanged();

private:
	d2d::Vector m_min, m_max;

}; // RectIcon

}

#endif // _EASYICON_RECT_ICON_H_