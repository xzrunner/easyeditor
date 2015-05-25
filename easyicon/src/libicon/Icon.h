#ifndef _EASYICON_ICON_H_
#define _EASYICON_ICON_H_

#include <drag2d.h>

namespace eicon
{

class Icon : public d2d::Object
{
public:
	Icon();
	Icon(d2d::Image* img);
	virtual ~Icon();

	virtual const char* GetIconDesc() const = 0;

	virtual void Draw(const d2d::Matrix& mt, float process) const = 0;

	virtual void LoadFromFile(const Json::Value& value) = 0;
	virtual void StoreToFile(Json::Value& value) const = 0;

	virtual void GetRegion(float process, d2d::Rect& region) const = 0;

	void ReloadTexture() const;

	void SetImage(d2d::Image* img);
	const d2d::Image* GetImage() const { return m_img; }

protected:
	void Draw(const d2d::Matrix& mt, const d2d::Vector r[4]) const;

protected:
	d2d::Image* m_img;

}; // Icon

}

#endif // _EASYICON_ICON_H_