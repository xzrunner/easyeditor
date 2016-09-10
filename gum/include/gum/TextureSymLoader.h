#ifndef _GUM_TEXTURE_SYM_LOADER_H_
#define _GUM_TEXTURE_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class TextureSymbol; }

namespace gum
{

class TextureSymLoader : private cu::Uncopyable
{
public:
	TextureSymLoader(s2::TextureSymbol* sym);
	~TextureSymLoader();

	void LoadJson(const std::string& filepath);	

private:
	s2::TextureSymbol* m_sym;

}; // TextureSymLoader

}

#endif // _GUM_TEXTURE_SYM_LOADER_H_