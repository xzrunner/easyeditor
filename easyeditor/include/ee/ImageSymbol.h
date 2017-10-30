#ifndef _EASYEDITOR_IMAGE_SYMBOL_H_
#define _EASYEDITOR_IMAGE_SYMBOL_H_

#include "Symbol.h"
#include "Image.h"

#include <sprite2/ImageSymbol.h>

namespace ee
{

class ImageSymbol : public Symbol, public s2::ImageSymbol
{
public:
	ImageSymbol();
	ImageSymbol(const ImageSymbol& img) {}
	ImageSymbol(const ImagePtr& image, const std::string& filename);

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
	virtual bool OnQueryTexcoordsFail() const;

	unsigned int GetTexID() const;

	const ImagePtr& GetImage() const { return m_image; }
	void SetImage(const ImagePtr& img) { m_image = img; }

protected:
	virtual bool LoadResources();

#ifdef S2_DEBUG
	virtual bool IsProxyImg() const { return false; }
#endif // S2_DEBUG

private:
	void InitCoreTex();

	void LoadSync();
	void LoadAsync();

	static void LoadCB(const void* res_path, void (*unpack)(const void* data, size_t size, void* ud), void* ud);
	static void ParserCB(const void* data, size_t size, void* ud);
	static void ReleaseCB(void* ud);

protected:
	ImagePtr m_image = nullptr;

}; // ImageSymbol

}

#endif // _EASYEDITOR_IMAGE_SYMBOL_H_