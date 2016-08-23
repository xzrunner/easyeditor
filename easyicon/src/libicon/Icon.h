#ifndef _EASYICON_ICON_H_
#define _EASYICON_ICON_H_

#include <sprite2/Icon.h>

#include <json/json.h>

namespace ee { class Image; }

namespace eicon
{

class Icon : public virtual s2::Icon
{
public:
	Icon();
	Icon(ee::Image* img);
	virtual ~Icon();

	virtual const char* GetIconDesc() const = 0;

	virtual void LoadFromFile(const Json::Value& value) = 0;
	virtual void StoreToFile(Json::Value& value) const = 0;

	void SetImage(ee::Image* img);
	const ee::Image* GetImage() const { return m_img; }

protected:
	ee::Image* m_img;

}; // Icon

}

#endif // _EASYICON_ICON_H_