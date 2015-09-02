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

	virtual void LoadFromFile(const Json::Value& value) = 0;
	virtual void StoreToFile(Json::Value& value) const = 0;

	// return [0, 1]
	virtual void GetBound(float process, d2d::Vector bound[4]) const = 0;

	void Draw(const d2d::Matrix& mt, float process) const;

	d2d::Rect GetRegion(float process) const;

	void GetTexCoords(float process, d2d::Vector* tex_coords) const;
	void GetScreenCoords(float process, const d2d::Vector* tex_coords,
		d2d::Vector* screen_coords) const;

	void ReloadTexture() const;

	void SetImage(d2d::Image* img);
	const d2d::Image* GetImage() const { return m_img; }

protected:
	d2d::Image* m_img;

	d2d::Rect m_canvas_region;	// [0, 1]

}; // Icon

}

#endif // _EASYICON_ICON_H_