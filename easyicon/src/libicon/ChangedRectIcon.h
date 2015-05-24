#ifndef _EASYICON_CHANGED_RECT_ICON_H_
#define _EASYICON_CHANGED_RECT_ICON_H_

#include "Icon.h"

namespace eicon
{

class ChangedRectIcon : public Icon
{
public:
	ChangedRectIcon();

	virtual const char* GetIconDesc() const { return "changed-rect"; }

	virtual void Draw(const d2d::Matrix& mt) const;
	virtual void SetProcess(float proc);

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

protected:
	virtual void OnImageChanged();

private:
	d2d::Rect m_begin, m_end;
	float m_proc;

	d2d::Rect m_curr;

}; // ChangedRectIcon

}

#endif // _EASYICON_CHANGED_RECT_ICON_H_