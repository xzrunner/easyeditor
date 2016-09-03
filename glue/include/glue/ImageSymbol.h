#ifndef _GLUE_IMAGE_SYMBOL_H_
#define _GLUE_IMAGE_SYMBOL_H_

#include <sprite2/ImageSymbol.h>

#include <string>

namespace glue
{

class Image;

class ImageSymbol : public s2::ImageSymbol
{
public:
	ImageSymbol(const std::string& filepath);
	virtual ~ImageSymbol();

protected:
	virtual void QueryTexcoords(float* texcoords, int& texid) const;
	virtual void Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const;
	virtual bool IsOrthoCam() const;
	virtual void GetScreenSize(int& w, int& h) const;
	virtual float GetP3dCamAngle() const;
	virtual int GetScreenCacheTexid() const;

private:
	Image* m_img;

}; // ImageSymbol

}

#endif // _GLUE_IMAGE_SYMBOL_H_