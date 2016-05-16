#ifndef _EASYANIM_LAYERS_LOADER_H_
#define _EASYANIM_LAYERS_LOADER_H_

#include "Symbol.h"

#include <sprite2/AnimSymbol.h>

#include <json/json.h>

namespace eanim
{

class LayersLoader
{
public:
	void LoadLayers(const Json::Value& value, const std::string& dir,
		std::vector<s2::AnimSymbol::Layer*>& layers) const;

protected:
	virtual std::string GetSymbolPath(const std::string& dir, 
		const Json::Value& json_val) const = 0;

}; // LayersLoader

}

#endif // _EASYANIM_LAYERS_LOADER_H_