#ifndef _EASYICON_ICON_H_
#define _EASYICON_ICON_H_

#include <json/json.h>

#include <ee/Object.h>
#include <ee/Rect.h>

#include <SM_Matrix.h>

namespace ee { class Image; }

namespace eicon
{

class Icon : public ee::Object
{
public:
	Icon();
	Icon(ee::Image* img);
	virtual ~Icon();

	virtual const char* GetIconDesc() const = 0;

	virtual void LoadFromFile(const Json::Value& value) = 0;
	virtual void StoreToFile(Json::Value& value) const = 0;

	// return [0, 1]
	virtual void GetBound(float process, sm::vec2 bound[4]) const = 0;

	virtual void Draw(const sm::mat4& mt, float process) const;

	virtual ee::Rect GetRegion(float process) const;

	void GetTexCoords(float process, sm::vec2* tex_coords) const;
	void GetScreenCoords(float process, const sm::vec2* tex_coords,
		sm::vec2* screen_coords) const;

	void ReloadTexture() const;

	void SetImage(ee::Image* img);
	const ee::Image* GetImage() const { return m_img; }

protected:
	virtual void AfterSetImage() {}

protected:
	ee::Image* m_img;

}; // Icon

}

#endif // _EASYICON_ICON_H_