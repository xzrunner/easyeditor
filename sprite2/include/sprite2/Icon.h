#ifndef _SPRITE2_ICON_H_
#define _SPRITE2_ICON_H_

#include "pre_defined.h"

#include <CU_RefCountObj.h>
#include <SM_Vector.h>
#include <SM_Rect.h>

namespace s2
{

class RenderParams;
class ImageSymbol;

class Icon : public cu::RefCountObj
{
public:
	Icon();
	Icon(const Icon& icon);
	Icon& operator = (const Icon& icon);
	virtual ~Icon() {}

	virtual void Draw(const RenderParams& params, float process) const;
	
	virtual sm::rect GetRegion(float process) const;

	void GenTexcoords(float process, sm::vec2* texcoords) const;
	void GenVertices(float process, const sm::vec2* texcoords,
		sm::vec2* vertices) const;

	void SetImage(s2::ImageSymbol* img);
	const s2::ImageSymbol* GetImage() const { return m_img; }

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const = 0;
	
	virtual void Update() {}

protected:
	ImageSymbol* m_img;

}; // Icon

}

#endif // _SPRITE2_ICON_H_