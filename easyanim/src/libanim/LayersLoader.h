#ifndef _EASYANIM_LAYERS_LOADER_H_
#define _EASYANIM_LAYERS_LOADER_H_

#include <gum/AnimSymLoader.h>

namespace eanim
{

class Symbol;

class LayersLoader : public gum::AnimSymLoader
{
public:
	LayersLoader(Symbol* sym);

protected:
	/**
	 *  @interface
	 *    gum::AnimSymLoader
	 */	
	virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

	virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const = 0;

}; // LayersLoader

}

#endif // _EASYANIM_LAYERS_LOADER_H_