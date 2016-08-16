#ifndef _EASYEDITOR_IMAGE_SYMBOL_H_
#define _EASYEDITOR_IMAGE_SYMBOL_H_

#include "Symbol.h"

#include <sprite2/ImageSymbol.h>

namespace ee
{

class Image;

class ImageSymbol : public Symbol, public s2::ImageSymbol
{
public:
	ImageSymbol();
	ImageSymbol(const ImageSymbol& img) {}
	ImageSymbol(Image* image, const std::string& filename);
	virtual ~ImageSymbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;
	virtual void InvalidRect(const sm::mat4& mt) const;

	unsigned int GetTexID() const;

	Image* GetImage() const { return m_image; }

protected:
	virtual void QueryTexcoords(float* texcoords, int& texid) const;
	virtual void Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const;
	virtual bool IsOrthoCam() const;
	virtual void GetScreenSize(int& w, int& h) const;
	virtual float GetP3dCamAngle() const;
	virtual int GetScreenCacheTexid() const;

	virtual void LoadResources();

private:
	void InitCoreTex();

protected:
	Image* m_image;

}; // ImageSymbol

}

#endif // _EASYEDITOR_IMAGE_SYMBOL_H_