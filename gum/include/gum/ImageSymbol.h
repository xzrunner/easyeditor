#ifndef _GUM_IMAGE_SYMBOL_H_
#define _GUM_IMAGE_SYMBOL_H_

#include <sprite2/ImageSymbol.h>

#include <string>

namespace simp { class NodePicture; }

namespace gum
{

class Image;

class ImageSymbol : public s2::ImageSymbol
{
public:
	ImageSymbol();
	virtual ~ImageSymbol();

	/**
	 *  @interface
	 *    s2::ImageSymbol
	 */
	virtual sm::vec2 GetNoTrimedSize() const;

	void SetImage(Image* img);
	const Image* GetImage() const { return m_img; }

	void SetRegion(const sm::ivec2& min, const sm::ivec2& max);

	virtual void QueryTexcoords(float* texcoords, int& texid) const;

protected:
	virtual void Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const;
	virtual bool IsOrthoCam() const;
	virtual void GetScreenSize(int& w, int& h) const;
	virtual float GetP3dCamAngle() const;
	virtual int GetScreenCacheTexid() const;

private:
	Image* m_img;

	float m_texcoords[8];

	bool m_packed;

}; // ImageSymbol

}

#endif // _GUM_IMAGE_SYMBOL_H_
