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
	 *    ee::Symbol
	 */
	virtual void InvalidRect(const S2_MAT& mt) const;

	/**
	 *  @interface
	 *    s2::ImageSymbol
	 */
	virtual bool QueryTexcoords(bool use_dtex, float* texcoords, int& texid) const;

	unsigned int GetTexID() const;

	Image* GetImage() const { return m_image; }
	void   SetImage(Image* img);

protected:
	virtual bool LoadResources();

private:
	void InitCoreTex();

	void LoadSync();
	void LoadAsync();

	static void LoadCB(const char* filepath, void (*unpack)(const void* data, size_t size, void* ud), void* ud);
	static void ParserCB(const void* data, size_t size, void* ud);
	static void ReleaseCB(void* ud);

protected:
	Image* m_image;

}; // ImageSymbol

}

#endif // _EASYEDITOR_IMAGE_SYMBOL_H_